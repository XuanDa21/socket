#ifndef viewClient_h
#define viewClient_h
#include <string>
#include <iostream>
#include "client.h"
#include <vector>
using namespace std;
class viewClient {
    private: 
        client cli;

        string message;

        void printErrClient();

        void setInit();

        string setMessage();

        string getMessage();

    public: 

        viewClient();

        void startClient();


};
#endif