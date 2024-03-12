#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>

using namespace std;

int main() {
    pid_t pid;
    int pipefd[2];
    pipe(pipefd);
    fd_set read_fds;
    struct timeval timeout;
    pid = fork();

    // Process 1
    if (pid > 0) {
        close(pipefd[0]);  // Close read end

        int numberToSend = 42;

        // Simulate some delay before sending the number
        sleep(10);

        // Send the number to Process 2
        write(pipefd[1], &numberToSend, sizeof(numberToSend));

        cout << "Process 1: Sent number " << numberToSend << endl;

        close(pipefd[1]);  
        wait(NULL);
    }

    // Process 2
    else if (pid == 0){
        close(pipefd[1]);
        while (true) {
        FD_ZERO(&read_fds);
        FD_SET(pipefd[0], &read_fds);

        timeout.tv_sec = 3;  
        timeout.tv_usec = 0;

        int result = select(pipefd[0] + 1, &read_fds, nullptr, nullptr, &timeout);

         if (result == 0) {
            cout << "Process 2: Timeout occurred! No data received." << endl;
        } else if (result > 0) {
            if (FD_ISSET(pipefd[0], &read_fds)) {
                // Read the number sent by Process 1
                int receivedNumber;
                ssize_t bytesRead = read(pipefd[0], &receivedNumber, sizeof(receivedNumber));

                if (bytesRead == 0) {
                    cout << "Process 2: End of file reached." << endl;
                    break;
                }
                if (bytesRead > 0){
                cout << "Process 2: Received number " << receivedNumber << endl;
                }
            }
        }
        wait(NULL);
        }
        close(pipefd[0]); 
    }
    
    return 0;
}


