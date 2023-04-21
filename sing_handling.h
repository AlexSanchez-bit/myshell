#include "header.h"
#ifndef SignHandling_h
#define SignHandling_h

void sign_handler(int sign_num)
{
     signal(SIGINT,SIG_DFL);
}

#endif
