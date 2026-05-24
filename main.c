#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct point {

  char x;
  struct point* next;

};

struct stack {

  struct point* next;
  size_t size;

};

void pushToStack(struct stack* st,char x){
if(st){

  struct point* el = (struct point*)malloc(sizeof(struct point));

  if(el){
    el->x = x;
    el->next = st->next;
    st->next = el;
    st->size = st->size + 1;
  }

}
  return;
}

char popFromStack(struct stack* st/*,int *code*/){
if(st){

  if(/*st->size > 0 && */st->next){

    char res = st->next->x;
    struct point* next = st->next->next;

    free(st->next);
    st->next = next;

    st->size = st->size - 1;

    //*code = 0;
    return res;
  }
  // else{
  //   if(code){
  //     *code = -1;
  //   }
  // }

}
  // if(code){
  //     *code = -2;
  //   }
return 0;//TODO
}

void freeStack(struct stack** st){
if(st && *st){

struct point* cur = (*st)->next;

  while(cur){
    struct point* next = cur->next;
    free(cur);
    cur = next;
  }

  free(*st);
  (*st) = NULL;

}
}

void printStack(struct stack *st){
if(st){
  struct point *cur = NULL;
    for(cur = st->next; cur; cur = cur->next){
      printf("%c\n",cur->x);
    }
  }
}


//fillTab()

void priorityLowEqual(char* expression){
if(expression){

unsigned char Tab[256] = {0};
unsigned char numbers[10] = {'0','1','2','3','4','5','6','7','8','9'};
unsigned char low_prior[3] = {'+','-','('};
unsigned char high_prior[3] = {'/','*',')'};

size_t sizeNums = sizeof(numbers);
size_t sizeLow = sizeof(low_prior);
size_t sizeHigh = sizeof(high_prior);

for(int i = 0; i < sizeNums; i++){
  Tab[numbers[i]] = 1;
}

for(int i = 0; i < sizeLow; i++){
  Tab[low_prior[i]] = 2;
}

for(int i = 0; i < sizeHigh; i++){
  Tab[high_prior[i]] = 3;
}

size_t ex_size = strlen(expression);
for(size_t i = 0; i < ex_size; i++){
  if(Tab[expression[i]] == 1){
    printf("%c<number>",expression[i]);
  }else{
    if(Tab[expression[i]] == 2){
      printf("%c<low>",expression[i]);
    }else{
      if(Tab[expression[i]] == 3){
        printf("%c<high>",expression[i]);
      }
    }
  }

}

}
}



int main()
{

char expression[] = "1+2+3*4";
priorityLowEqual(expression);

return 0;
}
