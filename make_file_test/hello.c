#include<stdio.h>



int add(int,int);
int sub(int,int);




int main( int argc , char *argv[]){
    
    int a = 10;
    int b = 5;
    printf("hello world!\n");
    printf("%d+%d=%d\n",a,b,add(a,b));
    printf("%d-%d=%d\n",a,b,sub(a,b));


    return 0;
}

