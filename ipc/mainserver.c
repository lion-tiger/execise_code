#include <unistd.h>
#include <readline/readline.h>
#include <fcntl.h>

#define MAXLINE 1024
#define SERV_FIFO "/tmp/fifo"
#define FILE_MODE (S_IRUSR|S_IWUSR | /*S_IRGPR|*/S_IROTH)
int main(int argc,char **argv)
{
    int ret_mkfifo;
    int readfifo,writefifo,fd;
    int dummyfifo;
    char buff[MAXLINE];
    char fifoname[MAXLINE],*ptr;
    pid_t pid;
    ssize_t n;
    //Step0 prepair receive server FIFO
    ret_mkfifo = mkfifo(SERV_FIFO,FILE_MODE);
    printf("Step0: the return of mkfifo is [%d]\n",ret_mkfifo);

    readfifo = open(SERV_FIFO,O_RDONLY,0);
    dummyfifo = open(SERV_FIFO,O_WRONLY,0);// import tips!!
    printf("readfifo is [%d],dummyfifo is [%d]\n",readfifo,dummyfifo);

    //while( (n = readline(readfifo,buff,MAXLINE))>0)
    while( (n = read(readfifo,buff,MAXLINE))>0)
    {
	//Step1 Read file name and pid number from client
	if (buff[n-1]=='\n')
	{
	    buff[n-1]='\0';
	}
	if ( (ptr=strchr(buff, ' '))==NULL){
	    printf("bogus request : %s\n",buff);
	    continue;
	}

	*ptr++=0;   // notice
	pid = atol(buff);
	    // client has already make the client fifo 
	    // so just directly open this fifo
	snprintf(fifoname,sizeof(fifoname),"/tmp/fifo.%ld",(long)pid);

	writefifo = open(fifoname,O_WRONLY,0);
	if (writefifo < 0)
	{
	    printf("can't open:%s\n",fifoname);
	    continue;
	}

	fd=open(ptr,O_RDONLY);
	if (fd<0)
	{
	    snprintf(buff,+n,sizeof(buff)-n,":can not open file\n");
	    n=strlen(ptr);
	    write(writefifo,buff,n);
	    close(writefifo);
	}
	else{
	    while( (n=read(fd,buff,MAXLINE))>0)
		    write(writefifo,buff,n);

	    close(fd);
	    close(writefifo);
	}

        //read();

        //Step2 make client fifo to send back data to client
	//mkfifo()

        //Step3  open the file to read
	//open()

        //Step4 read file context
	//read
	//Step5 write context to client FIFO
	//write
    }
	    
    return 0;
}
