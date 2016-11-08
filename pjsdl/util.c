#include "util.h"


int dbg(const char *str)
{
	#if DEBUG
	char tmp[1024];
	sprintf(tmp, "%s:%s:%d:%s", __FILE__, __func__,__LINE__, str);
	printf("%s\n", tmp);
	#endif
}