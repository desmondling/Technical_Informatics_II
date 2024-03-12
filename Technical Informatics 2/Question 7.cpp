#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <pthread.h>

using namespace std;

void* p1(void*arg);
void* p2(void*arg);
void* p3(void*arg);
int pipefd1[2],pipefd2[2],pipefd3[2];

int main() {
    pid_t pid;
    pipe(pipefd1);
    pipe(pipefd2);
    pipe(pipefd3);
    fd_set read_fds;
    struct timeval timeout;
    pid = fork();

    // Process 1
    if (pid > 0) {
        pthread_t thread1,thread2,thread3;
    	pthread_create(&thread1,NULL,p1,&pipefd1);
    	pthread_create(&thread2,NULL,p2,&pipefd2);
    	pthread_create(&thread3,NULL,p3,&pipefd3);
    	pthread_join(thread1,nullptr);
    	close(pipefd1[1]);
    	sleep(2);
    	pthread_join(thread2,nullptr); 
    	close(pipefd2[1]);
    	sleep(2);
    	pthread_join(thread3,nullptr);
    	close(pipefd3[1]);
    	sleep(2);
    	//wait(NULL);
    }

    // Process 2
    else if (pid == 0){
        close(pipefd1[1]);
        close(pipefd2[1]);
        close(pipefd3[1]);
        while (true) {
            FD_ZERO(&read_fds);
            FD_SET(pipefd1[0], &read_fds);
            FD_SET(pipefd2[0],&read_fds);
            FD_SET(pipefd3[0],&read_fds);

            timeout.tv_sec = 1;  
            timeout.tv_usec = 0;

            if (select((pipefd1[0],pipefd2[0],pipefd3[0])+1, &read_fds, nullptr, nullptr, &timeout) > 0){
                if (FD_ISSET(pipefd1[0], &read_fds)) {
                    // Read the number sent by Thread 1
                    int i;
                    read(pipefd1[0], &i, sizeof(i));
                    sleep(2);
                    cout << "Received number from Thread 1: " << i << endl;
                }
                if (FD_ISSET(pipefd2[0], &read_fds)) {
                    // Read the number sent by Thread 2
                    int j;
                    read(pipefd2[0], &j, sizeof(j));
                    sleep(2);
                    cout << "Received number from Thread 2:  " << j << endl;
                }
                if (FD_ISSET(pipefd3[0], &read_fds)) {
                    // Read the number sent by Thread 3
                    int k;
                    read(pipefd3[0], &k, sizeof(k));
                    sleep(2);
                    cout << "Received number from Thread 3: " << k << endl;
                }
            }
        }
        close(pipefd1[0]); 
        close(pipefd2[0]);
        close(pipefd3[0]);
    }
    
    
    
    wait(NULL);
    return 0;
}

void* p1(void* arg){
	close(pipefd1[0]);
	int i = 1234;
	for (i=1; i<=5; i++){
	sleep(2);
	
	write(pipefd1[1],&i,sizeof(i));
	cout << "Thread 1 sends: " << i << endl;
	}
	wait(NULL);
	pthread_exit(NULL);
}
void* p2(void* arg){
	close(pipefd2[0]);
	int j = 5678;
	sleep(5);
	write(pipefd2[1],&j,sizeof(j));
	cout << "Thread 2 sends: " << j << endl;
	wait(NULL);
	pthread_exit(NULL);
}
void* p3(void* arg){
	close(pipefd3[0]);
	int k = 9012;
	sleep(10);
	write(pipefd3[1],&k,sizeof(k));
	cout << "Thread 3 sends: " << k << endl;
	wait(NULL);
	pthread_exit(NULL);
}


