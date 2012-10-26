/********************************************************************************\
**                                                                              **
**  Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>                          **
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
\********************************************************************************/

////////////////////////////////////
// Collision implementation functions - API dependant implementations of instances colliding with other things.
// In this case, we treat instances as their bounding box (BBox).
////////////////////////////////////

#include "Universal_System/collisions_object.h"
#include "Universal_System/instance_system.h" //iter
#include "Universal_System/instance.h"

#include "coll_impl.h"
#include <cmath>

static inline void get_border(int *leftv, int *rightv, int *topv, int *bottomv, int left, int top, int right, int bottom, double x, double y, double xscale, double yscale, double angle)
{
    if (angle == 0)
    {
        const bool xsp = (xscale >= 0), ysp = (yscale >= 0);
        const double lsc = left*xscale, rsc = (right+1)*xscale-1, tsc = top*yscale, bsc = (bottom+1)*yscale-1;

        *leftv   = (xsp ? lsc : rsc) + x + .5;
        *rightv  = (xsp ? rsc : lsc) + x + .5;
        *topv    = (ysp ? tsc : bsc) + y + .5;
        *bottomv = (ysp ? bsc : tsc) + y + .5;
    }
    else
    {
        const double arad = angle*(M_PI/180.0);
        const double sina = sin(arad), cosa = cos(arad);
        const double lsc = left*xscale, rsc = (right+1)*xscale-1, tsc = top*yscale, bsc = (bottom+1)*yscale-1;
        const int quad = int(fmod(fmod(angle, 360) + 360, 360)/90.0);
        const bool xsp = (xscale >= 0), ysp = (yscale >= 0),
                   q12 = (quad == 1 || quad == 2), q23 = (quad == 2 || quad == 3),
                   xs12 = xsp^q12, sx23 = xsp^q23, ys12 = ysp^q12, ys23 = ysp^q23;

        *leftv   = cosa*(xs12 ? lsc : rsc) + sina*(ys23 ? tsc : bsc) + x + .5;
        *rightv  = cosa*(xs12 ? rsc : lsc) + sina*(ys23 ? bsc : tsc) + x + .5;
        *topv    = cosa*(ys12 ? tsc : bsc) - sina*(sx23 ? rsc : lsc) + y + .5;
        *bottomv = cosa*(ys12 ? bsc : tsc) - sina*(sx23 ? lsc : rsc) + y + .5;
    }
}

static inline int min(int x, int y) { return x<y? x : y; }
static inline double min(double x, double y) { return x<y? x : y; }
static inline int max(int x, int y) { return x>y? x : y; }
static inline double max(double x, double y) { return x>y? x : y; }

