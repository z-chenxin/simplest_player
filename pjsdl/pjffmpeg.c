#include "pjffmpeg.h"
#include "pjsdl.h"
#include "SDL.h"

pjffmpeg *p_pjffmpeg_init(void)
{
    pjffmpeg *pj;
    av_register_all();
    avformat_network_init();

    pj = av_malloc(sizeof(pjffmpeg));
    if(pj == NULL)
    {
        dbg("faild to init ffmpeg");
        return NULL;
    }
    
    pj->pFormatCtx = avformat_alloc_context();
    if(pj->pFormatCtx == NULL)
    {
        dbg("failed to malloc avformat context");
        //av_free(pj);
        return NULL;
    }
    
    return pj;
}

int p_pjffmpeg_get_info(pjffmpeg *st, const char* filepath)
{
    int index, ret;
    if(avformat_open_input(&st->pFormatCtx, filepath, NULL, NULL) != 0)
    {
        dbg("failed to open input stream");
        return -1;
    }

    if(avformat_find_stream_info(st->pFormatCtx, NULL) < 0)
    {
        dbg("faield to get stream info");
        return -1;
    }

    ret = -1;
    for(index = 0; index < st->pFormatCtx->nb_streams; index++)
    {
        if(st->pFormatCtx->streams[index]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            ret = index;
            break;
        }
    }
    if(ret == -1)
    {
        dbg("failed to find a video stream");
        return ret;
    }

    st->codec_cnt = ret;

    return ret;
}


int p_pjffmepg_open_decoder(pjffmpeg *st)
{
    st->pCodecCtx = st->pFormatCtx->streams[st->codec_cnt]->codec;
    
    st->pCodec    = avcodec_find_decoder(st->pCodecCtx->codec_id);
    if(st->pCodec == NULL)
    {
        dbg("failed to find decoder");
        return -1;
    }

    if(avcodec_open2(st->pCodecCtx, st->pCodec, NULL) < 0)
    {
        dbg("failed to open decoder");
        return -1;
    }

    return 0;
}

int p_pjffmepg_decode(pjffmpeg *st)
{
    pjsdl *sdl = p_pjsdl_create_window(st->pCodecCtx->width, st->pCodecCtx->height);
    if(sdl == NULL)
    {
        dbg("failed to create window of SDL");
        return -1;
    }

    st->pAvPkt = av_packet_alloc();
    if(st->pAvPkt == NULL)
    {
        dbg("failed to malloc avpacket");
        return -1;
    }

    av_init_packet(st->pAvPkt);

    st->pAvFrame    = av_frame_alloc();
    st->pAvFrameSDL = av_frame_alloc();
    if(st->pAvFrame == NULL || st->pAvFrameSDL == NULL)
    {
        dbg("faield to malloc avframe");
        return -1;
    } 

    st->frame_buf = av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, \
    st->pCodecCtx->width, st->pCodecCtx->height, 1));
    if(st->frame_buf == NULL)
    {
        dbg("failed to malloc frame_buf");
        return -1;
    }

    av_image_fill_arrays(st->pAvFrameSDL->data, st->pAvFrameSDL->linesize, \
    st->frame_buf, AV_PIX_FMT_YUV420P, st->pCodecCtx->width, st->pCodecCtx->height, 1);

    st->pSwsCtx = sws_getContext(st->pCodecCtx->width, st->pCodecCtx->height,\
     st->pCodecCtx->pix_fmt, st->pCodecCtx->width, st->pCodecCtx->height, \
     AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

    while(av_read_frame(st->pFormatCtx, st->pAvPkt) >= 0)
    {
        if(st->pAvPkt->stream_index == st->codec_cnt)
        {
            if(avcodec_decode_video2(st->pCodecCtx, st->pAvFrame, \
            &st->get_picture, st->pAvPkt) < 0)
            {
                dbg("failed to decode a frame");
                return -1;
            }

            if(st->get_picture)
            {
                sws_scale(st->pSwsCtx, st->pAvFrame->data, st->pAvFrame->linesize, \
                0, st->pCodecCtx->height, st->pAvFrameSDL->data, st->pAvFrameSDL->linesize);

                p_pjsdl_display_video(sdl, st);
                SDL_Delay(40);
            }
        }
    }

    SDL_Quit();

    return 0;
}



