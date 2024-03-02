#ifndef client_h
#define client_h
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#define bufsize 1024
using namespace std;
class client { 
    private: 
        const char* ipAddress;
        int clientSocket, port;  
        bool isError = false;          
        struct sockaddr_in serverAddr;
        string messageFromServer, messageToServer;
        char buffer[bufsize] = {0};
        FILE* fp;
        long fileSize;

        char* stringToChar(string);

        vector<string> removeDupWord(string);

        void handleFileSendToServer();

        void sendMessage(string);

        void recMessage();
        
        void sendFileToServer();

        void receiveContentFile();
        
    public: 

        client();

        string getMessageFromServer();

        bool getError();

        void setIpAndPort(const char*, int);

        void init();

        void communicateWithServer(string);

};  

#endif