/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include <ma.h>
#include "allegro.h"

void rotate_palette();

//PALETTE pal;

//#define GREEN   1
//#define BLUE    2
//#define RED     3
//#define YELLOW  4
#define GREEN 0x00ff00
#define BLUE 0x0000ff
#define RED 0xff0000
#define YELLOW 0xffff00

/********************************************** POLYGON MODELS *****/

//#define NUM_FACES       6
//#define NUM_VERTICES    11
#define NUM_VERTICES 44
#define NUM_FACES 40

#define NVPF 4	//6

struct Vertex {
	vec3 v;
	int color;
};

Vertex Vertices[NUM_VERTICES];
vec3 verts_out[NUM_VERTICES];
int faces[NUM_FACES*NVPF];
//int colors[NUM_FACES];
MATRIX_f matrix, cam;
unsigned char rotation;
bool gRotation = true;
char yRotation = -16;

void init_model();
void draw_model();
void rotate_matrix();
int qsort_helper(const int *e1, const int *e2);

/******************************************** GLOBAL VARIABLES *****/

int SCREEN_W, SCREEN_H;

/******************************************* MAIN PROGRAM CODE *****/

int qsort_helper(const void *ve1, const void *ve2)
{
	const int* e1 = (const int*)ve1;
	const int* e2 = (const int*)ve2;
	return -fixToInt((verts_out[e1[0]].z+verts_out[e1[1]].z+verts_out[e1[2]].z+verts_out[e1[3]].z) -
		(verts_out[e2[0]].z+verts_out[e2[1]].z+verts_out[e2[2]].z+verts_out[e2[3]].z));
}

//takes 3 points
bool isClockwise(const MAPoint2d* p) {
	int highest = 0;
	for(int i=1; i<3; i++) {
		if(p[highest].y < p[i].y)	//arbitrary
			highest = i;
	}
	int low[2];
	int j=0;
	for(int i=0; i<3; i++) {
		if(i != highest) {
			low[j++] = i;
		}
	}
	ASSERT(j == 2);
	return p[low[0]].x < p[low[1]].x;	//arbitrary
}

void draw_model (void)
{
	int i;
	MATRIX_f out;
	int xy_out[NVPF*2];

	clearScreen();
	matrix_mul_f(&matrix, &cam, &out);

	for(i=0; i<NUM_VERTICES; ++i)
	{
		apply_matrix_f(&out, &Vertices[i].v, &verts_out[i]);
		persp_project_f(&verts_out[i]);
	}
	qsort(faces, NUM_FACES, sizeof(int)*NVPF, qsort_helper);

	for(i=0; i<NUM_FACES; ++i)
	{
		for(int j=0; j<NVPF; j++) {
			xy_out[j*2+0] = fixToInt(verts_out[ faces[i*NVPF+j] ].x);
			xy_out[j*2+1] = fixToInt(verts_out[ faces[i*NVPF+j] ].y);
		}
		//if(isClockwise((const MAPoint2d*) xy_out))
		{
			polygon(NVPF, xy_out, Vertices[faces[i*NVPF]].color);
		}
	}
}

void rotate_matrix (void)
{
	if(gRotation)
		++rotation;
	//get_y_rotate_matrix_f(&matrix, rotation);
	get_transformation_matrix_f(&matrix, 1, yRotation, rotation, 0, 0, 0, 0);
}

