#include <signal.h>
#include "header.h"
#include "parser.h"
#include "variables.h"
#include "execution.h"
#include "cd.h"
#include "sing_handling.h"

//variables globales
char* prompt ="\nmyshell $:"; //mensaje de la shell
int job_count=0;//cantida de procesos en bg
List* Jobs;//lista con los jobs activos
StackInt* job_stack=NULL;//stack para los procesos en el background
char* last_line;//ultima linea insertada
char* color ="\x1b[38;2;0;255;180m";//azul clarito

//metodos auxiliares
void execcmd(parsed_line pl);//ejecutar un comando
void notification(char*,int);//notificar los jobs
void change_prompt();//cambiar aspecto de la prompt
int manage_separator(char* separator,int status);//para los separadores && || ; | 
int sig_sep(parsed_line pl);//ubica la posicion del siguiente separador
void change_color(int status);//cambiar el color en que se imprime el nombre de la shell
 void ver_procesos_background();//verifica que hayan terminado los procesos del background

int main(int argc,char** args)
{

    char wcd[1024];//working directory
     getcwd(wcd,1024);//ruta desde donde se llamo la shell
     set_path(wcd);//guarda el path de history 
     set_help_folder(wcd);//guarda el path para help (para no perder la ruta)

    char* charline_ptr; //para recibir la entrada estandar
    size_t size=0; //se guarda el tamanno de la entrada
    ssize_t read_code;//para ver el codigo de error al leer una linea
    parsed_line pl = new_l();
    Jobs = new_p();
    last_line=get_last_line();//saca la ultima linea del historial
   signal(SIGINT,sign_handler); //para manejar las senales
    while(1){ //loop

      printf("%s%s \x1b[38;2;255;255;255m",color,prompt); //muestra el prompt
      fflush(stdout);//limpia el flujo de salida estandar
    read_code= getline(&charline_ptr,&size,stdin);//recibe el input    
          if (read_code==-1)// si se presiona ctrl D
          {
            free_list(&Jobs);
            free_stack(&job_stack);
            printf("\n exitting\n");
            return -1;
          }
         pl = from(charline_ptr,size);//parsea la linea

         if(pl.commands->data==NULL)
         {
            continue;   
         }
         if(strcmp(pl.commands->data,"again")!=0 ){// si no se inserta again guardar en el historial
         last_line=malloc(sizeof(char)*(size));
         strcpy(last_line,charline_ptr);
          write_history(charline_ptr);
         }
         execcmd(pl);
      ver_procesos_background();
      dest_parser(&pl);
    }
 return 0;   
}

