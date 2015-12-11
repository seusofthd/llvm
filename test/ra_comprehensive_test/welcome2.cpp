#include <stdio.h>

int main() {
  //Test + - *
    int x = 1;
    int y = 2;
    int z;
    z = x + y;
    int test1 = z;

    int m = 4;
    int n = 5;
    int p;
    p = m - n;
    int test2 = p;

    int a = 6;
    int b = 7;
    int q;
    q = a * b;
    int test3 = q;


    //Test Branch
    if(p > 10)
      y = 1;
    else if(p > 0)
      y = 2;
    else
      y = 3;

    int test4 = y;

    //Test Loop
    int k = 0;
    int i = 0;
    while(i < 100){
      i = i + 1;
      k = k + 1;
    }
    

    int test5 = k;
  

    return test1 + test2 + test3 + test4 + test5;
}

