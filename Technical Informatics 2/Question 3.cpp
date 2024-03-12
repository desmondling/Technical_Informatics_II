#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstring>
#include <pthread.h>

using namespace std;

void* send(void*arg);
void* receive(void*arg);
int pipefd1[2],pipefd2[2];

int main() {
    pid_t pid;
    pipe(pipefd1);
    pipe(pipefd2);

    pid = fork();
    if (pid == 0) {
    	close(pipefd1[1]);
    	close(pipefd2[0]);
    	int i;
    		while(read(pipefd1[0],&i,sizeof(i))>0){
    			write(pipefd2[1],&i,sizeof(i));
    		close(pipefd2[1]);
    		}
    }

    else if (pid > 0) {
    	pthread_t thread1,thread2;
    	pthread_create(&thread1,NULL,send,&pipefd1);
    	pthread_create(&thread2,NULL,receive,&pipefd2);
    	pthread_join(thread1,nullptr);
    	pthread_join(thread2,nullptr);
    }
    return 0;
}

void* send(void* arg){
	close(pipefd1[0]);
	for (int i=1;i<=5;i++){
		write(pipefd1[1],&i,sizeof(i));
		sleep(1);
		cout << "Thread 1 sends: " << i << endl;
	}
	close(pipefd1[1]);
	pthread_exit(NULL);
}
void* receive(void* arg){
	close(pipefd2[1]);
	for (int i=1;i<=5;i++){
		read(pipefd2[0],&i,sizeof(i));
		sleep(1);
		cout << "Thread 2 receives: " << i << endl;
	}
	close(pipefd2[0]);
	pthread_exit(NULL);
}

