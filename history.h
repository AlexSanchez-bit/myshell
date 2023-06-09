#include "header.h"
#ifndef History_h
#define History_h


char* path;
char* get_last_line();

void set_path(char* c_path)
{
char* file_name = "/history.hist";
path=malloc(sizeof(char)*(strlen(file_name)+strlen(c_path)));
path=strcat(path,c_path);
path=strcat(path,file_name);
}

void write_history(char* line)
{
   char* aux = get_last_line();

   if(aux!=NULL && strcmp(line,aux)==0)
   {
     return;
   }

   int file = open(path,O_WRONLY | O_APPEND | O_CREAT);
   if(file==-1)
   {
    printf("error abriendo el archivo\n");
   }
   write(file,line,strlen(line));
   write(file,"\n",1);
   close(file);
}

void read_history()
{
    char* buff = malloc(sizeof(char)*2000);
   int file = open(path,O_RDONLY | O_CREAT);
   read(file,buff,2000);
   char* token = strtok(buff,"\n");
   int i=0;
   while (token!=NULL)
   {
     printf("%d:%s\n",i+1,token);
    token=strtok(NULL,"\n");
    i++;
   }
   
   close(file);

}

char* get_last_line()
{
    char* buff = malloc(sizeof(char)*2000);
   int file = open(path,O_RDONLY | O_CREAT);
   read(file,buff,2000);
   char* token = strtok(buff,"\n");
   char* ant=token;
   int i=0;
   int size=0;
   while (token!=NULL)
   {
      size=strlen(token)+1;
    ant=malloc(sizeof(char)*(size));
    ant=strcat(ant,token);
    token=strtok(NULL,"\n");
    i++;
   }
   close(file);
   if(ant!=NULL)
   {
     *(ant+size-1)='\n'; 
   }
   return ant;
}

#endif