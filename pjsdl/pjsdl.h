#ifndef __PJJSDL_H
#define __PJSDL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "SDL.h"
#include "util.h"
#include "pjffmpeg.h"


typedef struct st_sdl{
	SDL_Window   *window;
	SDL_Renderer *renderer;
	SDL_Texture  *texture;
	int			 width;
	int          height;
	SDL_Rect     rect;
}pjsdl;

pjsdl *p_pjsdl_create_window(int width, int height);
int p_pjsdl_display_video(pjsdl *pj, pjffmpeg *st);
#endif