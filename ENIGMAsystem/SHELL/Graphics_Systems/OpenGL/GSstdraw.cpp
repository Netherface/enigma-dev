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

#include <math.h>
#include <GL/gl.h>

#define __GETR(x) (((unsigned int)x & 0x0000FF))
#define __GETG(x) (((unsigned int)x & 0x00FF00) >> 8)
#define __GETB(x) (((unsigned int)x & 0xFF0000) >> 16)
#define __GETRf(x) fmod(x,256)
#define __GETGf(x) fmod(x/256,256)
#define __GETBf(x) fmod(x/65536,256)

namespace enigma{
float circleprecision=24;
extern unsigned cur_bou_tha_noo_sho_eve_cha_eve;
extern float currentcolor[4];
}
#define untexture() if(enigma::cur_bou_tha_noo_sho_eve_cha_eve) glBindTexture(GL_TEXTURE_2D,enigma::cur_bou_tha_noo_sho_eve_cha_eve=0);

int draw_point(float x,float y){
	untexture();
	glBegin(GL_POINTS);
	glVertex2f(x,y);
	glEnd();
	return 0;
}

int draw_point_color(float x,float y,double color){
	untexture();
	glPushAttrib(GL_CURRENT_BIT);
	glColor4f(__GETRf(color)/255.0,__GETGf(color)/255.0,__GETBf(color)/255.0,enigma::currentcolor[3]);
	glBegin(GL_POINTS);
	glVertex2f(x,y);
	glEnd();
	glPopAttrib();
	return 0;
}

int draw_line(float x1,float y1,float x2,float y2){
	untexture();
	glBegin(GL_LINES);
	glVertex2f(x1,y1);
	glVertex2f(x2,y2);
	glEnd();
	return 0;
}

int draw_line_width(float x1,float y1,float x2,float y2,float width){
	untexture();
	glPushAttrib(GL_LINE_BIT);
	glLineWidth(width);
	glBegin(GL_LINES);
	glVertex2f(x1,y1);
	glVertex2f(x2,y2);
	glEnd();
	glPopAttrib();
	return 0;
}

int draw_line_color(float x1,float y1,float x2,float y2,double c1,double c2){
	untexture();
	glBegin(GL_LINES);
	glColor4f(__GETRf(c1)/255.0,__GETGf(c1)/255.0,__GETBf(c1)/255.0,enigma::currentcolor[3]);
	glVertex2f(x1,y1);
	glColor4f(__GETRf(c2)/255.0,__GETGf(c2)/255.0,__GETBf(c2)/255.0,enigma::currentcolor[3]);
	glVertex2f(x2,y2);
	glEnd();
	glColor4fv(enigma::currentcolor);
	return 0;
}

int draw_line_width_color(float x1,float y1,float x2,float y2,float width,double c1,double c2){
	untexture();
	glPushAttrib(GL_LINE_BIT);
	glLineWidth(width);
	glBegin(GL_LINES);
	glColor4f(__GETRf(c1)/255.0,__GETGf(c1)/255.0,__GETBf(c1)/255.0,enigma::currentcolor[3]);
	glVertex2f(x1,y1);
	glColor4f(__GETRf(c2)/255.0,__GETGf(c2)/255.0,__GETBf(c2)/255.0,enigma::currentcolor[3]);
	glVertex2f(x2,y2);
	glEnd();
	glPopAttrib();
	glColor4fv(enigma::currentcolor);
	return 0;
}

int draw_rectangle(float x1,float y1,float x2,float y2,bool outline){
	untexture();
	if(outline){
		glBegin(GL_LINE_LOOP);
		glVertex2f(x1,y1);
		glVertex2f(x1,y2);
		glVertex2f(x2,y2);
		glVertex2f(x2,y1);
		glEnd();
	}else glRectf(x1,y1,x2,y2);
	return 0;
}

int draw_rectangle_angle(float x1,float y1,float x2,float y2,bool outline,float angle){
	untexture();
	angle*=M_PI/180;
	float
		xm=(x2+x1)/2,
		ym=(y2+y1)/2,
		len=hypot(xm-x1,ym-y1),
		dir,ldx1,ldy1,ldx2,ldy2;
	dir=atan2(y1-ym,x1-xm)+angle;
	ldx1=len*cos(dir);
	ldy1=len*sin(dir);
	dir=atan2(y2-ym,x1-xm)+angle;
	ldx2=len*cos(dir);
	ldy2=len*sin(dir);
	glBegin(outline?GL_LINE_LOOP:GL_QUADS);
	glVertex2f(xm+ldx1,ym-ldy1);
	glVertex2f(xm+ldx2,ym-ldy2);
	glVertex2f(xm-ldx1,ym+ldy1);
	glVertex2f(xm-ldx2,ym+ldy2);
	glEnd();
	return 0;
}

