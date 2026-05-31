#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define max_length 2000
#define operands 1
#define low_prio 2
#define high_prio 3
#define open_br 4
#define close_br 5
#define equ 6
#define _space 7


struct point {

double x;
char cx;
struct point *next;

};
struct stack {

struct point *next;
size_t size;

};


void pushToStack(struct stack* st,double x,char cx){
if(st){

  struct point* el = (struct point*)calloc(sizeof(struct point),1);

  if(el){
    el->x = x;
    el->cx = cx;
    el->next = st->next;
    st->next = el;
    st->size = st->size + 1;
  }

}
  return;
}

void popFromStack(struct stack* st,double *num,char* cx){
if(st){

  if(st->next){

    double res = st->next->x;
    char res_c = st->next->cx;
    struct point* next = st->next->next;

    free(st->next);
    st->next = next;

    st->size = st->size - 1;
    if(num){
      *num = res;
    }

    if(cx){
      *cx = res_c;
    }

  }

}

return;
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

int parseFile(char *file, int **Tab, char **str){
int code = 0;

if(file && Tab && str){

FILE *f = fopen(file,"r");
if(f){

  char *buff = (char*)calloc(max_length+1,1);
  if(buff){
    *Tab = (int*)calloc(256,sizeof(int));

    if(*Tab){
    char b = 0;

    size_t size = 0;

      while(fscanf(f,"%c",&b) == 1 && b !='\n' && size < max_length){
        if(b != ' '){

          buff[size] = b;
          size++;
        }
      }
if(size < max_length){

    int ch = 0;
    int res = 0;
    while(res != -1){

      res = fscanf(f," %c = %d",&b,&ch);
      if(res == 2){

          (*Tab)[b] = ch;
      }
    }

for( char a = '0'; a <='9'; a++){
   (*Tab)[a] = a - 48;
}


buff[size] = '\0';
*str = buff;


}else{

code = 4;
free(buff);
free(*Tab);
*Tab = NULL;

}
}else{

  free(buff);
  code = 1;

}
}
  if(fclose(f) != 0){
    code = 2;
  }
} else {
code = 3;
}
}else{
code = 4;
}
return code;
}



char *convertToRPN(char *expression,int *err_code){
int code = 0;
if(expression){


  /*
  #define max_length 2000
  #define operands 1
  #define low_prio 2
  #define high_prio 3
  #define open_br 4
  #define close_br 5
  #define equ 6
  #define _space 7
  */

  struct stack *st = (struct stack*)calloc(1,sizeof(struct stack));
  if(st){

  size_t max_rpn =  max_length * 2;//uslovno

  char *arr = (char*)calloc(max_rpn+ 1,1);
  if(arr){

      char Tab[256] = {0};

      char low_prior[2] = {'+','-'};
      char high_prior[2] = {'/','*'};
      char open = '(';
      char close = ')';
      char space = ' ';
      char equal = '=';


      for(int i = 0; i < sizeof(low_prior); i++){
        Tab[low_prior[i]] = low_prio;
      }

      for(int i = 0; i < sizeof(high_prior); i++){
        Tab[high_prior[i]] = high_prio;
      }


      Tab[open] = open_br;
      Tab[close] = close_br;
      Tab[space] = space;
      Tab[equal] = equ;

      for(char a = 'a'; a <= 'z';a++){
        Tab[a] = operands;
      }

      for(char a = 'A'; a <= 'Z';a++){
        Tab[a] = operands;
      }

      for(char a = '0'; a <= '9';a++){
        Tab[a] = operands;
      }

    size_t arr_ix = 0;

    size_t ex_size = strlen(expression);
    unsigned char last = 0;

    for(size_t i = 0; i < ex_size; i++){

      if(i > 0 && /*Tab[expression[i - 1]] > operands &&*/ Tab[expression[i - 1]] < _space){
        last = expression[i - 1];
      }

      if(Tab[expression[i]] == operands){
        if(arr_ix < max_rpn){
          arr[arr_ix] = expression[i];
        }
        arr_ix++;
        //pushToStack(st,0,expression[i]);
      }else{

        if(Tab[expression[i]] == low_prio){
          if(last == 0 || (Tab[last] > operands && Tab[last] < open_br  || Tab[last] == equ)) {//unary minus
              if(expression[i] == '-'){
                if(arr_ix < max_rpn){
                  arr[arr_ix] = '0';
                }
                arr_ix++;
                pushToStack(st,0,expression[i]);
               }
          }else{
            while(st->next && (Tab[st->next->cx] == low_prio || Tab[st->next->cx] == high_prio)){
              char buf = 0;
              popFromStack(st,NULL,&buf);
              if(arr_ix < max_rpn){
                arr[arr_ix] = buf;
              }
              arr_ix ++;
            }
            pushToStack(st,0,expression[i]);
          }
        }else{

          if(Tab[expression[i]] == high_prio){

            if(st->size > 0 && (Tab[st->next->cx] == high_prio)){
                char buf = 0;
                popFromStack(st,NULL,&buf);
              if(arr_ix < max_rpn){
                arr[arr_ix] = buf;
              }
                arr_ix ++;
            }
            pushToStack(st,0,expression[i]);

          } else{
            if(Tab[expression[i]] == open_br){

                pushToStack(st,0,expression[i]);

            } else {
              if(Tab[expression[i]] == close_br){

            while(st->size > 0 && Tab[st->next->cx] != open_br){
                char buf = 0;
                popFromStack(st,NULL,&buf);
              if(arr_ix < max_rpn){
                arr[arr_ix] = buf;
              }
                arr_ix ++;
            }

            if(st->size == 0){
                code = 2;
              i = ex_size;
            }else{

                popFromStack(st,NULL,NULL);

            }
              } else {
                if(Tab[expression[i]] == equ){
                   while(st->next && Tab[st->next->cx] != open_br){
                    char buf = 0;
                    popFromStack(st,NULL,&buf);
                  if(arr_ix < max_rpn){
                    arr[arr_ix] = buf;
                  }
                    arr_ix ++;
            }
                  pushToStack(st,0,expression[i]);
                } else {
                  if(Tab[expression[i]] != _space){
                    code = 9;//return NULL;
                    i = ex_size;
                  }
                }
              }
            }
          }
        }
      }


    }
if(code == 0){
  //bool cont = true;
  while(st->size && !code){
    char buf = 0;
    popFromStack(st,NULL,&buf);
    arr[arr_ix] = buf;

    if(Tab[arr[arr_ix]] == open_br){
      code = 4;
    }
    arr_ix ++;

  }

  return arr;
}else{
code = 7;

}
freeStack(&st);
}else{
  code = 6;
  freeStack(&st);
}
}else{
  code = 5;
}
//}
}else {
  code = 3;
}
if(err_code){
  *err_code = code;
}
return NULL;

}


double calculate(char* expression, int* Tab, char *prefix,int* err_code){
int code = 0;
if(expression && Tab){
  struct stack *st = (struct stack*)calloc(1,sizeof(struct stack));

  if(st){
    size_t ex_size = strlen(expression);
    double temp1 = 0;
    double temp2 = 0;
    double res = 0;

    char name1 = 0;
    char name2 = 0;
    bool equality = false;

    for(size_t i = 0; i < ex_size;i++){

      if(expression[i] >= 'a' && expression[i] <= 'z' || expression[i] >= 'A' && expression[i] <= 'Z'|| expression[i] >= '0' && expression[i] <= '9'){//Tab[expression[i]] || expression[i] == '0' || i == 0){
        pushToStack(st,Tab[expression[i]],expression[i]);//for = symbol

      }else{
if(st->size < 2){
 // return 0;
code = 1;

i = ex_size;
}else{//error
          popFromStack(st,&temp2,&name1);
          popFromStack(st,&temp1,&name2);
        if(expression[i] == '+'){
          res = temp1 + temp2;
          pushToStack(st,res,0);
        } else {
          if(expression[i] == '-'){
            res = temp1 - temp2;
            pushToStack(st,res,0);
          }else{
            if(expression[i] == '*'){
              res = temp1 * temp2;
              pushToStack(st,res,0);
            }else {
              if(expression[i] == '/'){
                if(temp2 != 0){
                  res = temp1 / temp2;
                  pushToStack(st,res,0);
                }else{
                    code = 10;
                  i = ex_size;
                }
              } else {
                if(expression[i] == '='){
                if(!equality){
                  pushToStack(st,temp2,name2);
                  if(name2 >= 'a' && name2 <= 'z' || name2 >= 'A' && name2 <= 'Z'){
                      *prefix = name2;//buf;
                      equality = true;
                      } else {
                        code = 6;
                        i = ex_size;
                  }
                  }else{
                      code = 5;
                  i = ex_size;
                  }
                } else{
                    code = 2;
                  i = ex_size;
                  //error
                }
              }
            }
          }
        }
    }
}//
}
if(st->size == 1 && code == 0){
  popFromStack(st,&res,NULL);
  freeStack(&st);
  return res;
}
freeStack(&st);
  }else{

code = 3;
}
} else{

code = 4;

}
if(err_code){
  *err_code = code;
}
return 0;
}


int main()
{
int *Tab= NULL;
char *str = NULL;;
int code = 0;
char file[] = "test.txt";//"test2.txt";//"test3.txt";//"test4.txt";//"test5.txt";//"test6.txt";//"test7.txt";//"test8.txt";//"test9.txt";//"test10.txt";//"test11.txt";//"test12.txt";//"test13.txt";//"test14.txt";//"test15.txt";//"test16.txt";//"test17.txt";//"test18.txt";//"test19.txt";//"test20.txt";
if((code = parseFile(file,&Tab,&str)) != 0){
  printf("error %d",code);
}

if(str){
  printf("\n%s\n",str);
}
char *text = convertToRPN(str,&code);
if(text){
    printf("\n%s\n",text);
}else{
   printf("error %d",code);
}


char pref = 0;
code = 0;
double res = calculate(text,Tab,&pref,&code);
if(!code){
  if(pref){
    printf("%c = %g\n",pref,res);
  }else{
    printf("res = %g\n",res);
  }
} else{
 printf("error %d\n",code);
}


free(Tab);
free(str);
    return 0;
}