bool precise_collision_single(int intersection_left, int intersection_right, int intersection_top, int intersection_bottom,
                                double x1, double y1,
                                double xscale1, double yscale1,
                                double ia1,
                                unsigned char* pixels1,
                                int w1, int h1,
                                int xoffset1, int yoffset1)
{
    
    if (xscale1 != 0.0 && yscale1 != 0.0) {

        const double pi_half = M_PI/2.0;
        const double arad = M_PI/180.0;

        const double arad1 = ia1*M_PI/180.0;

        const double cosa1 = cos(-arad1);
        const double sina1 = sin(-arad1);
        const double cosa90_1 = cos(-arad1 + pi_half);
        const double sina90_1 = sin(-arad1 + pi_half);

        for (unsigned int rowindex = intersection_top; rowindex <= intersection_bottom; rowindex++)
        {
            for(unsigned int colindex = intersection_left; colindex <= intersection_right; colindex++)
            {
                        
                //Test for single image.
                const int bx1 = (colindex - x1);
                const int by1 = (rowindex - y1);
                const int px1 = (int)((bx1*cosa1 + by1*sina1)/xscale1 + xoffset1);
                const int py1 = (int)((bx1*cosa90_1 + by1*sina90_1)/yscale1 + yoffset1);
                const bool p1 = px1 >= 0 && py1 >= 0 && px1 < w1 && py1 < h1 && pixels1[py1*w1 + px1] != 0;

                if (p1) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool precise_collision_pair(int intersection_left, int intersection_right, int intersection_top, int intersection_bottom,
                                double x1, double y1, double x2, double y2,
                                double xscale1, double yscale1, double xscale2, double yscale2,
                                double ia1, double ia2,
                                unsigned char* pixels1, unsigned char* pixels2,
                                int w1, int h1, int w2, int h2,
                                int xoffset1, int yoffset1, int xoffset2, int yoffset2)
{
    
    if (xscale1 != 0.0 && yscale1 != 0.0 && xscale2 != 0.0 && yscale2 != 0.0) {

        const double pi_half = M_PI/2.0;
        const double arad = M_PI/180.0;

        const double arad1 = ia1*M_PI/180.0;
        const double arad2 = ia2*M_PI/180.0;

        const double cosa1 = cos(-arad1);
        const double sina1 = sin(-arad1);
        const double cosa90_1 = cos(-arad1 + pi_half);
        const double sina90_1 = sin(-arad1 + pi_half);

        const double cosa2 = cos(-arad2);
        const double sina2 = sin(-arad2);
        const double cosa90_2 = cos(-arad2 + pi_half);
        const double sina90_2 = sin(-arad2 + pi_half);

        for (unsigned int rowindex = intersection_top; rowindex <= intersection_bottom; rowindex++)
        {
            for(unsigned int colindex = intersection_left; colindex <= intersection_right; colindex++)
            {
                        
                //Test for first image.
                const int bx1 = (colindex - x1);
                const int by1 = (rowindex - y1);
                const int px1 = (int)((bx1*cosa1 + by1*sina1)/xscale1 + xoffset1);
                const int py1 = (int)((bx1*cosa90_1 + by1*sina90_1)/yscale1 + yoffset1);
                const bool p1 = px1 >= 0 && py1 >= 0 && px1 < w1 && py1 < h1 && pixels1[py1*w1 + px1] != 0;
    
                //Test for second image.
                const int bx2 = (colindex - x2);
                const int by2 = (rowindex - y2);
                const int px2 = (int)((bx2*cosa2 + by2*sina2)/xscale2 + xoffset2);
                const int py2 = (int)((bx2*cosa90_2 + by2*sina90_2)/yscale2 + yoffset2);
                const bool p2 = px2 >= 0 && py2 >= 0 && px2 < w2 && py2 < h2 && pixels2[py2*w2 + px2] != 0;

                //Final test.
                if (p1 && p2) {
                    return true;
                }
            }
        }
    }
    return false;
}

enigma::object_collisions* const collide_inst_inst(int object, bool solid_only, bool notme, double x, double y)
{
    enigma::object_collisions* const inst1 = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);

    //If instance cannot collide with anything, stop.
    if (inst1->sprite_index == -1 && inst1->mask_index == -1) {
        return NULL;
    }

    const bbox_rect_t &box = inst1->$bbox_relative();
    const double xscale1 = inst1->image_xscale, yscale1 = inst1->image_yscale,
                 ia1 = inst1->image_angle;
    int left1, top1, right1, bottom1;

    get_border(&left1, &right1, &top1, &bottom1, box.left, box.top, box.right, box.bottom, x, y, xscale1, yscale1, ia1);

    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        enigma::object_collisions* const inst2 = (enigma::object_collisions*)*it;
        if (notme && inst2->id == inst1->id)
            continue;
        if (solid_only && !inst2->solid)
            continue;
        if (inst2->sprite_index == -1 && inst2->mask_index == -1) //no sprite/mask then no collision
            continue;

        const bbox_rect_t &box2 = inst2->$bbox_relative();
        const double x2 = inst2->x, y2 = inst2->y,
                     xscale2 = inst2->image_xscale, yscale2 = inst2->image_yscale,
                     ia2 = inst2->image_angle;
        int left2, top2, right2, bottom2;
        get_border(&left2, &right2, &top2, &bottom2, box2.left, box2.top, box2.right, box2.bottom, x2, y2, xscale2, yscale2, ia2);

        if (left1 <= right2 && left2 <= right1 && top1 <= bottom2 && top2 <= bottom1) {

            //Check per pixel.
            const int collsprite_index1 = inst1->mask_index != -1 ? inst1->mask_index : inst1->sprite_index;
            const int collsprite_index2 = inst2->mask_index != -1 ? inst2->mask_index : inst2->sprite_index;

            enigma::sprite* sprite1 = enigma::spritestructarray[collsprite_index1];
            enigma::sprite* sprite2 = enigma::spritestructarray[collsprite_index2];

            const int usi1 = ((int) inst1->image_index) % sprite1->subcount;
            const int usi2 = ((int) inst2->image_index) % sprite2->subcount;

            unsigned char* pixels1 = (unsigned char*) (sprite1->colldata[usi1]);
            unsigned char* pixels2 = (unsigned char*) (sprite2->colldata[usi2]);

            if (pixels1 == 0 && pixels2 == 0) { //bbox vs. bbox.
                return inst2;
            }
            else {
                //Intersection.
                const int ins_left = max(left1, left2);
                const int ins_right = min(right1, right2);
                const int ins_top = max(top1, top2);
                const int ins_bottom = min(bottom1, bottom2);

                const int w1 = sprite1->width;
                const int h1 = sprite1->height;
                const int w2 = sprite2->width;
                const int h2 = sprite2->height;

                const double xoffset1 = sprite1->xoffset;
                const double yoffset1 = sprite1->yoffset;
                const double xoffset2 = sprite2->xoffset;
                const double yoffset2 = sprite2->yoffset;

                if (pixels1 != 0 && pixels2 == 0) { //precise vs. bbox.
                    const bool coll_result = precise_collision_single(
                        ins_left, ins_right, ins_top, ins_bottom,
                        x, y,
                        xscale1, yscale1,
                        ia1,
                        pixels1,
                        w1, h1,
                        xoffset1, yoffset1
                      );

                    if (coll_result) {
                        return inst2;
                    }
                }
                else if (pixels1 == 0 && pixels2 != 0) { //bbox vs. precise.
                    const bool coll_result = precise_collision_single(
                        ins_left, ins_right, ins_top, ins_bottom,
                        x2, y2,
                        xscale2, yscale2,
                        ia2,
                        pixels2,
                        w2, h2,
                        xoffset2, yoffset2
                    );

                    if (coll_result) {
                        return inst2;
                    }
                }
                else { //precise vs. precise
                    const bool coll_result = precise_collision_pair(
                        ins_left, ins_right, ins_top, ins_bottom,
                        x, y, x2, y2,
                        xscale1, yscale1, xscale2, yscale2,
                        ia1, ia2,
                        pixels1, pixels2,
                        w1, h1, w2, h2,
                        xoffset1, yoffset1, xoffset2, yoffset2
                    );

                    if (coll_result) {
                        return inst2;
                    }
                }
            }
        }
            
    }
    return NULL;
}

enigma::object_collisions* const collide_inst_rect(int object, bool solid_only, bool prec, bool notme, int x1, int y1, int x2, int y2)
{

    if (x1 > x2 || y1 > y2) {
        return NULL;
    }

    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        enigma::object_collisions* const inst = (enigma::object_collisions*)*it;
        if (notme && inst->id == enigma::instance_event_iterator->inst->id)
            continue;
        if (solid_only && !inst->solid)
            continue;
         if (inst->sprite_index == -1 && inst->mask_index == -1) //no sprite/mask then no collision
            continue;

        const bbox_rect_t &box = inst->$bbox_relative();
        const double x = inst->x, y = inst->y,
                     xscale = inst->image_xscale, yscale = inst->image_yscale,
                     ia = inst->image_angle;
        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x, y, xscale, yscale, ia);

        if (left <= x2 && x1 <= right && top <= y2 && y1 <= bottom) {

            //Only do precise if prec is true AND the sprite is precise.
            if (!prec) {
                return inst;
            }

            const int collsprite_index = inst->mask_index != -1 ? inst->mask_index : inst->sprite_index;

            enigma::sprite* sprite = enigma::spritestructarray[collsprite_index];

            const int usi = ((int) inst->image_index) % sprite->subcount;

            unsigned char* pixels = (unsigned char*) (sprite->colldata[usi]);

            if (pixels == 0) { //bbox.
                return inst;
            }
            else { //precise.
                //Intersection.
                const int ins_left = max(left, x1);
                const int ins_right = min(right, x2);
                const int ins_top = max(top, y1);
                const int ins_bottom = min(bottom, y2);

                //Check per pixel.

                const int w = sprite->width;
                const int h = sprite->height;

                const double xoffset = sprite->xoffset;
                const double yoffset = sprite->yoffset;

                const bool coll_result = precise_collision_single(
                    ins_left, ins_right, ins_top, ins_bottom,
                    x, y,
                    xscale, yscale,
                    ia,
                    pixels,
                    w, h,
                    xoffset, yoffset
                );

                if (coll_result) {
                    return inst;
                }
            }
        }
    }
    return NULL;
}

