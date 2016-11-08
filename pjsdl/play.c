#include "play.h"

int main(int argc, char **argv)
{
    pjffmpeg *ff;

    ff = p_pjffmpeg_init();
    if(ff == NULL)
    {
        dbg("failed to init pjffmpeg");
        return -1;
    }

    if(p_pjffmpeg_get_info(ff, argv[1]) < 0)
    {
        dbg("failed to get info");
        return -1;
    }

    p_pjffmepg_open_decoder(ff);

    p_pjffmepg_decode(ff);
    
    return -1;
}