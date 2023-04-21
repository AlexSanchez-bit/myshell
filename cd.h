#ifndef Cd_h
#define Cd_h
#include "header.h"
int change_dir(char* directory)
{

    if(directory==NULL || strcmp(directory,"~")==0)//no llega directorio
    {
      chdir(getenv("HOME"));
      return 0;
    }
  int err = chdir(directory);
  if(err==-1)
  {
    printf("directorio invalido: %s\n",directory);
    return -1;
  }
  return 0;
}

#endif