void execcmd(parsed_line pl)
{
    if(pl.pipes->size >= pl.command_count)
    {
      printf("error parsing expression expected\n");
      return;
    }

     int** pipes = malloc(sizeof(int*)*pl.pipes->size);//guardo un array con los pipes
   for(int i =0;i<pl.pipes->size;i++)
   {
      pipes[i]=malloc(sizeof(int)*2);
      pipe(*(pipes+i));//los inicializo
   }

   int in_pipe =-1;//flag para saber si estoy o no en un pipe 0 si , -1 no
   StackInt* pipes_pids=NULL;//stack para los procesos (esperarlos despues de ejecutar los pipes)
   int last_pipe=0;//iterador para el array de pipes

   for(int i=0;i<pl.command_count;i++)
   {
         char* separator=NULL;
         if(pl.separator->data!=NULL){
          separator= malloc(sizeof(*(pl.separator->data)));
         strcpy(separator,pl.separator->data);
         remove_l(pl.separator,pl.separator->data);
         }


      char* command = get_comand(&pl,i);//sacar el comando actual
//comandos comunes (van aparte)
      if(strcmp(command,"exit")==0)
      {
            free_list(&Jobs);
            free_stack(&job_stack);
            exit(EXIT_SUCCESS);
      }
      if(strcmp(command,"get")==0 && at_a(pl.arguments,i)->size>1)
      {
         char* orig = getValue(*(get_args(&pl,i)+1));         
         if(orig!=NULL && *orig=='`')
         {
         char* command = malloc(sizeof(char)*strlen(orig));
         command=strcpy(command,orig);
            *command=' ';
            *(command+strlen(command)-1)='\n';
            *(command+strlen(command)-2)=' ';
            parsed_line pl_i = from(command,1);
            execcmd(pl_i);
         }
      }
      if(strcmp(command,"fg")==0)
      {         
         int status;
         char* arg=*(get_args(&pl,i)+1);

         if(arg==NULL){// si no hay argumento saca el ultimo
         int towait = pop(&job_stack);
         if(towait==-1){continue;}
         waitpid(towait,&status,0);//espera por el proceso
         notification(at(Jobs,job_count-1),status);//notifica a la terminal
         }else{
             int err = remove_el(&job_stack,atoi(arg));
             if(err==-1)
             {
               printf(" el proceso %d no existe\n",atoi(arg));
               change_color(100);
             }else{
               last_pid=atoi(arg);
               waitpid(atoi(arg),&status,0);//espera al proceso
               notification(at(Jobs,err),status);//notifica a la terminal
               remove_l(at_a(pl.arguments,i),arg);//elimina el argumento
             }

         }
      }
      if(strcmp(command,"cd")==0){
         change_dir(*(get_args(&pl,i)+1));         
      }
      if(strcmp(command,"again")==0 )
      {
         if(last_line!=NULL)
         {
            parsed_line pl1 = from(last_line,strlen(last_line));
            execcmd(pl1);
         }else{
            printf("no hay comando guardado\n");
         }
      }
      if(strcmp(command,"Jobs")==0 || strcmp(command,"jobs")==0)
      {
         char** tmp = list_to_arr(Jobs);
         char** pids = stack_to_arr(job_stack);
         for(int i=0;*(tmp+i)!=NULL;i++)
         {
            printf("%i - %s (%s) \n",i+1,*(tmp+i),*(pids+job_count-i-1));
         }
      }



       //ejecucion de los binarios
       //pipes para pasar al executioner
      int* pipe1=NULL;
      int* pipe2=NULL;

      //detectando y asignando pipes
       if(pl.pipes->size>0 && strcmp(command,pl.pipes->data)==0 && strcmp(separator,"|")==0)
       {
           remove_l(pl.pipes,command);
           pipe1=*(pipes+last_pipe);
           if( in_pipe==-1 )
           {
           in_pipe=0;
           }else{
            last_pipe++;
            pipe2=*(pipes+last_pipe);
           }
       }else if(in_pipe==0)
       {
          pipe2=*(pipes+last_pipe);
          in_pipe=-1;
          last_pipe++;
       } 

     //guardando redirecciones
    char** r_inp =redirect_input(&pl,i);
    //ejecutando el binario
    int job_pid =  exec_cmd(at_a(pl.arguments,i)->size,get_args(&pl,i),r_inp,pipe1,pipe2);
    //cerrando los pipes


      if(job_pid<0)//negativo significa q no hay q esperarlo (fg)
      {
         insert(Jobs,command);
         push(&job_stack,(job_pid*-1));
         job_count++;
         change_prompt();      
      }else 
      if (in_pipe==-1) // si no esta dentro de un pipe se espera a los procesos
      {         
         last_pid=job_pid;
         int status;
         int tmp = pop(&pipes_pids);
         while (tmp!=-1)
         {
            waitpid(tmp,&status,0);
            tmp = pop(&pipes_pids);
         }
         waitpid(job_pid,&status,0);
         change_color(status);
         if(separator!=NULL){

         int sep = manage_separator(separator,status);
          if(sep==1)
          {
            i+=1+sig_sep(pl);
          }else if(sep==-1)
          {
            break;
          }

         }

      }else{ //caso contrario se guardan en la lista de llamadas de pipes
         push(&pipes_pids,job_pid);
      }
      free(separator);
   }
   for(int i =0;i<pl.pipes->size;i++)
   {
      free(*(pipes+i));
   }
}

void notification(char* messaje,int status)
{
   job_count--;
   change_prompt();
   printf("finalizado Job {%s} con estatus {%d}\n",messaje,status);
    remove_l(Jobs,messaje);
}

void change_prompt()
{
   if(job_count>=1)
   {
     prompt=malloc(sizeof(char)*(11+job_count));
     *prompt='\n';
     for(int i=1;i<=job_count;i++)
     {
         *(prompt+i)='*';
     }
      color ="\x1b[38;2;0;100;180m";
     prompt=strcat(prompt,"myshell $:");
   }
   else{
      prompt=malloc(sizeof(char)*11);
      color ="\x1b[38;2;0;255;180m";
      prompt="\nmyshell $:";
   }
}

void change_color(int status)
{   
   if(status==0 && job_count==0)
   {
      color ="\x1b[38;2;0;255;180m";
   }else if(status==0)
   {
      color ="\x1b[38;2;0;100;180m";
   }
   else{
      color ="\x1b[38;2;255;0;0m";
   }
}

 int manage_separator(char* separator,int status)//1 romper el bucle 0 continuar 
 {
     if(strcmp(separator,";")==0)
     {
      return 0;
     }

     if(strcmp(separator,"&&")==0)
     {
          return status==0?0:1;
     }

     if(strcmp(separator,"||")==0)
     {
          return status==0?1:0;
     }
     return 0; 
 }

 int sig_sep(parsed_line pl)//auxiliar para determinar el siguiente separador que no sea |
 {
   if(pl.separator->size ==0){return 0;}
   List* aux = pl.separator;
   int cant=0;

   while (aux!=NULL && strcmp(aux->data,"|")==0)
   {
      cant++;
      aux=aux->next;
   }
   
  return cant; 

 }

 void ver_procesos_background()
 {
      for(StackInt* aux = job_stack;aux!=NULL;aux=aux->next){
            int status=-1;
            int pid=aux->data;
             status=waitpid(pid,&status,WNOHANG);
              if(status!=-1){
               int pos = remove_el(&job_stack,pid);             
               notification(at(Jobs,pos),status);//notifica a la terminal
              }
    }
 }