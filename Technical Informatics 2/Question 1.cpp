#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

int main() {
    pid_t pid;
    int pipefd1[2], pipefd2[2];
    int done_signal;
    pipe(pipefd1);
    pipe(pipefd2);

    pid = fork();
    if (pid > 0) {

    	 close(pipefd1[0]);
    	 close(pipefd2[1]);


        for (int i = 1; i <= 100; i++) {
            write(pipefd1[1], &i, sizeof(i));
            sleep(1);
            read(pipefd2[0], &done_signal, sizeof(done_signal));
            cout<<""<<done_signal<<endl;

        }
            close(pipefd2[0]);
        	close(pipefd1[1]);
    }



    else if (pid == 0) {

        close(pipefd1[1]);
        close(pipefd2[0]);
        int count;

        while (read(pipefd1[0], &count, sizeof(count)) > 0) {
            cout << " " << count << endl;
            sleep(1);
            done_signal = -1;
            write(pipefd2[1], &done_signal, sizeof(done_signal));
        }

        close(pipefd1[1]);
        close(pipefd2[0]);
        wait(NULL);

    }

    return 0;
}

