#ifndef viewServer_h
#define viewServer_h
#include <string>
#include <iostream>
#include "server.h"
using namespace std;
class viewServer {
    private: 
        server ser;

        void printErrServer();

        void setInit();

    public: 

        viewServer();

        void startServer();

};
#endif