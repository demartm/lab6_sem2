#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define max_len 2000
#define operands 1
#define low_prio 2
#define high_prio 3
#define open_br 4
#define close_br 5
#define equ 6
#define _space 7


struct point {

double x;/*
char cx;*/
struct point *next;

};
struct stack {

struct point *top;
size_t size;

};


void pushToStack(struct stack* st,double x/*,char cx*/){
if(st){

  struct point* el = (struct point*)calloc(sizeof(struct point),1);

  if(el){
    el->x = x;
    //el->cx = cx;
    el->next = st->top;
    st->top = el;
    st->size = st->size + 1;
  }

}

}

void popFromStack(struct stack* st,double *x, char *cx/*,double *num,char* cx*/){
if(st){

  if(st->top){

    double res = st->top->x;
    //char res_c = st->next->cx;
    struct point* next = st->top->next;

    free(st->top);
    st->top = next;

    st->size = st->size - 1;
    if(x){
      *x= res;
    }


    if(cx){
      *cx = (char)res;
    }

  }

}
}


void clearStack(struct stack* st){
if(st){

struct point* cur = st->top;

  while(cur){
    struct point* next = cur->next;
    free(cur);
    cur = next;
  }


}
}

int parseFile(char *file, int *Tab, char *str,size_t max_length){
int code = 0;

if(file && Tab && str){

FILE *f = fopen(file,"r");
if(f){

  char *buff = str;

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

      res = fscanf(f,"%c=%d",&b,&ch);
      if(res == 2){

          Tab[b] = ch;
      }
    }

for( char a = '0'; a <='9'; a++){
   Tab[a] = a - 48;
}


buff[size] = '\0';
//*str = buff;


}else{

code = 4;

}

}
  if(fclose(f) != 0){
    code = 2;
  }

}else{
code = 4;
}
return code;
}



