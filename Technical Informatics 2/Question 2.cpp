#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

int main() {
    pid_t pid;

    pid = fork();
    if (pid > 0) {

    	cout << "Parent PID in Parent Process: " << getpid() << endl;
    	cout << "Child PID in Parent Process: " << pid << endl;
    	wait(NULL);
        }



    else if (pid == 0) {

    	cout << "Parent PID in Child Process: " << getppid() << endl;
    	cout << "Child PID in Child Process: " << getpid() << endl;
        wait(NULL);
    }

    return 0;
}

