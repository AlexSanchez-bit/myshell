#ifndef Exec_h
#define Exec_h
#include "header.h"
#include "redirection.h"
#include "cd.h"
#include "history.h"
#include "help.h"
#define READ_END 0
#define WRITE_END 1

char* concat(char* str1,char* str2);

typedef void (*send)(char*);


int exec_cmd(int argc,char** args,char** r_inp,int* pipe1,int* pipe2)
{

    if(argc==0)
    {
        printf("no command :'( \n");
        exit(EXIT_FAILURE);
    }
    int wait=-1;
     if(argc>=2){
        wait = strcmp(*(args+argc-1),"&");
         if(wait==0){
            *(args+argc-1)=NULL;        
         }
     }

    char * command = *(args);

    pid_t pid;
     pid = fork();

    if(pid==-1)
    {
        printf("system error fork not allowed");
        exit(EXIT_FAILURE);
    }
    else if(pid==0) //desde el proceso hijo
    {

        if(strcmp(command,"true")==0)
        {
            exit(EXIT_SUCCESS);     
        }         
        if(strcmp(command,"false")==0)
        {
            exit(EXIT_FAILURE);     
        }         
      if(strcmp(command,"help")==0)
      {
         print_help(*(args+1));
            exit(EXIT_SUCCESS);     
      }
      if(strcmp(command,"history")==0)
      {
            read_history();
            exit(EXIT_SUCCESS);     
      }
      if(strcmp(command,"cd")==0)
      {
            exit(EXIT_SUCCESS);     
      }
      if(strcmp(command,"again")==0)
      {
            exit(EXIT_SUCCESS);     
      }
      if(strcmp(command,"Jobs")==0 || strcmp(command,"jobs")==0)
      {
            exit(EXIT_SUCCESS);     
      }
      if(strcmp(command,"fg")==0)
      {
              if(*(args+1)==NULL)//si llega con null es pq se saco el job
              {
            exit(EXIT_SUCCESS);     
              }else{

            exit(EXIT_FAILURE);     
              }
      }

        if(pipe1!=NULL && pipe2==NULL)
        {
           //primero de la cadena
            close(pipe1[READ_END]);
            dup2(pipe1[WRITE_END],STDOUT_FILENO);
            close(pipe1[WRITE_END]);

        }else if(pipe1!=NULL && pipe2!=NULL)
        {
            //los del medio
                close(pipe2[READ_END]);
                dup2(pipe1[READ_END],STDIN_FILENO);
                close(pipe1[READ_END]);

                dup2(pipe2[WRITE_END],STDOUT_FILENO);
                close(pipe2[WRITE_END]);

        }else if( pipe1==NULL && pipe2!=NULL){
            //el ultimo del enlace
                dup2(pipe2[READ_END],STDIN_FILENO);
                close(pipe2[READ_END]);
        }

             printf(" \n");

       for(int i=0;*(r_inp+i)!=NULL;i++)
       {
        redirect(*(r_inp+i));
       }
         

    char* const_head = (*(command)!='.' && *(command)!='/')?"/bin/":"";
    char* call_status = concat(const_head,command);
                int execution = execv(call_status,args);
                if(execution==-1)
                    {
                        fprintf(stderr,"comando no reconocido : %s \n",command);
                        exit(EXIT_FAILURE);     
                    }
    exit(EXIT_SUCCESS);     
    }
    else
    {
        if(wait==0){
            return pid*-1;
        }
        if(pipe1!=NULL && pipe2==NULL)
        {
         close(pipe1[WRITE_END]);
        }else if(pipe1!=NULL && pipe2!=NULL)
        {
         close(pipe1[READ_END]);
         close(pipe2[WRITE_END]);
        }else if( pipe1==NULL && pipe2!=NULL){
         close(pipe2[READ_END]);
        }

        return pid;
    }
}


char* concat(char* str1,char* str2)
{
    int size1,size2=0;

    for( ;*(str1+size1)!='\0';size1++){}
    for( ;*(str2+size2)!='\0';size2++){}

    char* ret = malloc(sizeof(char)*(size1+size2));
    strcpy(ret,str1);
    strcat(ret,str2);
     return ret;
}

#endif