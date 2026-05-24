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

char *convertToRPN(char* expression,int *result){
int code = 0;

if(expression){

char *arr = (char*)calloc(strlen(expression) * 2 + 1,1);
if(arr){

  struct stack* st = (struct stack*)calloc(sizeof(struct stack),1);

  if(st){

  unsigned char Tab[256] = {0};
  unsigned char numbers[10] = {'0','1','2','3','4','5','6','7','8','9'};
  unsigned char low_prior[3] = {'+','-'};
  unsigned char high_prior[3] = {'/','*'};
  unsigned char open = '(';
  unsigned char close = ')';
  unsigned char space = ' ';


  size_t sizeNums = sizeof(numbers);
  size_t sizeLow = sizeof(low_prior);
  size_t sizeHigh = sizeof(high_prior);

  size_t arr_ix = 0;

  for(int i = 0; i < sizeNums; i++){
    Tab[numbers[i]] = 1;
  }

  for(int i = 0; i < sizeLow; i++){
    Tab[low_prior[i]] = 2;
  }

  for(int i = 0; i < sizeHigh; i++){
    Tab[high_prior[i]] = 3;
  }
  Tab[open] = 4;
  Tab[close] = 5;
  Tab[space] = 6;

  size_t ex_size = strlen(expression);

  char last = 0;

  for(size_t i = 0; i < ex_size && !code; i++){

  if(i > 0 && Tab[expression[i - 1]] < 6){
    last = expression[i - 1];
  }
  if(last && Tab[expression[i]] > 2 && Tab[expression[i]] < 4 && Tab[last] > 1 && Tab[last] < 4){
    code = 1;
    i = ex_size;
  }else{

    if(Tab[expression[i]] == 1){

      arr[arr_ix] = expression[i];
      arr_ix ++;

    }else{
      if(Tab[expression[i]] == 2){

        if(last == 0 || (Tab[last] > 1 && Tab[last] < 4)){
          if(expression[i] == '-'){
          arr[arr_ix] = '0';
          arr_ix ++;
          pushToStack(st,expression[i]);
        }
        }else{

          while(st->next && (Tab[st->next->x] == 2 || Tab[st->next->x] == 3)){

            arr[arr_ix] = popFromStack(st);
            arr_ix ++;
          }
          pushToStack(st,expression[i]);
        }
      }else{
        if(Tab[expression[i]] == 3){

        if(st->next && (Tab[st->next->x] == 3)){
          arr[arr_ix] = popFromStack(st);
          arr_ix ++;
        }
        pushToStack(st,expression[i]);

        }else{
        if(Tab[expression[i]] == 4){

          pushToStack(st,expression[i]);

        }else{
          if(Tab[expression[i]] == 5){

            while(st->size != 0 && st->next->x != open){

              arr[arr_ix] = popFromStack(st);
              arr_ix ++;
            }

            if(st->size == 0){
              code = 2;
              i = ex_size;
            }else{

              if(st->size){
                popFromStack(st);
              }
            }

          } else{
            if(Tab[expression[i]] != 6){
              code = 3;
              i = ex_size;
            }
          }
        }
      }
      }
    }
  }
  }
bool cont = true;
while(st->size && cont){
  arr[arr_ix] = popFromStack(st);

  if(arr[arr_ix] == open){
    code = 4;
  }
  arr_ix ++;

}

if(!code){
  return arr;
} else {
  if(st){
    freeStack(&st);
  }

  if(arr){
    free(arr);
  }
}

}else{
  code = 5;
}
}else{
  code = 6;
}
} else{
  code = 7;
}
if(result){
  *result = code;
}
return NULL;
}



int main()
{

char expression[18][50] = {
{"-(1+2)+3*4*(5-6)"},
{"-1+2+-2+3*4*(5-6)"},
{"(1+(2-(3+(4-(5+(6-(7+(8-9))))))))"},
{"1-2-3-4-5"},
{"1/2/3/4"},
{"1+2+3+4+5+6+7+8+9"},
{"1+2*3+4*5+6"},
{"(1+2)*(3+4)*(5+6)"},
{"1*(2+3*(4+5*(6+7)))"},
{"((1+2)*3-4)/5"},
{"1-(2-(3-(4-5)))"},
{"++1++2*(3-4*(5+6/7)-8)*9"},
{"((1+2)*((3+4)*(5+6)))"},
{"+-++-(((1)))"},
{"1 2"},//TODO
{"1*2*3*4*5a*6*7*8*9"},
{"1+2*3-4/5+6*7-8"},
{"1++2******3-4/5+6*7-8"},
}
;//"1+2+3*4       *(5-6)";
int code = 0;
for(int i = 0; i < sizeof(expression)/50; i++){
  char *arr = convertToRPN(expression[i],&code);
if(arr){
printf("%s\n",arr);
}else{
printf("error %d\n",code);
}
}

return 0;
}