int draw_rectangle_color(float x1,float y1,float x2,float y2,double c1,double c2,double c3,double c4,bool outline){
	untexture();
	glBegin(outline?GL_LINE_LOOP:GL_QUADS);
	glColor4f(__GETRf(c1)/255.0,__GETGf(c1)/255.0,__GETBf(c1)/255.0,enigma::currentcolor[3]);
	glVertex2f(x1,y1);
	glColor4f(__GETRf(c2)/255.0,__GETGf(c2)/255.0,__GETBf(c2)/255.0,enigma::currentcolor[3]);
	glVertex2f(x2,y1);
	glColor4f(__GETRf(c4)/255.0,__GETGf(c4)/255.0,__GETBf(c4)/255.0,enigma::currentcolor[3]);
	glVertex2f(x2,y2);
	glColor4f(__GETRf(c3)/255.0,__GETGf(c3)/255.0,__GETBf(c3)/255.0,enigma::currentcolor[3]);
	glVertex2f(x1,y2);
	glEnd();
	glColor4fv(enigma::currentcolor);
	return 0;
}

int draw_set_circle_precision(float pr){
	enigma::circleprecision=pr<3?3:pr;
	return 0;
}
float draw_get_circle_precision(){
	return enigma::circleprecision;
}

int draw_circle(float x,float y,float r,bool outline){
	untexture();
	float pr=2*M_PI/enigma::circleprecision;
	if(outline){
		glBegin(GL_LINES);
		for(float i=pr;i<M_PI/2;i+=pr){
			float xc1=cos(i)*r,yc1=sin(i)*r;
			i+=pr;
			float xc2=cos(i)*r,yc2=sin(i)*r;
			glVertex2f(x+xc1,y+yc1);glVertex2f(x+xc2,y+yc2);
			glVertex2f(x-xc1,y+yc1);glVertex2f(x-xc2,y+yc2);
			glVertex2f(x+xc1,y-yc1);glVertex2f(x+xc2,y-yc2);
			glVertex2f(x-xc1,y-yc1);glVertex2f(x-xc2,y-yc2);
			glVertex2f(x+yc1,y+xc1);glVertex2f(x+yc2,y+xc2);
			glVertex2f(x-yc1,y+xc1);glVertex2f(x-yc2,y+xc2);
			glVertex2f(x+yc1,y-xc1);glVertex2f(x+yc2,y-xc2);
			glVertex2f(x-yc1,y-xc1);glVertex2f(x-yc2,y-xc2);
		}
	}else{
		glBegin(GL_QUADS);
		for(float i=pr;i<M_PI/2;i+=pr){
			float xc1=cos(i)*r,yc1=sin(i)*r;
			i+=pr;
			float xc2=cos(i)*r,yc2=sin(i)*r;
			glVertex2f(x-xc1,y+yc1);glVertex2f(x+xc1,y+yc1);glVertex2f(x+xc2,y+yc2);glVertex2f(x-xc2,y+yc2);
			glVertex2f(x-xc1,y-yc1);glVertex2f(x+xc1,y-yc1);glVertex2f(x+xc2,y-yc2);glVertex2f(x-xc2,y-yc2);
			glVertex2f(x-yc1,y+xc1);glVertex2f(x+yc1,y+xc1);glVertex2f(x+yc2,y+xc2);glVertex2f(x-yc2,y+xc2);
			glVertex2f(x-yc1,y-xc1);glVertex2f(x+yc1,y-xc1);glVertex2f(x+yc2,y-xc2);glVertex2f(x-yc2,y-xc2);
		}
	}
	glEnd();
	return 0;
}

