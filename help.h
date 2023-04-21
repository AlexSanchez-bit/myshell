#include "header.h"
#ifndef Help_h
#define Help_h

void print_help(char* help_arg)
{
  if(help_arg==NULL)
  {
    help_arg="help";
  }

  char* direction="./help/";
  char* file_toread = malloc(sizeof(char)*(11+strlen(help_arg)));
  file_toread=strcat(file_toread,direction);
  file_toread=strcat(file_toread,help_arg);
  file_toread=strcat(file_toread,".hlp");

  int file = open(file_toread,O_RDONLY);

  if(file==-1)
  {
    printf("el comando no existe\n");
  }

  char buffer[2000];

  read(file,&buffer,2000);
  close(file);
  printf("%s",buffer);

}

#endif