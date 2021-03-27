#include<stdlib.h>
#include<stdio.h>
#include<string.h>

void err_func(FILE *fp){
    if(fp==NULL){
        perror("file open/write error\n");
        exit(1);
    }
}


int main(int argc,char *argv[]){
    
    FILE *fp , *fp_out ;
    int n = 0;

    fp = fopen("open.c","r");
    err_func(fp);

    fp_out = fopen("open1.c","w");
    err_func(fp_out);
    

    while((n = fgetc(fp))!=EOF){
        fputc(n,fp_out);
    }

    fclose(fp);
    fclose(fp_out);
    return 0;
}
