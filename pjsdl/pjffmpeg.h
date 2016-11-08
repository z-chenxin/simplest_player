#ifndef __PJFFMPEG_H
#define __PJFFMPEG_H

#include <stdint.h>
#include <libavformat/avformat.h>
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include <libavutil/imgutils.h>
#include "util.h"

typedef struct st_ffmpeg{
    uint8_t           *frame_buf;
    AVFrame           *pAvFrame;
    AVFrame           *pAvFrameSDL;
    AVCodec           *pCodec;
    AVPacket          *pAvPkt;
    AVCodecContext    *pCodecCtx;
    AVFormatContext   *pFormatCtx;
    int               codec_cnt;
    int               get_picture;
    struct SwsContext *pSwsCtx;
}pjffmpeg;

pjffmpeg *p_pjffmpeg_init(void);
int p_pjffmpeg_get_info(pjffmpeg *st, const char* filepath);
int p_pjffmepg_open_decoder(pjffmpeg *st);
int p_pjffmepg_decode(pjffmpeg *st);
#endif