int draw_circle_color(float x,float y,float r,double c1, double c2,bool outline){
    untexture();
	if(outline) glBegin(GL_LINE_STRIP);
	else{
		glBegin(GL_TRIANGLE_FAN);
		glColor4f(__GETRf(c1)/255.0,__GETGf(c1)/255.0,__GETBf(c1)/255.0,enigma::currentcolor[3]);
		glVertex2f(x,y);
	}
	glColor4f(__GETRf(c2)/255.0,__GETGf(c2)/255.0,__GETBf(c2)/255.0,enigma::currentcolor[3]);
	float pr=2*M_PI/enigma::circleprecision;
	glVertex2f(x+r,y);
	for(float i=pr;i<2*M_PI;i+=pr)
		glVertex2f(x+r*cos(i),y-r*sin(i));
	glVertex2f(x+r,y);
	glEnd();
	glColor4fv(enigma::currentcolor);
	return 0;
}

int draw_circle_perfect(float x,float y,float r,bool outline){
	untexture();
	float r2=r*r,r12=r*M_SQRT1_2;
	glBegin(outline?GL_POINTS:GL_LINES);
	for(float xc=0,yc=r;xc<=r12;xc++){
		if(xc*xc+yc*yc>r2) yc--;
		glVertex2f(x+xc,y+yc);
		glVertex2f(x+xc,y-yc);
		glVertex2f(x-xc,y+yc);
		glVertex2f(x-xc,y-yc);
		glVertex2f(x+yc,y+xc);
		glVertex2f(x+yc,y-xc);
		glVertex2f(x-yc,y+xc);
		glVertex2f(x-yc,y-xc);
	}
	glEnd();
    return 0;
}

int draw_circle_color_perfect(float x,float y,float r, double c1, double c2, bool outline){
	untexture();
	float r2=r*r;
	if(outline){
		glBegin(GL_POINTS);
		glColor4f(__GETRf(c2)/255.0,__GETGf(c2)/255.0,__GETBf(c2)/255.0,enigma::currentcolor[3]);
		float r12=r*M_SQRT1_2;
		for(float xc=0,yc=r;xc<=r12;xc++){
			if(xc*xc+yc*yc>r2) yc--;
			glVertex2f(x+xc,y+yc);
			glVertex2f(x+xc,y-yc);
			glVertex2f(x-xc,y+yc);
			glVertex2f(x-xc,y-yc);
			glVertex2f(x+yc,y+xc);
			glVertex2f(x+yc,y-xc);
			glVertex2f(x-yc,y+xc);
			glVertex2f(x-yc,y-xc);
		}
	}else{
		glBegin(GL_TRIANGLE_FAN);
		glColor4f(__GETRf(c1)/255.0,__GETGf(c1)/255.0,__GETBf(c1)/255.0,enigma::currentcolor[3]);
		glVertex2f(x,y);
		glColor4f(__GETRf(c2)/255.0,__GETGf(c2)/255.0,__GETBf(c2)/255.0,enigma::currentcolor[3]);
		glVertex2f(x-r,y);
		for(float xc=-r+1;xc<r;xc++)
			glVertex2f(x+xc,y+sqrt(r2-(xc*xc)));
		for(float xc=r;xc>-r;xc--)
			glVertex2f(x+xc,y-sqrt(r2-(xc*xc)));
		glVertex2f(x-r,y);
	}
	glEnd();
	glColor4fv(enigma::currentcolor);
	return 0;
}

int draw_ellipse(float x1,float y1,float x2,float y2,bool outline){
	untexture();
	float
		x=(x1+x2)/2,y=(y1+y2)/2,
		hr=fabs(x2-x),vr=fabs(y2-y),
		pr=2*M_PI/enigma::circleprecision;
	if(outline){
		glBegin(GL_LINES);
		for(float i=pr;i<M_PI;i+=pr){
			float xc1=cos(i)*hr,yc1=sin(i)*vr;
			i+=pr;
			float xc2=cos(i)*hr,yc2=sin(i)*vr;
			glVertex2f(x+xc1,y+yc1);glVertex2f(x+xc2,y+yc2);
			glVertex2f(x-xc1,y+yc1);glVertex2f(x-xc2,y+yc2);
			glVertex2f(x+xc1,y-yc1);glVertex2f(x+xc2,y-yc2);
			glVertex2f(x-xc1,y-yc1);glVertex2f(x-xc2,y-yc2);
		}
	}else{
		glBegin(GL_QUADS);
		for(float i=pr;i<M_PI;i+=pr){
			float xc1=cos(i)*hr,yc1=sin(i)*vr;
			i+=pr;
			float xc2=cos(i)*hr,yc2=sin(i)*vr;
			glVertex2f(x-xc1,y+yc1);glVertex2f(x+xc1,y+yc1);glVertex2f(x+xc2,y+yc2);glVertex2f(x-xc2,y+yc2);
			glVertex2f(x-xc1,y-yc1);glVertex2f(x+xc1,y-yc1);glVertex2f(x+xc2,y-yc2);glVertex2f(x-xc2,y-yc2);
		}
	}
	glEnd();
	return 0;
}

