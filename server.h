#ifndef server_h
#define server_h
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>
#include <sstream>
#include <netinet/in.h>
#include <cstdint>
#include <filesystem>
using namespace std;
#define maxClients 100 
#define bufsize 1024

struct Client {
    string ip;
    int port;
};

class server {
    private: 
        const char* ipAddress;
        fd_set readfds; 
        struct sockaddr_in serverAddr;
        int masterSocket, addrlen, newSocket, clientSocket[maxClients], activity, i, valread, sd, maxSd, portSocket, port;
        Client newclient[maxClients];
        bool isError = false;
        string messageFromClient, ip;
        map<int, pair<string, string>> mapCommand;
        map<int, string> mapFile;
        // file 
        char buffer[bufsize] = {0};
        FILE* fp;
        long fileSize;
        
        void handleMessFromClient(int, int);

        void sendMessage(int port, char*);

        void recvMessage(int port);

        vector<string> removeDupWord(string);

        char* stringToChar(string);

        string getMessFromClient();

        void handleFileFromClient(int, int);

        void receiveContentFile(int);

        void handleFileSendToClient(int, string);

        void creatNewSocket();

        void selectClient();

        void handleExit(int);

    public: 

        server();

        bool getError();

        void init();

        void setIpAndPort(const char*, int);

        void communicateWithClient();

};
#endif