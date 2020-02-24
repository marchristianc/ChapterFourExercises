//
//  main.c
//  rpn
//
//  Created by William McCarthy on 192//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAXOP   100    /* max size of operand/operator */
#define NUMBER '0'     /* signal that a number was found */
#define MAXVAL  100

size_t sp = 0;   // aka unsigned long -- printf using %zu
double val[MAXVAL];   // stack of values

char buf[BUFSIZ];
size_t bufp = 0;

int getch_(void) { return (bufp > 0) ? buf[--bufp] : getchar(); }
void ungetch_(int c) {
  if (bufp >= BUFSIZ) { fprintf(stderr, "ungetch: too many characters\n");  return; }
  buf[bufp++] = c;
}

int getop(char* s) {
  int i, c;
  while ((s[0] = c = getch_()) == ' ' || c == '\t') { }  // skip whitespace
  s[1] = '\0';
  
  i = 0;
  if (c == '-' && !isdigit(s[++i] = c = getch_())) {
    ungetch_(c);
    c = s[0];
  }

  if (!isdigit(c) && c != '.') { return c; }  // if not a digit, return

  if (isdigit(c)) {  // get digits before '.'
    while (isdigit(s[++i] = c = getch_())) { }
  }
  if (c == '.') {    // get digits after decimal (if any)
    while (isdigit(s[++i] = c = getch_())) { }
  }
  s[i] = '\0';
  if (c != EOF) { ungetch_(c); }
  return NUMBER;      // return type is NUMBER, number stored in s
}

double pop(void) {
  if (sp == 0) { fprintf(stderr, "stack underflow\n");  return 0.0; }
  return val[--sp];
}

void push(double f) {
  if (sp == MAXVAL) { fprintf(stderr, "stack overflow -- can't push %g\n", f);  return; }
  val[sp++] = f;
}

double top(){
  if(sp > 0)
    return val[sp-1];
  else{
    fprintf(stderr, "stack underflow\n");  
    return 0.0;
  }
}

void swap(){
  if(sp >=2 ){
    double toSwap;
    int tgt = sp - 1;
    toSwap = val[tgt - 1];
    val[tgt - 1] = val[tgt];
    val[tgt] = toSwap;
  }else{
    fprintf(stderr, "stack underflow\n");
  }
}

void clear(){
  while(--sp > 0){
    val[sp] = 0;
  }
  sp = 0;
}

void rpn(void) {
  int type;
  double op2;
  char s[BUFSIZ];
  
  while ((type = getop(s)) != EOF) {
    switch(type) {
      case '\n':    printf("\t%.8g\n", pop());  break;
      case NUMBER:  push(atof(s));              break;
      case '+':     push(pop() + pop());        break;
      case '*':     push(pop() * pop());        break;
      case '-':     push(-(pop() - pop()));     break;
      case '/':
        if ((op2 = pop()) == 0.0) { fprintf(stderr, "divide by zero\n");  break; }
        push(pop() / op2);
        break;
      case '%':     
        if ((op2 = pop()) == 0.0) { fprintf(stderr, "divide by zero\n");  break; }
        push((int)op2 % (int)pop());     
        break;
      case '!':
        //print the top elements of the stack
        printf("\ttop of stack: %f\n", top());
        break;
      case '&':
        //duplicate top of stack
        push(top());
        break;
      case '$':
        //swap top of stack
        swap();
        printf("\tswapped the top of the stack\n");
        break;
      case '#':
        clear();
        printf("\tcleared the stack\n");
        break;
      case 's':
        push(sin(pop()));
        break;
      case 'e':
        push(exp(pop()));
        break;
      case 'p':
        push(pow(pop(), pop()));
        break;
      default:      fprintf(stderr, "unknown command %s\n", s);  break;
    }
  }
}

int main(int argc, const char * argv[]) {
  rpn();

  return 0;
}