int draw_ellipse_color(float x1,float y1,float x2,float y2,double c1, double c2,bool outline){
    untexture();
	float
		x=(x1+x2)/2,y=(y1+y2)/2,
		hr=fabs(x2-x),vr=fabs(y2-y),
		pr=2*M_PI/enigma::circleprecision;
	if(outline) glBegin(GL_LINE_STRIP);
	else{
		glBegin(GL_TRIANGLE_FAN);
		glColor4f(__GETRf(c1)/255.0,__GETGf(c1)/255.0,__GETBf(c1)/255.0,enigma::currentcolor[3]);
		glVertex2f(x,y);
	}
    glColor4f(__GETRf(c2)/255.0,__GETGf(c2)/255.0,__GETBf(c2)/255.0,enigma::currentcolor[3]);
    glVertex2f(x+hr,0);
    for(float i=pr;i<2*M_PI;i+=pr)
		glVertex2f(x+hr*cos(i),y+vr*sin(i));
	glVertex2f(x+hr,0);
	glEnd();
	glColor4fv(enigma::currentcolor);
	return 0;
}

int draw_ellipse_perfect(float x1,float y1,float x2,float y2,bool outline){
	untexture();
	float
		x=(x1+x2)/2,y=(y1+y2)/2,
		hr=fabs(x2-x),vr=fabs(y2-y);
	glBegin(outline?GL_POINTS:GL_LINES);
	for(float xc=0;xc<hr;xc++){
		float yc=vr*cos((M_PI/2)/hr*xc);
		glVertex2f(x+xc,y+yc);
		glVertex2f(x+xc,y-yc);
		glVertex2f(x-xc,y+yc);
		glVertex2f(x-xc,y-yc);
	}
	glEnd();
	return 0;
}

int draw_triangle(float x1,float y1,float x2,float y2,float x3,float y3,bool outline){
	untexture();
	glBegin(outline?GL_LINE_LOOP:GL_TRIANGLES);
	glVertex2f(x1,y1);
	glVertex2f(x2,y2);
	glVertex2f(x3,y3);
	glEnd();
	return 0;
}

int draw_triangle_color(float x1,float y1,float x2,float y2,float x3,float y3,double col1,double col2,double col3,bool outline){
	untexture();
	glBegin(outline?GL_LINE_LOOP:GL_TRIANGLES);
	glColor4f(__GETRf(col1)/255.0,__GETGf(col1)/255.0,__GETBf(col1)/255.0,enigma::currentcolor[3]);
	glVertex2f(x1,y1);
	glColor4f(__GETRf(col2)/255.0,__GETGf(col2)/255.0,__GETBf(col2)/255.0,enigma::currentcolor[3]);
	glVertex2f(x2,y2);
	glColor4f(__GETRf(col2)/255.0,__GETGf(col2)/255.0,__GETBf(col2)/255.0,enigma::currentcolor[3]);
	glVertex2f(x3,y3);
	glEnd();
	glColor4fv(enigma::currentcolor);
	return 0;
}

