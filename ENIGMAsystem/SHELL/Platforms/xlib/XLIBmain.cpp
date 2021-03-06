/** Copyright (C) 2008-2011 IsmAvatar <cmagicj@nni.com>, Josh Ventura
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

//This file has been modified beyond recognition by Josh @ Dreamland
//under the pretense that it would be better compatible with ENIGMA

#include <X11/Xlib.h>
#include <GL/glx.h>
#include <unistd.h>
#include <stdio.h>
#include <string>

#include "Platforms/platforms_mandatory.h"

#include "XLIBmain.h"
#include "XLIBwindow.h"
#include "LINUXjoystick.h"

#include "Universal_System/var4.h"
#include "Universal_System/CallbackArrays.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/loading.h"

#include <time.h>

namespace enigma_user {
  const int os_type = os_linux;
  int keyboard_lastkey = 0;
  extern string keyboard_lastchar;
}

namespace enigma
{
  extern char keymap[512];
  extern char usermap[256];
  void ENIGMA_events(void); //TODO: Synchronize this with Windows by putting these two in a single header.

  namespace x11
  {
    Display *disp;
    Screen *screen;
    Window win;
    Atom wm_delwin;

    int handleEvents()
    {
      XEvent e;
      XNextEvent(disp, &e);
      int gk; unsigned char actualKey;
      switch (e.type)
      {
        case KeyPress: {
              gk=XLookupKeysym(&e.xkey,0);
              if (gk==NoSymbol)
                return 0;

              if (!(gk & 0xFF00)) actualKey = enigma::usermap[(int)enigma::keymap[gk & 0xFF]];
              else actualKey = enigma::usermap[(int)enigma::keymap[gk & 0x1FF]];
              { // Set keyboard_lastchar. Seems to work without 
                  char str[1];
                  int len = XLookupString(&e.xkey, str, 1, NULL, NULL);
                  if (len > 0) {
                      enigma_user::keyboard_lastchar = string(1,str[0]);
                  }
              }
	      enigma_user::keyboard_lastkey = actualKey;
	      enigma_user::keyboard_string += enigma_user::keyboard_lastchar;
              if (enigma::last_keybdstatus[actualKey]==1 && enigma::keybdstatus[actualKey]==0) {
                enigma::keybdstatus[actualKey]=1;
                return 0;
              }
              enigma::last_keybdstatus[actualKey]=enigma::keybdstatus[actualKey];
              enigma::keybdstatus[actualKey]=1;
              return 0;
        }
        case KeyRelease: {
            gk=XLookupKeysym(&e.xkey,0);
            if (gk == NoSymbol)
              return 0;

            if (!(gk & 0xFF00)) actualKey = enigma::usermap[(int)enigma::keymap[gk & 0xFF]];
            else actualKey = enigma::usermap[(int)enigma::keymap[gk & 0x1FF]];

            enigma::last_keybdstatus[actualKey]=enigma::keybdstatus[actualKey];
            enigma::keybdstatus[actualKey]=0;
          return 0;
        }
        case ButtonPress: {
            if (e.xbutton.button < 4) enigma::mousestatus[e.xbutton.button == 1 ? 0 : 4-e.xbutton.button] = 1;
            else switch (e.xbutton.button) {
              case 4: mouse_vscrolls++; break;
              case 5: mouse_vscrolls--; break;
              case 6: mouse_hscrolls++; break;
              case 7: mouse_hscrolls--; break;
              default: ;
            }
          return 0;
        }
        case ButtonRelease: {
            if (e.xbutton.button < 4) enigma::mousestatus[e.xbutton.button == 1 ? 0 : 4-e.xbutton.button] = 0;
            else switch (e.xbutton.button) {
              case 4: mouse_vscrolls++; break;
              case 5: mouse_vscrolls--; break;
              case 6: mouse_hscrolls++; break;
              case 7: mouse_hscrolls--; break;
              default: ;
            }
          return 0;
        }
        case Expose: {
            //screen_refresh();
          return 0;
        }
        case ClientMessage:
          if ((unsigned)e.xclient.data.l[0] == (unsigned)wm_delwin) //For some reason, this line warns whether we cast to unsigned or not.
            return 1;
          //else fall through
        default:
            printf("%d\n",e.type);
          return 0;
      }
      //Move/Resize = ConfigureNotify
      //Min = UnmapNotify
      //Restore = MapNotify
    }
  }
}

using namespace enigma::x11;

namespace enigma
{
  void input_initialize()
  {
    //Clear the input arrays
    for(int i=0;i<3;i++){
      last_mousestatus[i]=0;
      mousestatus[i]=0;
    }
    for(int i=0;i<256;i++){
      last_keybdstatus[i]=0;
      keybdstatus[i]=0;
    }

    init_joysticks();
  }

  void input_push()
  {
    for(int i=0;i<3;i++){
      last_mousestatus[i] = mousestatus[i];
    }
    for(int i=0;i<256;i++){
      last_keybdstatus[i] = keybdstatus[i];
    }
    mouse_hscrolls = mouse_vscrolls = 0;
  }

  int game_ending();
}

namespace enigma_user {
  extern double fps;
}

static inline long clamp(long value, long min, long max)
{
  if (value < min) return min;
  if (value > max) return max;
  return value;
}

static bool game_isending = false;
int main(int argc,char** argv)
{
  // Copy our parameters
	enigma::parameters = new string[argc];
	enigma::parameterc = argc;
	for (int i=0; i<argc; i++)
		enigma::parameters[i]=argv[i];
	enigma::initkeymap();


	// Initiate display
	disp = XOpenDisplay(NULL);
	if(!disp){
		printf("Display failed\n");
		return -1;
	}


	// Identify components (close button, root pane)
	wm_delwin = XInternAtom(disp,"WM_DELETE_WINDOW",False);
	Window root = DefaultRootWindow(disp);

	// Prepare openGL
	GLint att[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 24, None };
	XVisualInfo *vi = glXChooseVisual(disp,0,att);
	if(!vi){
		printf("GLFail\n");
		return -2;
	}

	// Window event listening and coloring
	XSetWindowAttributes swa;
	swa.border_pixel = 0;
	swa.background_pixel = 0;
	swa.colormap = XCreateColormap(disp,root,vi->visual,AllocNone);
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;// | StructureNotifyMask;
	unsigned long valmask = CWColormap | CWEventMask; //  | CWBackPixel | CWBorderPixel;

	//default window size
	int winw = enigma_user::room_width;
	int winh = enigma_user::room_height;
	win = XCreateWindow(disp,root,0,0,winw,winh,0,vi->depth,InputOutput,vi->visual,valmask,&swa);
	XMapRaised(disp,win); //request visible

	//prepare window for display (center, caption, etc)
	screen = DefaultScreenOfDisplay(disp);
	//printf("Screen: %d %d %d %d\n",s->width/2,s->height/2,winw,winh);
	XMoveWindow(disp,win,(screen->width-winw)/2,(screen->height-winh)/2);

	//geom();
	//give us a GL context
	GLXContext glxc = glXCreateContext(disp, vi, NULL, True);
	if (!glxc){
		printf("NoContext\n");
		return -3;
	}

	//apply context
	glXMakeCurrent(disp,win,glxc); //flushes
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_ACCUM_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

	/* XEvent e;//wait for server to report our display request
	do {
	XNextEvent(disp, &e); //auto-flush
	} while (e.type != MapNotify);*/

	//register CloseButton listener
	Atom prots[] = {wm_delwin};
	if (!XSetWMProtocols(disp,win,prots,1)) {
		printf("NoClose\n");
		return -4;
	}
	gmw_init(); //init gm window functions, flushes
	//#include "initialize.h"

	//Call ENIGMA system initializers; sprites, audio, and what have you
	enigma::initialize_everything();

	/*
	for(char q=1;q;ENIGMA_events())
		while(XQLength(disp))
			if(handleEvents()>0) q=0;
	glxc = glXGetCurrentContext();
	glXDestroyContext(disp,glxc);
	XCloseDisplay(disp);
	return 0;*/

	struct timespec time_offset;
	struct timespec time_offset_slowing;
	struct timespec time_current;
	clock_gettime(CLOCK_MONOTONIC, &time_offset);
	time_offset_slowing.tv_sec = time_offset.tv_sec;
	time_offset_slowing.tv_nsec = time_offset.tv_nsec;
	int frames_count = 0;

	while (!game_isending)
	{
		using enigma::current_room_speed;
		clock_gettime(CLOCK_MONOTONIC, &time_current);
		{
			long passed_mcs = (time_current.tv_sec - time_offset.tv_sec)*1000000 + (time_current.tv_nsec/1000 - + time_offset.tv_nsec/1000);
			passed_mcs = clamp(passed_mcs, 0, 1000000);
			if (passed_mcs >= 1000000) { // Handle resetting.
				enigma_user::fps = frames_count;
				frames_count = 0;
				time_offset.tv_sec += passed_mcs/1000000;
				time_offset_slowing.tv_sec = time_offset.tv_sec;
				time_offset_slowing.tv_nsec = time_offset.tv_nsec;
			}
		}

		if (current_room_speed > 0) {
			long spent_mcs = (time_current.tv_sec - time_offset_slowing.tv_sec)*1000000 + (time_current.tv_nsec/1000 - time_offset_slowing.tv_nsec/1000);
			spent_mcs = clamp(spent_mcs, 0, 1000000);
			long remaining_mcs = 1000000 - spent_mcs;
			long needed_mcs = long((1.0 - 1.0*frames_count/current_room_speed)*1e6);
			const int catchup_limit_ms = 50;
			if (needed_mcs > remaining_mcs + catchup_limit_ms*1000) {
				// If more than catchup_limit ms is needed than is remaining, we risk running too fast to catch up.
				// In order to avoid running too fast, we advance the offset, such that we are only at most catchup_limit ms behind.
				// Thus, if the load is consistently making the game slow, the game is still allowed to run as fast as possible
				// without any sleep.
				// And if there is very heavy load once in a while, the game will only run too fast for catchup_limit ms.
				time_offset_slowing.tv_nsec += 1000*(needed_mcs - (remaining_mcs + catchup_limit_ms*1000));
				spent_mcs = (time_current.tv_sec - time_offset_slowing.tv_sec)*1000000 + (time_current.tv_nsec/1000 - time_offset_slowing.tv_nsec/1000);
				spent_mcs = clamp(spent_mcs, 0, 1000000);
				remaining_mcs = 1000000 - spent_mcs;
				needed_mcs = long((1.0 - 1.0*frames_count/current_room_speed)*1e6);
			}
			if (remaining_mcs > needed_mcs) {
				const long sleeping_time = std::min((remaining_mcs - needed_mcs)/5, long(999999));
				usleep(std::max(long(1), sleeping_time));
				continue;
			}
		}

		while(XQLength(disp))
			if(handleEvents() > 0)
				goto end;

		enigma::handle_joysticks();
		enigma::ENIGMA_events();
		enigma::input_push();

		frames_count++;
	}

	end:
	enigma::game_ending();
  glXDestroyContext(disp,glxc);
  XCloseDisplay(disp);
	return 0;
}

namespace enigma_user
{

void game_end() {
  game_isending = true;
}
void action_end_game() {
  game_end();
}

int display_get_width() { return XWidthOfScreen(screen); }
int display_get_height() { return XHeightOfScreen(screen); }

}

