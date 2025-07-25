#include <stdio.h>
#include <string.h>

int main(void){
    char* msg = "hello\nitheima";
    FILE* file = fopen("practise.txt","w+");
    if(file == NULL){
        perror("打开文件失败!");
        return -1;
    }

    int i = 0;
    //将字符串内容轮询写入文件
    while(i < strlen(msg)){
        fputc(msg[i],file);
        i++;
    }
    fputc('\n',file);
    fclose(file);
}