#include "header.h"
#ifndef Help_h
#define Help_h

char* help_folder;


void set_help_folder(char* or_f)
{
char* folder_name = "/help";
help_folder=malloc(sizeof(char)*(strlen(folder_name)+strlen(or_f)));
help_folder=strcat(help_folder,or_f);
help_folder=strcat(help_folder,folder_name);
}


void print_help(char* help_arg)
{
  if(help_arg==NULL)
  {
    help_arg="help";
  }

  char* file_toread = malloc(sizeof(char)*(strlen(help_folder)+strlen(help_arg)+1));
  file_toread=strcpy(file_toread,help_folder);
  file_toread=strcat(file_toread,"/");
  file_toread=strcat(file_toread,help_arg);
  file_toread=strcat(file_toread,".hlp");

  int file = open(file_toread,O_RDONLY | O_CREAT,0600);

  if(file==-1)
  {
    printf("el comando %s no existe\n",help_arg);
    exit(EXIT_FAILURE);
    return;
  }

  char buffer[2000];

  read(file,&buffer,2000);
  close(file);
  printf("%s",buffer);

}

#endif