#ifndef List_h
#define List_h
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct List{
   char* data;
   struct List* next;
   int size;
};

typedef struct List List;
//impl 

List new_(char* element)
{
  return (List){element,NULL,0};
}

List* new_p()
{
     List tmp = new_(NULL);
     List* comandos=malloc(sizeof(List));
     *comandos=tmp;
     return comandos;
}

void insert(List* list,char* element)
{
     if(list->data==NULL)
     {
        list->data=malloc(sizeof(element));
        strcpy(list->data,element);
        list->size++;
        return;
     }

     List* aux=list;
     while(aux->next!=NULL)
     {
        aux=aux->next;
     }   
     aux->next=new_p();
     list->size++;
     insert(aux->next,element);
}

char* at(List* list,int index)
{
     List aux=*(list);
     int i=0;
     for(;i<index && aux.next!=NULL;i++)
     {
        aux=*(aux.next);
     }

     if(i<index)
     {
        return NULL;
     }

     return aux.data;
}

void remove_l(List* list,char* element)
{
     if(list->data==NULL)
     {
      return;
     }  
   if(strcmp(list->data,element)==0)
   {
      if(list->next==NULL)
      {
         free(list->data);
         list->data=NULL;
      }else{ 
          int size = list->size;
         *list=*(list->next);
         list->size=size;

      }      
      list->size--;
      return;
   }

   List* aux = list;
   List* ant =list;
   while (strcmp(aux->data,element)!=0 || aux->next!=NULL)
   {
      ant=aux;
      aux=aux->next;
   }

   if(strcmp(aux->data,element)==0)
   {
      list->size--;
      ant->next=aux->next;
      aux->next=NULL;
      free(aux);
   }
   
}

char** list_to_arr(List* list)
{
   char** ret = malloc(sizeof(char*)*(list->size+1));
   for(int i=0;i<list->size;i++)
   {
      char* tmp = at(list,i);
      *(ret+i)=malloc(sizeof(char)*strlen(tmp));
      strcpy(*(ret+i),tmp);
   }
   *(ret+list->size)=NULL;
   return ret;
}


//lista e listas
struct AList
{
   List* data;
  struct AList* next;
  int size;
};
typedef struct AList AList;
//impl 

AList new_a(List* element)
{
  return (AList){element,NULL,0};
}

AList* new_p_a()
{
     AList tmp = new_a(NULL);
     AList* comandos=malloc(sizeof(tmp));
     *comandos=tmp;
     return comandos;
}

void insert_a(AList* list,List* element)
{
     if(list->data==NULL)
     {
        list->data=malloc(sizeof(element));
        *(list->data)=*(element);
        list->size++;
        return;
     }

     AList* aux=list;
     while(aux->next!=NULL)
     {
        aux=aux->next;
     }   
     aux->next=new_p_a();
     insert_a(aux->next,element);
        list->size++;
}

List* at_a(AList* list,int index)
{
     AList aux=*(list);
     int i=0;
     for(;i<index && aux.next!=NULL;i++)
     {
        aux=*(aux.next);
     }

     if(i<index)
     {
        return NULL;
     }

     return aux.data;
}

char** get_arr(AList* list,int i)
{
   return list_to_arr(at_a(list,i));
}


typedef struct stackint
{
   int data;
   struct stackint* next;
} StackInt;

StackInt* _new_S(int dat)
{
   StackInt new_ = (StackInt){dat,NULL};
   StackInt* tmp = malloc(sizeof(new_));
    *tmp = new_;
   return tmp;
}

void push(StackInt** stck,int data)
{
   StackInt* top = _new_S(data);
   if(*stck==NULL)
   {
      *stck=top;
   }
   else{
      (top->next)=*stck;
      *stck=top;
   }
}

int pop(StackInt** stck)
{
   if(*stck==NULL)
   {
      return -1;
   }
   int ret = (*stck)->data;
   *stck=(*stck)->next;
   return ret;
}

int remove_el(StackInt** list,int element)
{
     if((*list)==NULL)
     {
      return -1;
     }  
   if((*list)->data==element)
   {
      if((*list)->next==NULL)
      {
         *list=NULL;
      }else{ 
          *list=(*list)->next;          
      }      
      return 0;
   }

   StackInt* aux = *list;
   StackInt* ant =*list;
   int i=0;
   while (aux->data!=element && aux->next!=NULL)
   {
      ant=aux;
      aux=aux->next;
      i++;
   }

   if(aux->data==element)
   {
      ant->next=aux->next;
      aux->next=NULL;
      free(aux);
      return i;
   }
   return -1; 
}

char** stack_to_arr(StackInt* stck)
{
   if(stck==NULL)
   {
      return NULL;
   }
   List* list = new_p();
   StackInt* aux = stck;
   while (aux != NULL)
   {
      char buff[10];
      sprintf(buff,"%d",aux->data);//convierte el entero en string y lo guarda en el buffer
       insert(list,buff);
       aux=aux->next;
   }
   return list_to_arr(list); 
}

#endif