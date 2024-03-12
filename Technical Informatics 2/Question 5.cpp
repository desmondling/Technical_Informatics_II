#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string>

using namespace std;

int main() {
    pid_t pid1,pid2;
    int pipefd1[2],pipefd2[2];
    string password;
    pipe(pipefd1);
    pipe(pipefd2);
    pid1 = fork();
    
    if (pid1 > 0){
    	
        close(pipefd1[0]);
        cout << "Password: " ;
        cin >> password;
        
        write(pipefd1[1],&password,sizeof(password));
        close(pipefd1[1]);
        
    }
    
    else if (pid1==0){
        pid2 = fork();
        
        if (pid2 > 0){
            close(pipefd1[1]);
            close(pipefd2[0]);
            read(pipefd1[0],&password,sizeof(password));
            cout << "Child Password is " << password << endl;
            write(pipefd2[1],&password,sizeof(password));
            
            close(pipefd1[0]);
            close(pipefd2[1]);
            wait(NULL);
        }
        
        else if (pid2==0){
            close(pipefd2[1]);
            read(pipefd2[0],&password,sizeof(password));
            cout << "Grandchild Password is " << password << endl;
            
            close(pipefd2[0]);
            wait(NULL);
        }
    }
    return 0;
}
