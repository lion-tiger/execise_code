#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#define MAXLINE 1024
#define FILE_MODE (S_IRUSR|S_IWUSR | /*S_IRGPR|*/S_IROTH)
#define SERV_FIFO "/tmp/fifo"

int main(int argc,char **argv)
{
    int ret;
    char buff[MAXLINE];
    char fifoname[MAXLINE];
    char *ptr;
    int len;
    int readfifo,writefifo;
    ssize_t n;
    pid_t pid;
    pid = getpid();
    //char c;
    printf("The process id is [%d]\n",pid);
    //scanf("%c",&c);
    //printf("%c",c);
    snprintf(fifoname,sizeof(fifoname),"/tmp/fifo.%ld",(long)pid);

    ret = mkfifo(fifoname,FILE_MODE);
    printf("the return value of mkfifo is [%d]\n",ret);

    snprintf(buff,sizeof(buff),"%ld",(long)pid);
    len = strlen(buff);
    ptr=buff+len;
    fgets(ptr,MAXLINE-len,stdin);
    len=strlen(buff);

    writefifo=open(SERV_FIFO,O_WRONLY,0);
    write(writefifo,buff,len);


    readfifo=open(fifoname,O_RDONLY,0);

    while((n=read(readfifo,buff,MAXLINE))> 0)
    {
        write(STDOUT_FILENO,buff,n);
    }
    close(readfifo);
    unlink(fifoname);

    return 0;
}
