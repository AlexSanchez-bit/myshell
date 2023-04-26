#include "header.h"
#ifndef Vars_h
#define Vars_h

List* varnames=NULL;
List* var_values=NULL;

void init_varlist()
{
    if(varnames==NULL){
    varnames=new_p();
    var_values=new_p();
    }
}

void print_vars()
{
    for(int i=0;i<varnames->size;i++)
    {
        printf("%s = %s\n",at(varnames,i),at(var_values,i));
    }
}


char* getValue(char* var_name)
{
    init_varlist();
    if(varnames->data==NULL)
    {
        return NULL;
    }
    int i=0;
    List* aux = varnames;
    while (strcmp(aux->data,var_name)!=0 && aux->next!=NULL)
    {
        aux=aux->next;
        i++;
    }

    if(strcmp(aux->data,var_name)==0)
    {
        return at(var_values,i);
    }
return NULL;
}

int get_value(char* var_name)
{
    char* value = getValue(var_name);    
    if(value!=NULL)
    {
        if(*value!='`'){
        printf("el valor de: %s es %s \n",var_name,value);
        }
        return (EXIT_SUCCESS);
    }
    printf("la variable no existe\n");
    return (EXIT_FAILURE);
}

int unset_var(char* varname)
{
    init_varlist();
    remove_l(var_values,getValue(varname));
    remove_l(varnames,varname);
}

int set_var(char** args)
{
    init_varlist();
    if(*(args+1)==NULL){
         print_vars();
         return 0;
    }

    char* old_value = getValue(*(args+1));

    if(old_value!=NULL)
    {
        unset_var(*(args+1));
    }

    size_t memory_size = 0;
    size_t argc=0;
    for(int i=2;*(args+i)!=NULL;i++)
    {
        memory_size+=strlen(*(args+i));
        argc++;
    }

    if(memory_size==0)
    {
        return EXIT_FAILURE;
    }

    insert(varnames,*(args+1));
    char* value;
    value = malloc(sizeof(char)*memory_size+argc-1);
    value[0]='\0';
    for(int i=2;*(args+i)!=NULL;i++)
    {
        value=strcat(value,*(args+i));
        if(argc>1){
        value=strcat(value," ");
        }
    }
    insert(var_values,value);
    return 0;
}

#endif
