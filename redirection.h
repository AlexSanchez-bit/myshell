#ifndef Redir_h
#define Redir_h

#include "header.h"

void pass_stream(char* new_file);
void pass_to_stream(char* new_file,int red_code);
void append_to_stream(char* new_file,int red_code);
void redirect(char* order);
int es_letra(char a);
void stream_to_stream(int from,int to);
void redirect_to_file(char* order,int to);


int last =-1;
void redirect(char* order)
{


   if(*order == '>' || *order =='<')
   {
     redirect_to_file(order,1); 
   }
   else
      {
         int from = atoi(&*(order));         
         if(es_letra(*(order+strlen(order)-1))==0){//si lo ultimo no es una letra es que sera un numero
            int to = atoi(&*(order+strlen(order)-1));//tomo el numero del final
            if(*(order+1)=='>'){
            stream_to_stream(from,to);
            }
            else{
            stream_to_stream(to,from);
            }
            return;
         }else{
         // si no es un numero entonces es el archivo
         char* new_order = (order+1);
         redirect_to_file(new_order,from);
         }
      }
   }

   void redirect_to_file(char* order,int to)
   {

      int des=0;
      char* filename = order;
      while (*filename=='>' || * filename=='<')
      {
         filename+=1;
         des++;
      }

      char* new_filename = malloc(sizeof(char)*(strlen(order)-des));
      new_filename=strcpy(new_filename,filename);

      if(*order=='>')
      {
         if(des>=2)
         {
            append_to_stream(new_filename,to);
            return;
         }         
         pass_to_stream(new_filename,to);
      }
      else{
         pass_stream(new_filename);
         return;
      }
   }

   int es_letra(char a)
   {
      if(a=='1' || a=='2' || a=='0')
      {
         return 0;
      }
      return -1;
   }


   void stream_to_stream(int from,int to)
   {
      int err = dup2(to,from);//hago el cambio de la entrada/salida/error por el archivo      
      if(err==-1)
      {
         printf("hubo un error\n");
      }
   }


 void append_to_stream(char* new_file,int red_code)
 {
   int file=open(new_file,O_APPEND | O_WRONLY | O_CREAT); //abro el archivo en modo lectura 
   if(file == -1)
   {
      fprintf(stderr,"archivo %s no existe\n",new_file);
      exit(EXIT_FAILURE);
   }
   dup2(file,red_code);//hago el cambio de la entrada/salida/error por el archivo
   last=file;
   close(file);
 }


void pass_stream(char* new_file)//cambia el archivo por el stdio
{
   int file=open(new_file,O_RDONLY);
   if(file == -1)
   {
    fprintf(stderr,"no se encontro el archivo: %s\n",new_file);
    exit(EXIT_FAILURE);
   }
   int err=dup2(file,0);
   if(err==-1)
   {
      printf("hubo un error\n");
   }
   close(file);
}

void pass_to_stream(char* new_file,int red_code)//cambia el stdio por un archivo
{
   int file=open(new_file,O_WRONLY | O_TRUNC | O_CREAT); //abro el archivo en modo lectura 
   if(file == -1)
   {
    fprintf(stderr,"no se encontro el archivo: %s\n",new_file);
    exit(EXIT_FAILURE);
   }
   int err=dup2(file,red_code);//hago el cambio de la entrada/salida/error por el archivo
   if(err==-1)
   {
      printf("hubo un error redireccionando \n");
   }
   last=file;
   close(file);
}


#endif