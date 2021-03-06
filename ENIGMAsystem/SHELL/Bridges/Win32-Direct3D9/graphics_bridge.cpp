/** Copyright (C) 2013 Robert B. Colton
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

#include <string>
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <dxerr9.h>
using namespace std;

#include "libEGMstd.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/Win32/WINDOWSmain.h"
#include "Platforms/Win32/WINDOWSwindow.h"
#include "../General/DX9Device.h"
LPD3DXSPRITE dsprite = NULL;

// global declarations
LPDIRECT3D9 d3dr;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

namespace enigma
{
	
    void EnableDrawing (HGLRC *hRC)
    {
		HRESULT hr;
		
		D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information
		d3dr = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface
		D3DFORMAT format = D3DFMT_A8R8G8B8; //For simplicity we'll hard-code this for now.
		
		ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
		d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE; // 0 Levels of multi-sampling
		d3dpp.MultiSampleQuality = 0;                //No multi-sampling
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;  // Throw away previous frames, we don't need them
		d3dpp.hDeviceWindow = hWnd;  // This is our main (and only) window
		d3dpp.Flags = NULL;            // No flags to set
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; //Default Refresh Rate
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   //Present the frame immediately
		d3dpp.BackBufferCount = 1;  //We only need a single back buffer
		d3dpp.BackBufferFormat = format;      //Display format
		d3dpp.EnableAutoDepthStencil = TRUE; // Automatic depth stencil buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8; //32-bit zbuffer 24bits for depth 8 for stencil buffer
		
		// create a device class using this information and information from the d3dpp stuct
		hr = d3dr->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev);
		if(FAILED(hr)){
			MessageBox(hWnd,
               "Failed to create Direct3D 9.0 Device",
			   DXGetErrorDescription9(hr), //DXGetErrorString9(hr)
               MB_ICONERROR | MB_OK);
		}
		
		d3ddev->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE); 
		
		if (SUCCEEDED(D3DXCreateSprite(d3ddev,&dsprite)))
		{
			// created OK
		}
    }

    void DisableDrawing (HWND hWnd, HDC hDC, HGLRC hRC)
    {
		d3ddev->Release();    // close and release the 3D device
		d3dr->Release();    // close and release Direct3D
    }
}

#include "Universal_System/roomsystem.h"

namespace enigma_user
{

int display_aa = 0;

/* Two things wrong here...
a) this shit dont work
b) display_reset needs moved out of WINDOWSwindow.cpp/.h



// Not really sure if you have to reset everything or if you can just reset a few things.
void display_reset(int aa, bool vsync) {
		HRESULT hr;
		
		D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information
		D3DFORMAT format = D3DFMT_A8R8G8B8; //For simplicity we'll hard-code this for now.

		ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
		d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE; // 0 Levels of multi-sampling
		d3dpp.MultiSampleQuality = aa;                //No multi-sampling
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;  // Throw away previous frames, we don't need them
		d3dpp.hDeviceWindow = enigma::hWnd;  // This is our main (and only) window
		d3dpp.Flags = NULL;            // No flags to set
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; //Default Refresh Rate
		d3dpp.BackBufferCount = 1;  //We only need a single back buffer
		d3dpp.BackBufferFormat = format;      //Display format
		d3dpp.EnableAutoDepthStencil = TRUE; // Automatic depth stencil buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8; //32-bit zbuffer 24bits for depth 8 for stencil buffer
		d3dpp.Flags = NULL;            // No flags to set
		if (vsync) {
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;   //Present the frame immediately
		} else {
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   //Present the frame immediately
		}
		
		hr = d3ddev->Reset(&d3dpp);
}
*/

void screen_refresh() {
    window_set_caption(room_caption);
    enigma::update_mouse_variables();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}

void set_synchronization(bool enable) //TODO: Needs to be rewritten
{

}  

}

