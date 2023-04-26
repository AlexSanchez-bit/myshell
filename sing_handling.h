#include "header.h"
#ifndef SignHandling_h
#define SignHandling_h

int last_pid;
void sign_handler(int sign_num)
{
     kill(last_pid,0);//mata el ultimo proceso
     signal(SIGINT,SIG_DFL);
}

#endif
