/*********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\*********************************************************************************/

/**
  @file    object.h
  @summary This file outlines the most basic local variables needed by different systems
*/

#ifndef _object_h
#define _object_h

const int self =   -1;
const int other =  -2;
const int all =    -3;
const int noone =  -4;
const int global = -5;
const int local =  -7;

#include <map>
#include "var_cr3.h"

namespace enigma
{
    extern int maxid;
    extern int instancecount;
    extern int id_current;

    extern double newinst_x, newinst_y;
    extern int newinst_obj, newinst_id;

    struct object_basic
    {
      const unsigned id;
      const int object_index;
      
      virtual enigma::variant myevent_create();
      
      virtual enigma::variant myevent_gamestart();
      virtual enigma::variant myevent_roomstart();
      virtual enigma::variant myevent_beginstep();
      virtual enigma::variant myevent_alarm();
      virtual enigma::variant myevent_keyboard();
      virtual enigma::variant myevent_keypress();
      virtual enigma::variant myevent_keyrelease();
      virtual enigma::variant myevent_mouse();
      virtual enigma::variant myevent_step();
      virtual enigma::variant myevent_pathend();
      virtual enigma::variant myevent_outsideroom();
      virtual enigma::variant myevent_boundary();
      virtual enigma::variant myevent_collision();
      virtual enigma::variant myevent_nomorelives();
      virtual enigma::variant myevent_nomorehealth();
      virtual enigma::variant myevent_endstep();
      virtual enigma::variant myevent_draw();
      virtual enigma::variant myevent_endanimation();
      virtual enigma::variant myevent_roomend();
      virtual enigma::variant myevent_gameend();

      virtual enigma::variant myevent_destroy();
      
      object_basic();
      object_basic(int uid, int uoid);
      virtual ~object_basic();
    };
    
    extern object_basic* ENIGMA_global_instance;
    
    //#include "imap.h"
    
    /*//imap instance_list;
    iiterator instance_iterator;
    iiterator instance_event_iterator;
    iiterator instance_with_iterator;*/
    
    extern std::map<int,object_basic*> instance_list;
    extern std::map<int,object_basic*>::iterator instance_iterator;
    extern std::map<int,object_basic*>::iterator instance_event_iterator;
    
    extern std::map<int,int> cleanups;
    extern int cleancount;
    
    
    void constructor(object_basic* instance);//This is the universal create event code
    
    void step_basic(object_basic* instance);
}

namespace enigma
{
    void constructor(object_basic* instance);
    void step_basic(object_basic* instance);
}

#endif //_object_h