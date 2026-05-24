#include <stdio.h>
#include <stdlib.h>

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


int main()
{

struct stack* st = (struct stack*)malloc(sizeof(struct stack));
st->size = 0;
st->next = NULL;

// for(int i = 1; i <= 10;i++){
// pushToStack(st,i);
// }

pushToStack(st,'+');
pushToStack(st,'-');
pushToStack(st,'/');
pushToStack(st,'*');
pushToStack(st,'(');
pushToStack(st,')');

//st->size = 1;
int code = 0;
// while(st->size && code == 0){

// int cur = popFromStack(st,&code);
// if(code == 0){
//   printf("pop value: %d stack size: %d\n",cur,st->size);
// }else{
//   printf("empty stack");
// }

// }
printStack(st);

freeStack(&st);
return 0;
}
