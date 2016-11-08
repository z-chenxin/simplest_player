#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "pjsdl.h"
#include "pjffmpeg.h"
#if 1


static pjsdl *p_pjsdl_st_init(void)
{
	pjsdl *pj = malloc(sizeof(pjsdl));
	if(pj == NULL)
	{
		dbg("failed to malloc st of pjsdl");
		return NULL;
	}
	
	return pj;
}

static void p_pjsdl_st_free(pjsdl **pj)
{
	free(*pj);
	*pj = NULL;
}

static void p_pjsdl_exit(void)
{
	SDL_Quit();
	dbg("exit SDL");
}

pjsdl *p_pjsdl_create_window(int width, int height)
{   
	pjsdl *pj = p_pjsdl_st_init(); 
	if(pj == NULL)
		return pj;
	
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        dbg("failed to init SDL");
        return NULL;
    }

    pj->window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if(pj->window == NULL)
	{
		dbg("failed to create window");
		return NULL;
	}
	
	atexit(p_pjsdl_exit);
	
	pj->renderer = SDL_CreateRenderer(pj->window, -1, 0);
	if(pj->renderer == NULL)
	{
		dbg("failed to create renderer");
		return NULL;
	}
	
	pj->texture = SDL_CreateTexture(pj->renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, width, height);
	if(pj->texture == NULL)
	{
		dbg("failed to create texture");
		return NULL;
	}
	
	return pj;
}

int p_pjsdl_display_video(pjsdl *pj, pjffmpeg *st)
{
	/**
	* 更新YUV纹理->清屏(清渲染器内容？)->渲染纹理->显示渲染器内容
	*/
	pj->rect.x = 0;
	pj->rect.y = 0;
	pj->rect.w = st->pCodecCtx->width;
	pj->rect.h = st->pCodecCtx->height;

	SDL_RenderClear(pj->renderer);
	SDL_UpdateYUVTexture(pj->texture, &pj->rect, st->pAvFrameSDL->data[0],\
                st->pAvFrameSDL->linesize[0], st->pAvFrameSDL->data[1], st->pAvFrameSDL->linesize[1],\
                st->pAvFrameSDL->data[2], st->pAvFrameSDL->linesize[2]);
	SDL_RenderCopy(pj->renderer, pj->texture, NULL, &pj->rect);
	SDL_RenderPresent(pj->renderer);

    return 0;
}
#endif

