// pingpongt.c for xv6 (assignment #1 for CSC.T371)
// name: Taisuke Sakugawa
// id: 21B30362

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// <<<remove this comment and fill your code here if needed>>>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(1, "usage: %s N\n", argv[0]);
        exit(1);
    }

    // # of rounds
    int n = atoi(argv[1]);

    // tick value before starting rounds
    int start_time = gettimeofday();

    int pp[2],qq[2];
    unsigned char buf[1];
    unsigned int num =0;

    //create pipe
    pipe(pp); //child -> parent
    pipe(qq); //parent -> child

    int status;
    if(fork() >  0){
        //parent process
        close(qq[0]);
        close(pp[1]);

        for(int i=0;i<n;i++){
            buf[0]=(unsigned char)num%256;
            write(qq[1],buf,1);
            read(pp[0],buf,1);
            num = (int)buf[0];
            num++;
        }

        //wait child process
        wait(&status);

        // tick value after ending rounds
        int end_time = gettimeofday();
        // print # of ticks in N rounds
        printf("# of times[sec] in %d rounds: %d\n", n, end_time - start_time);
        exit(0);
    }else{
        //child process
        close(pp[0]);
        close(qq[1]);

        for(int i=0;i<n;i++){
            read(qq[0],buf,1);
            num = (int)buf[0];
            num++;
            buf[0]=(unsigned char)num%256;
            write(pp[1],buf,1);
        }
    }
    exit(0);
}