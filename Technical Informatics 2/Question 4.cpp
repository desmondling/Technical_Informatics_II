#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string>

using namespace std;

int main() {
    pid_t pid;
    int pipefd1[2];
    pipe(pipefd1);
    struct Date{
        int day;
        int month;
        int year;
    };
    struct Person{
        string name;
        string familyname;
        int std_id;
        Date birthday;
    };
    pid = fork();
    
    if (pid > 0){
    	
        close(pipefd1[0]);
        Person person;
        cout << "Name: " ;
        cin >> person.name;
        
        cout << "Family Name: " ;
        cin >> person.familyname;
        
        cout << "Student ID: " ;
        cin >> person.std_id;
        
        cout << "Birthday (Day Month Year): " ;
        cin >> person.birthday.day >> person.birthday.month >> person.birthday.year;
        
        write(pipefd1[1],&person,sizeof(person));
        close(pipefd1[1]);
        
    }
    
    if (pid==0){

        close(pipefd1[1]);
        Person person;
        while(read(pipefd1[0],&person,sizeof(person))>0){
            cout << "Name: " << person.name << endl;
                cout << "Family Name: " << person.familyname << endl;
                cout << "Student ID: " << person.std_id << endl;
            cout << "Brithday: " << person.birthday.day << "/" << person.birthday.month << "/" << person.birthday.year << endl;
        }
        close(pipefd1[1]);
        wait(NULL);
    }
    return 0;
}