int convertToRPN(char *expression,char *arr,size_t max_rpn){
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

  struct stack st = {0};

      char Tab[256] = {0};

      char oper[8] = {'+', '-', '/' ,'*' ,'(' ,')' ,'=',' '};
      int priority[8] = {2,2,3,3,4,5,6,7};

      for(int i = 0; i<sizeof(oper); i++){
        Tab[oper[i]] = priority[i];
      }


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

      if(i > 0 && Tab[expression[i - 1]] < _space){
        last = expression[i - 1];
      }

      if(Tab[expression[i]] == operands){
        if(arr_ix < max_rpn){
          arr[arr_ix] = expression[i];
        arr_ix++;
        }else{
          code = 11;
          i = ex_size;
        }
        //pushToStack(st,0,expression[i]);
      }else{

        if(Tab[expression[i]] == low_prio){
          if(last == 0 || (Tab[last] > operands && Tab[last] <= open_br  || Tab[last] == equ)) {//unary minus
              if(expression[i] == '-'){
                if(arr_ix < max_rpn){
                  arr[arr_ix] = '0';
                  arr_ix++;
                  }else{
                    code = 11;
                    i = ex_size;
                  }
                pushToStack(&st,expression[i]);
               }
          }else{
            while(st.top && (Tab[(int)(st.top->x)] == low_prio || Tab[(int)(st.top->x)] == high_prio)){
              char buf = 0;
              popFromStack(&st,NULL,&buf);
              if(arr_ix < max_rpn){
                arr[arr_ix] = buf;
                arr_ix++;
                }else{
                  code = 11;
                  i = ex_size;
                }
            }
            pushToStack(&st,expression[i]);
          }
        }else{

          if(Tab[expression[i]] == high_prio){

            if(st.size > 0 && (Tab[(int)(st.top->x)] == high_prio)){
                char buf = 0;
                popFromStack(&st,NULL,&buf);
              if(arr_ix < max_rpn){
                arr[arr_ix] = buf;
                arr_ix++;
                }else{
                  code = 11;
                  i = ex_size;
                }
            }
            pushToStack(&st,expression[i]);

          } else{
            if(Tab[expression[i]] == open_br){

                pushToStack(&st,expression[i]);

            } else {
              if(Tab[expression[i]] == close_br){

            while(st.size > 0 && Tab[(int)(st.top->x)] != open_br){
                char buf = 0;
                popFromStack(&st,NULL,&buf);
              if(arr_ix < max_rpn){
                arr[arr_ix] = buf;
                arr_ix++;
                }else{
                  code = 11;
                  i = ex_size;
                }
            }

            if(st.size == 0){
                code = 2;
              i = ex_size;
            }else{

                popFromStack(&st,NULL,NULL);

            }
              } else {
                if(Tab[expression[i]] == equ){
                   while(st.top && Tab[(int)(st.top->x)] != open_br){
                    char buf = 0;
                    popFromStack(&st,NULL,&buf);
                  if(arr_ix < max_rpn){
                    arr[arr_ix] = buf;
                    arr_ix++;
                    }else{
                      code = 11;
                      i = ex_size;
                    }
            }
                  pushToStack(&st,expression[i]);
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
  while(st.size && !code){
    char buf = 0;
    popFromStack(&st,NULL,&buf);
  if(arr_ix < max_rpn){
      arr[arr_ix] = buf;
  }else{
  code = 11;
}
  if(arr_ix < max_rpn){
    if(Tab[arr[arr_ix]] == open_br){
      code = 4;
    }
    arr_ix ++;

  }
}

  //return code;
}else{
code = 7;

}
if(arr_ix < max_rpn){
arr[arr_ix] = '\0';
}

clearStack(&st);

}else {
  code = 3;
}

return code;

}


double calculate(char* expression, int* Tab, char *prefix,int* err_code){
int code = 0;
if(expression && Tab){
  struct stack st = {0};//*st = (struct stack*)calloc(1,sizeof(struct stack));
  //if(st){
    size_t ex_size = strlen(expression);
    double temp1 = 0;
    double temp2 = 0;
    double res = 0;

    char name1 = 0;
    char name2 = 0;
    bool equality = false;

    for(size_t i = 0; i < ex_size;i++){

      if(expression[i] >= 'a' && expression[i] <= 'z' || expression[i] >= 'A' && expression[i] <= 'Z'|| expression[i] >= '0' && expression[i] <= '9'){//Tab[expression[i]] || expression[i] == '0' || i == 0){
        pushToStack(&st,Tab[expression[i]]);//for = symbol

      }else{
if(st.size < 2){
 // return 0;
code = 1;

i = ex_size;
}else{//error
          popFromStack(&st,&temp2,&name2);
          popFromStack(&st,&temp1,&name1);
        if(expression[i] == '+'){
          res = temp1 + temp2;
          pushToStack(&st,res);
        } else {
          if(expression[i] == '-'){
            res = temp1 - temp2;
            pushToStack(&st,res);
          }else{
            if(expression[i] == '*'){
              res = temp1 * temp2;
              pushToStack(&st,res);
            }else {
              if(expression[i] == '/'){
                if(temp2 != 0){
                  res = temp1 / temp2;
                  pushToStack(&st,res);
                }else{
                    code = 10;
                  i = ex_size;
                }
              } else {
                if(expression[i] == '='){
                if(!equality){

                  if(expression[0] >= 'a' && expression[0] <= 'z' || expression[0] >= 'A' && expression[0]<= 'Z'){
                      *prefix = expression[0];//buf;
                      equality = true;
                      pushToStack(&st,res);
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
if(st.size == 1 && code == 0){
  popFromStack(&st,&res,NULL);
  clearStack(&st);
  return res;
}
clearStack(&st);

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
int Tab[256]={0};// NULL;
char str[2000] = {0};
char arr[2000] = {0};
int code = 0;
char file[] = "test18.txt";//"test2.txt";//"test3.txt";//"test4.txt";//"test5.txt";//"test6.txt";//"test7.txt";//"test8.txt";//"test9.txt";//"test10.txt";//"test11.txt";//"test12.txt";//"test13.txt";//"test14.txt";//"test15.txt";//"test16.txt";//"test17.txt";//"test18.txt";//"test19.txt";//"test20.txt";
if((code = parseFile(file,Tab,str,max_len)) != 0){
  printf("error %d",code);
}

if(str){
  printf("\n%s\n",str);
}
code = convertToRPN(str,arr,2000);
if(!code){
    printf("\n%s\n",arr);
}else{
   printf("error %d",code);
}


char pref = 0;
code = 0;
double res = calculate(arr,Tab,&pref,&code);
if(!code){
  if(pref){
    printf("%c = %g\n",pref,res);
  }else{
    printf("res = %g\n",res);
  }
} else{
 printf("error %d\n",code);
}


//free(Tab);
//free(str);
    return 0;
}