extern "C" int MAMain()
{
	/*int vsync_on;

	allegro_init ();
	install_keyboard ();
	install_timer ();

	if (argc > 1)
	{
	if (set_gfx_mode (GFX_VGA, 320, 200, 0, 0) != 0)
	{
	printf ("error setting VGA 320x200x256\n");
	return 1;
	}
	}
	else 
	{
	if (set_gfx_mode (GFX_AUTODETECT, 640, 480, 0, 0) != 0)
	{
	printf ("error setting SVGA 640x480x256\nTry giving a parameter to set 320x200x256 mode\n");
	return 1;
	}
	}

	generate_332_palette(pal);

	pal[0].r=pal[0].g=pal[0].b=32; //Background:Grey
	pal[GREEN].r=pal[GREEN].b=0;
	pal[GREEN].g=63; //Green
	pal[BLUE].r=pal[BLUE].g=0;
	pal[BLUE].b=63; //Blue
	pal[RED].b=pal[RED].g=0;
	pal[RED].r=63; //Red
	pal[YELLOW].r=pal[YELLOW].g=60;
	pal[YELLOW].b=0; //75% Yellow

	set_palette (pal);

	buffer = create_bitmap (SCREEN_W, SCREEN_H);*/
	Extent scrSize = maGetScrSize();
	SCREEN_W = EXTENT_X(scrSize);
	SCREEN_H = EXTENT_Y(scrSize);

	set_projection_viewport (0, 0, SCREEN_W, SCREEN_H);
	init_model();
	//get_camera_matrix_f(&cam, 1.5, 1.5, 12, 0, 0, -1, 0, 1, 0, 32, 1);
  get_camera_matrix_f(&cam, 0, 0, 10, 0, 0, -1, 0, 1, 0, 32, 1);

	for (;;)
	{
		rotate_matrix();
		draw_model();
		const MAPoint2d points[3] = { {0,0}, {3,0}, {0,3} };
		maSetColor(0xffffffff);
		maFillPolygon(points, 3);
		maUpdateScreen();
		{
			EVENT event;
			if(maGetEvent(&event)) {
				if(event.type == EVENT_TYPE_CLOSE)
					return 0;
				else if(event.type == EVENT_TYPE_KEY_PRESSED) {
					switch(event.key) {
					case MAK_0:
						return 0;
					case MAK_FIRE:
						gRotation = !gRotation;
					}
				}
			}
			if((maGetKeys() & MAKB_UP) && yRotation > -64) {
				yRotation--;
			}
			if((maGetKeys() & MAKB_DOWN) && yRotation < 64) {
				yRotation++;
			}
		}
		//if (key[KEY_C])
		//rotate_palette(1);
	}

	return 0;
}

/*void rotate_palette(int step)
{
RGB temp;
int i;

temp = pal[0];
for(i=0; i<255; i+=step)
{
pal[i]=pal[i+1];
}
pal[255]=temp;
set_palette(pal);
}*/

/*void init_model (void)
{
	int i;

	model[0][0]=0;
	model[0][1]=1;
	model[0][2]=3;
	model[0][3]=2;
	colors[0]=GREEN;
	model[1][0]=4;
	model[1][1]=5;
	model[1][2]=1;
	model[1][3]=0;
	colors[1]=YELLOW;
	model[2][0]=6;
	model[2][1]=7;
	model[2][2]=3;
	model[2][3]=2;
	colors[2]=YELLOW;
	model[3][0]=4;
	model[3][1]=0;
	model[3][2]=2;
	model[3][3]=6;
	colors[3]=RED;
	model[4][0]=1;
	model[4][1]=5;
	model[4][2]=7;
	model[4][3]=3;
	colors[4]=BLUE;
	model[5][0]=4;
	model[5][1]=5;
	model[5][2]=7;
	model[5][3]=6;
	colors[5]=GREEN;

	vertices[0].x=0;
	vertices[0].y=3;
	vertices[0].z=0;
	vertices[1].x=1;
	vertices[1].y=3;
	vertices[1].z=0;
	vertices[2].x=0;
	vertices[2].y=0;
	vertices[2].z=0;
	vertices[3].x=1;
	vertices[3].y=0;
	vertices[3].z=0;
	vertices[4].x=0;
	vertices[4].y=3;
	vertices[4].z=1;
	vertices[5].x=1;
	vertices[5].y=3;
	vertices[5].z=1;
	vertices[6].x=0;
	vertices[6].y=0;
	vertices[6].z=1;
	vertices[7].x=1;
	vertices[7].y=0;
	vertices[7].z=1;
	vertices[8].x=1;
	vertices[8].y=1.5;
	vertices[8].z=0;
	vertices[9].x=1;
	vertices[9].y=1.5;
	vertices[9].z=1;
	vertices[10].x=0;
	vertices[10].y=1.5;
	vertices[10].z=1;

	get_translation_matrix_f(&matrix, -1.5, -1.5, -1.5);
	for(i=0; i<NUM_VERTICES; ++i)
	{
		apply_matrix_f(&matrix, &vertices[i], &vertices[i]);
	}

	get_x_rotate_matrix_f(&matrix, 128);
	for(i=0; i<NUM_VERTICES; ++i)
	{
		apply_matrix_f(&matrix, &vertices[i], &vertices[i]);
	}
}*/

