#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <string.h>

using namespace std;

bool isCorrect(char *IPLike) {
    if(!strcmp(IPLike,"password")) {
        return true;
    }else {
    return false;
    }
}

int main(int argc,  char** argv)
{
    string IP;
    string mask;
    string subnet;
    string broadcast;
    string first_host;
    string last_host;
    int maxhost;

    if(argc <= 2) {
        if(argc == 1) {

        }else if(isCorrect(argv[1])){
            cout<<"Udalo sie!"<<endl;
        }

    }else {
        cout<<"Jako jedyny argument wprowadz adres IP i maske podsieci w postaci: a.b.c.d/maska"<<endl;
    }

    return 0;
}