enigma::object_collisions* const collide_inst_point(int object, bool solid_only, bool prec, bool notme, int x1, int y1)
{
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(object); it; ++it)
    {
        enigma::object_collisions* const inst = (enigma::object_collisions*)*it;
        if (notme && inst->id == enigma::instance_event_iterator->inst->id)
            continue;
        if (solid_only && !inst->solid)
            continue;
        if (inst->sprite_index == -1 && inst->mask_index == -1) //no sprite/mask then no collision
            continue;

        const bbox_rect_t &box = inst->$bbox_relative();
        const double x = inst->x, y = inst->y,
                     xscale = inst->image_xscale, yscale = inst->image_yscale,
                     ia = inst->image_angle;
        int left, top, right, bottom;
        get_border(&left, &right, &top, &bottom, box.left, box.top, box.right, box.bottom, x, y, xscale, yscale, ia);

        if (x1 >= left && x1 <= right && y1 >= top && y1 <= bottom) {

            //Only do precise if prec is true AND the sprite is precise.
            if (!prec) {
                return inst;
            }

            const int collsprite_index = inst->mask_index != -1 ? inst->mask_index : inst->sprite_index;

            enigma::sprite* sprite = enigma::spritestructarray[collsprite_index];

            const int usi = ((int) inst->image_index) % sprite->subcount;

            unsigned char* pixels = (unsigned char*) (sprite->colldata[usi]);

            if (pixels == 0) { //bbox.
                return inst;
            }
            else { //precise.
                //Intersection.
                const int ins_left = max(left, x1);
                const int ins_right = min(right, x1);
                const int ins_top = max(top, y1);
                const int ins_bottom = min(bottom, y1);

                //Check per pixel.

                const int w = sprite->width;
                const int h = sprite->height;

                const double xoffset = sprite->xoffset;
                const double yoffset = sprite->yoffset;

                const bool coll_result = precise_collision_single(
                    ins_left, ins_right, ins_top, ins_bottom,
                    x, y,
                    xscale, yscale,
                    ia,
                    pixels,
                    w, h,
                    xoffset, yoffset
                );

                if (coll_result) {
                    return inst;
                }
            }
        }
    }
    return NULL;
}