void init_model()
{
#define VERTICES(a,b,c,d) Vertices[a].v.x=d; Vertices[a].v.y=c; Vertices[a].v.z=b;
#define FACES(a,b,c,d,e) faces[a*NVPF]=b; faces[a*NVPF+1]=c; faces[a*NVPF+2]=e; faces[a*NVPF+3]=d;

  MATRIX_f matrix;
  int i;

  VERTICES(0,0,0,0);
  VERTICES(1,1,0,0);
  VERTICES(2,1,3,0);
  VERTICES(3,0,3,0);
  VERTICES(4,0,3,1);
  VERTICES(5,1,3,1);
  VERTICES(6,0,0,1);
  VERTICES(7,1,0,1);
  VERTICES(8,0,1.5,1);
  VERTICES(9,1,1.5,1);
  VERTICES(10,1,1.5,0);

  FACES(0,0,3,2,1);
  FACES(1,3,0,6,4);
  FACES(2,5,4,8,9);
  FACES(3,1,10,9,7);
  FACES(4,4,5,2,3);
  FACES(5,6,0,1,7);
  FACES(6,9,8,13,16);//Övre
  FACES(7,7,9,16,20);//Inre
  FACES(8,13,8,6,21);//Yttre
  FACES(9,21,6,7,20);//Undre

  for(i=0; i<10; i++)
    for(int j=0; j<NVPF; j++)
    {
      faces[(i+10)*NVPF+j] = faces[i*NVPF+j]+11;
      faces[(i+20)*NVPF+j] = faces[i*NVPF+j]+22;
      faces[(i+30)*NVPF+j] = faces[i*NVPF+j]+33;
    }
  FACES(36,42,41,2,5);//Övre
  FACES(37,40,42,5,9);//Inre
  FACES(38,2,41,39,10);//Yttre
  FACES(39,10,39,40,9);//Undre

  Vertices[0].color=0xff00ff00;//N
  Vertices[1].color=0xff0000ff;
  Vertices[2].color=0xff00ff00;//N
  Vertices[3].color=0xff0000ff;//N
  Vertices[4].color=0xfffff00f;
  Vertices[5].color=0xff00ff00;
  Vertices[6].color=0xfffff00f;
  Vertices[7].color=0xff0000ff;
  Vertices[9].color=0xffff0000;
  Vertices[10].color=0xff0000ff;

  Vertices[11].color=0xff0000ff;
  Vertices[12].color=0xff00ff00;
  Vertices[13].color=0xff0000ff;
  Vertices[14].color=0xff00ff00;
  Vertices[15].color=0xfffff00f;
  Vertices[16].color=0xff0000ff;
  Vertices[17].color=0xfffff00f;
  Vertices[18].color=0xff00ff00;
  Vertices[20].color=0xff0000ff;
  Vertices[21].color=0xffff0000;

  get_transformation_matrix_f(&matrix, 1, 0, 64, 0, 3, 0, 0);
  for(i=0; i<11; i++)
  {
		apply_matrix_f(&matrix, &Vertices[i].v, &Vertices[i+11].v);
//    Vertices[i+11].color = Vertices[i].color;
  }
  get_transformation_matrix_f(&matrix, 1, 0, 128, 0, 3, 0, 3);
  for(i=0; i<11; i++)
  {
    apply_matrix_f(&matrix, &Vertices[i].v, &Vertices[i+22].v);
    Vertices[i+22].color = Vertices[i].color;
  }
  get_transformation_matrix_f(&matrix, 1, 0, 192, 0, 0, 0, 3);
  for(i=0; i<11; i++)
  {
		apply_matrix_f(&matrix, &Vertices[i].v, &Vertices[i+33].v);
    Vertices[i+33].color = Vertices[i+11].color;
  }

  get_translation_matrix_f(&matrix, -1.5, -1.5, -1.5);
  for(i=0; i<NUM_VERTICES; i++)
    apply_matrix_f(&matrix, &Vertices[i].v, &Vertices[i].v);

#undef VERTICES
#undef FACES
}