int draw_roundrect(float x1,float y1,float x2,float y2,bool outline){
	untexture();
	if(x1>x2){
		float t=x2;
		x2=x1;
		x1=t;
	}
	if(y1>y2){
		float t=y2;
		y2=y1;
		y1=t;
	}
	float
		r=fmin(x2-x1,y2-y1)/3,
		r2=r*r,r12=r*M_SQRT1_2,
		bx1=x1+r,by1=y1+r,
		bx2=x2-r,by2=y2-r;
	glBegin(GL_LINES);
	if(outline){
		glVertex2f(x1,by1);glVertex2f(x1,by2);
		glVertex2f(x2,by1);glVertex2f(x2,by2);
		glVertex2f(bx1,y1);glVertex2f(bx2,y1);
		glVertex2f(bx1,y2);glVertex2f(bx2,y2);
		glEnd();
		glBegin(GL_POINTS);
		for(float xc=0,yc=r;xc<=r12;xc++){
			if(xc*xc+yc*yc>r2) yc--;
			glVertex2f(bx2+xc,by2+yc);
			glVertex2f(bx2+xc,by1-yc);
			glVertex2f(bx1-xc,by2+yc);
			glVertex2f(bx1-xc,by1-yc);
			glVertex2f(bx2+yc,by2+xc);
			glVertex2f(bx2+yc,by1-xc);
			glVertex2f(bx1-yc,by2+xc);
			glVertex2f(bx1-yc,by1-xc);
		}
		glEnd();
	}else{
		for(float xc=0,yc=r;xc<=r12;xc++){
			if(xc*xc+yc*yc>r2) yc--;
			glVertex2f(bx2+xc,by2+yc);
			glVertex2f(bx2+xc,by1-yc);
			glVertex2f(bx1-xc,by2+yc);
			glVertex2f(bx1-xc,by1-yc);
			glVertex2f(bx2+yc,by2+xc);
			glVertex2f(bx2+yc,by1-xc);
			glVertex2f(bx1-yc,by2+xc);
			glVertex2f(bx1-yc,by1-xc);
		}
		glEnd();
		glRectf(bx1,y1,bx2,y2);
	}
    return 0;
}

int draw_healthbar(float x1,float y1,float x2,float y2,float amount,double backcol,double mincol,double maxcol,int dir,bool showback,bool showborder){
	if(x1>x2){
		float t=x2;
		x2=x1;
		x1=t;
	}
	if(y1>y2){
		float t=y2;
		y2=y1;
		y1=t;
	}
	amount=amount>=100?1:(amount<=0?0:amount/100);
	untexture();
	if(showborder){
		glBegin(GL_LINE_LOOP);
		glVertex2f(x1-1,y1-1);
		glVertex2f(x1-1,y2+1);
		glVertex2f(x2+1,y2+1);
		glVertex2f(x2+1,y1-1);
		glEnd();
	}
	if(showback){
		glColor4f(__GETRf(backcol)/255.0,__GETGf(backcol)/255.0,__GETBf(backcol)/255.0,enigma::currentcolor[3]);
		glRectf(x1,y1,x2,y2);
	}
	switch(dir){
	case 1:x1=x2-(x2-x1)*amount;
	break;case 2:y2=y1+(y2-y1)*amount;
	break;case 3:y1=y2-(y2-y1)*amount;
	default:x2=x1+(x2-x1)*amount;
	}
	float
		R=__GETRf(mincol),
		G=__GETGf(mincol),
		B=__GETBf(mincol);
	glColor4f((R+(__GETRf(maxcol)-R)*amount)/255,(G+(__GETGf(maxcol)-G)*amount)/255,(B+(__GETBf(maxcol)-B)*amount)/255,enigma::currentcolor[3]);
	glRectf(x1,y1,x2,y2);
	glColor4fv(enigma::currentcolor);
	return 0;
}

int draw_getpixel(int x,int y){
#ifdef __BIG_ENDIAN__
	glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,&x);
	return x;
#elif defined __LITTLE_ENDIAN__
	glReadPixels(x,y,1,1,GL_BGR,GL_UNSIGNED_BYTE,&x);
	return x>>8;
#else
	char r,g,b;
	glReadPixels(x,y,1,1,GL_RED,GL_UNSIGNED_BYTE,&r);
	glReadPixels(x,y,1,1,GL_GREEN,GL_UNSIGNED_BYTE,&g);
	glReadPixels(x,y,1,1,GL_BLUE,GL_UNSIGNED_BYTE,&b);
	return r|(g<<8)|(b<<16);
#endif
}

int draw_mandelbrot(int x,int y,float w,double Zx,double Zy,double Zw,unsigned iter){
	int c=0;
	glBegin(GL_POINTS);
	for(int i=y; i<y+w; i++)
		for(int j=x;j<x+w;j++){
			double zx=Zx+(j-x)*(Zw/w),zy=Zy+(i-y)*(Zw/w),cx=zx,cy=zy;
			for(unsigned k=0;k<iter;k++)
				if(zx*zx+zy*zy>=4) goto UNMANLY;
				else{
					zx=zx*zx-zy*zy+cx;
					zy=2*zx*zy+cy;
				}
			glVertex2i(i,j);
			c++;
			UNMANLY:;
		}
	glEnd();
	return c;
}