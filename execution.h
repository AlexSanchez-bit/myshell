#ifndef Exec_h
#define Exec_h
#include "header.h"
#include "redirection.h"
#include "cd.h"
#include "history.h"
#include "help.h"
#include "variables.h"
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
    int var_ex_status =0;
        if(strcmp(command,"set")==0)
        {
           var_ex_status= set_var(args);
        }         
        if(strcmp(command,"get")==0)
        {
                var_ex_status=get_value(*(args+1));
        }         
        if(strcmp(command,"unset")==0)
        {
            if(*(args+1)==NULL)
            {
               var_ex_status= EXIT_FAILURE;
            }else{
            unset_var(*(args+1));
            }
        }         

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
        if(strcmp(command,"set")==0 || strcmp(command,"get")==0 || strcmp(command,"unset")==0)
        {
            exit(var_ex_status);
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

        if(pipe1!=NULL && pipe2==NULL)//si tiene un solo pipe solo conecto la salida
        {
           //primero de la cadena
            close(pipe1[READ_END]);
            dup2(pipe1[WRITE_END],STDOUT_FILENO);
            close(pipe1[WRITE_END]);

        }else if(pipe1!=NULL && pipe2!=NULL)//si tiene 2 es un comando del medio conecto salida y entrada
        {
            //los del medio
                close(pipe1[WRITE_END]); 
                close(pipe2[READ_END]);
                dup2(pipe1[READ_END],STDIN_FILENO);
                close(pipe1[READ_END]);

                dup2(pipe2[WRITE_END],STDOUT_FILENO);
                close(pipe2[WRITE_END]);

        }else if( pipe1==NULL && pipe2!=NULL){//si tiene solo el pipe 2 es el ultimo comando de la cadena conecto la entrada solamente
            //el ultimo del enlace
                close(pipe2[WRITE_END]);
                dup2(pipe2[READ_END],STDIN_FILENO);
                close(pipe2[READ_END]);
        }

                
 
       for(int i=0;*(r_inp+i)!=NULL;i++)
       {
        redirect(*(r_inp+i));
       }
        printf("\n");
                 
                int execution = execvp(command,args);
                printf("despues de la ejecucion");
                if(execution==-1)
                    {
                        fprintf(stderr,"comando no reconocido : %s \n",command);
                        exit(EXIT_FAILURE);     
                    }
                   
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
