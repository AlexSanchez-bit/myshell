#ifndef Parser_h
#define Parser_h
#include "header.h"

bool contains_char(char* str,char let);
char** tokenize( char* origin_s,int origin_size,const char* del,int* num_t);
//declaration block
 struct parsed_line{
    List* commands;
    AList* arguments;
    AList* redirect;
    List* pipes;
    List* separator;
    int command_count;    
}; 

typedef struct parsed_line parsed_line;

//impl block

//constructor
parsed_line new_l()
{
    return (parsed_line){NULL,NULL,NULL,NULL,NULL,0};
}

parsed_line from(char* origin_l,int lines)
{
    int cant_t =0;
     char** tokens = tokenize(origin_l,lines," ",&cant_t);//despues de esto original mantiene la linea
     List* comandos=new_p();
     AList* args_list = new_p_a();
     AList* r_inp = new_p_a();
     List* pipes = new_p();
     List* separator = new_p();
     bool args=false;
     int command_count=0;

    for(int i=0;i< cant_t &&*(tokens+i)!=NULL;i++)
    {        
        char* token =malloc(sizeof(char)*strlen(*(tokens+i)));
        strcpy(token,*(tokens+i));//hago una copia para poder borrar tokens (borrowing)          

        if(strcmp(token,"#")==0 || *(token)=='#')
        {
          break;
        }

        if(strcmp(token,"|")==0)
        {
            insert(pipes,at(comandos,command_count-1));            
            insert(separator,token);            
            args=false;
            continue;
        }

        if((contains_char(token,'<') || contains_char(token,'>')) && args)
        {
            i+=1;
            char* tmp;
            char * file_name =*(tokens+i);
            if(file_name!=NULL){            
            tmp = malloc(sizeof(char)*(strlen(token)+strlen(file_name)));
            tmp = strcat(token,file_name);
            }
            else{
                tmp = malloc(sizeof(char)*strlen(token));
                strcpy(tmp,token);
            }
            List* aux = at_a(r_inp,command_count-1);
            insert(aux,tmp);            
            continue;
        }
        if(strcmp(token,"&&")==0 ||strcmp(token,"||")==0 ||strcmp(token,";")==0 )
        {
            args=false;
            insert(separator,token);            
            continue;
        }

        if(!args){
        insert(comandos,token);//inserto un comando nuevo
        List* tmp = new_p(); //lista temporal para los argumentos
        insert(tmp,token);        
        insert_a(args_list,tmp);//el primero siempre es el propio comando
        insert_a(r_inp,new_p());
        command_count++; //aumento en 1 la cantidad de comandos
        args=true;//a partir de ahora los tokens son argumentos
        continue;
        }


        insert(at_a(args_list,command_count-1),token);//guardo la lista de argumentos para cada comando
        if(strcmp(token,"&")==0){args=false;}
    }

    return (parsed_line){comandos,args_list,r_inp,pipes,separator,command_count};
}

//setters

//getters

char* get_comand(parsed_line* pl,int i)
{
    return at(pl->commands,i);    
}

char** get_args(parsed_line* pl,int command)
{
return  list_to_arr(at_a(pl->arguments,command));
}

char** redirect_input(parsed_line* pl,int i)
{
    return list_to_arr(at_a(pl->redirect,i));
}


char** tokenize( char* origin_s,int origin_size,const char* del,int* num_t)
{

    // *(origin_s+strlen(origin_s)-1)=' ';
    char* cpy1=malloc(sizeof(char)*origin_size);
    char* cpy2=malloc(sizeof(char)*origin_size);
    strcpy(cpy1,origin_s);
    strcpy(cpy2,origin_s);
     *(cpy1+strlen(origin_s)-1)=' ';
     *(cpy2+strlen(origin_s)-1)=' ';

    char* token = strtok(cpy1,del);
    while(token!=NULL)
    {
       *(num_t)=*(num_t)+1;
    token=strtok(NULL,del);
    }
    char** args = malloc(sizeof(char*)*(*(num_t)+1));
    token=strtok(cpy2,del);
     for(int i=0;token!=NULL;i++)
     {
        *(args+i)=malloc(sizeof(char)*strlen(token));
        strcpy(*(args+i),token);
        token=strtok(NULL,del);
     }
     *(args+*num_t)=NULL;

     free(cpy1);
     free(cpy2);
    return args;
}

bool contains_char(char* str,char let)
{
    for (int i=0;i< strlen(str);i++)
    {
        if(*(str+i)==let)
        {return true;}
    }
    return false;
}

#endif