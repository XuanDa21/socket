#include "server.h"
server::server():ipAddress("127.0.0.2"), port(8080) {} 

void server::setIpAndPort(const char* ip, int port) {
    this -> ipAddress = ip;
    this -> port = port;
}

string server::getMessFromClient() {
    return this -> messageFromClient;
}

bool server::getError() {
    return this -> isError;
}

void server::init() {
    for (i = 0; i < maxClients; i++) {  
        clientSocket[i] = 0;  
    }  
    masterSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (masterSocket == -1) {
        isError = true;
    }
    string tempIp = ipAddress;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port); 
    inet_pton(AF_INET, tempIp.c_str(), &serverAddr.sin_addr.s_addr);
    if (bind(masterSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        isError = true;
    }
    if (listen(masterSocket, 5) == -1) {
        isError = true;
    }
    addrlen = sizeof(serverAddr);  
}

vector<string> server::removeDupWord(string str) {
    istringstream ss(str);
    string word; 
    vector<string> strs;
    while (ss >> word)
    {
        strs.push_back(word);
    } 
    return strs;
}

void server::handleMessFromClient(int port, int sd) {
    vector<string> strs = removeDupWord(getMessFromClient());
    string portStr = to_string(port);
    char* temp;
    if(strs[0].compare("PUT") == 0) {
        int k = stoi(strs[1]);
        if(mapCommand.count(k)) {
            isError = true;
        }
        mapCommand[k] = make_pair(portStr, strs[2]);
        temp = stringToChar("OK!");
        sendMessage(sd, temp);
    }

    else if(strs[0].compare("GET") == 0) {
        int k = stoi(strs[1]);
        if(mapCommand.count(k)) {
            auto it = mapCommand.find(k);
            string value = it -> second.second;
            char* tempV = stringToChar(value);
            if(tempV != 0) {
                temp = stringToChar("Value is "+value);
                sendMessage(sd, temp);
            }
        }
        else {
            char* temp = stringToChar("No value!");
            sendMessage(sd, temp);
        }
    }

    else if(strs[0].compare("DEL") == 0) {
        int k = stoi(strs[1]);
        if(mapCommand.count(k)) {
            auto it = mapCommand.find(k);
            int p = stoi(it -> second.first);
            if(p == port) {
                mapCommand.erase(it);
                temp = stringToChar("Deleted!");
                sendMessage(sd, temp);
            }
            else {
                temp = stringToChar("Not allowed!");
                sendMessage(sd, temp);
            }
        }
    }
}

void server::handleFileFromClient(int port, int sd) {
    vector<string> strs = removeDupWord(getMessFromClient());
    if(strs[0].compare("PUTFILE") == 0) {
        char* temp = stringToChar("OK!");
        sendMessage(sd, temp);
        receiveContentFile(sd);
        mapFile[port] = strs[1];
    }

    else if(strs[0].compare("GETFILE") == 0) {
        auto it = mapFile.find(port);
        string fileName = "new"+it -> second;
        if(fileName.compare(fileName) == 0) {
            char* temp = stringToChar("Recevied!");
            sendMessage(sd, temp);
            handleFileSendToClient(sd,fileName);
        }
    }
    

    else if(strs[0].compare("DELFILE") == 0) {
        auto it = mapFile.find(port);
        string fileName = strs[1];
        const char* fnremove = fileName.c_str();
        if(fileName.compare(fileName) == 0) {
            remove(fnremove);
            char* temp = stringToChar("Removed!");
            sendMessage(sd, temp);
        }
    }
}

void server::sendMessage(int port, char* message) {
    send(port, message, strlen(message), 0) != strlen(message);
}

void server::recvMessage(int port) {
    char buffer[bufsize];
    memset(buffer, 0, bufsize);
    int isReceive = recv(port, buffer, bufsize, 0);
    handleExit(isReceive);
    string receive(buffer);
    messageFromClient = receive;
}

void server::handleExit(int value) {
    if(value <= 0) {
        exit(0);
    }
}

void server::selectClient() {
    Client tempclient;
    for (i = 0; i < maxClients; i++) {  
        sd = clientSocket[i];
        tempclient = newclient[i];
        if (FD_ISSET(sd, &readfds)) {  
            portSocket = tempclient.port;
            recvMessage(sd);
            handleFileFromClient(portSocket, sd);
            handleMessFromClient(portSocket, sd);
        }  
    } 
}

void server::creatNewSocket() {
    FD_ZERO(&readfds);  
    FD_SET(masterSocket, &readfds);  
    maxSd = masterSocket;  
    for (i = 0; i < maxClients; i++) {  
        sd = clientSocket[i];  
        if(sd > 0)  
            FD_SET(sd, &readfds);  
        if(sd > maxSd)  
            maxSd = sd;  
    } 
    activity = select(maxSd+1, &readfds, NULL, NULL, NULL);  
    if (FD_ISSET(masterSocket, &readfds)) {  
        newSocket = accept(masterSocket, (struct sockaddr *)&serverAddr, (socklen_t*)&addrlen);
        for (i = 0; i < maxClients; ++i) {  
            if(clientSocket[i] == 0) {   
                clientSocket[i] = newSocket;
                newclient[i].ip = inet_ntoa(serverAddr.sin_addr);
                newclient[i].port = ntohs(serverAddr.sin_port);
                break;  
            }  
        }
    }
}

char* server::stringToChar(string str) {
    char* charArr = &str[0];
    return charArr;
}

void server::receiveContentFile(int sd) {
    vector<string> strs = removeDupWord(getMessFromClient());
    string str = "new"+strs[1];
    const char* fileName = str.c_str();
    recv(sd, &fileSize, sizeof(fileSize), 0);
    fp = fopen(fileName, "wb");
    while (fileSize > 0) {
        int bytesRead = recv(sd, buffer, sizeof(buffer), 0);
        fwrite(buffer, 1, bytesRead, fp);
        fileSize -= bytesRead;
    }
    fclose(fp);
}

void server::handleFileSendToClient(int sd, string fileName) {
    const char* tempFileName = fileName.c_str();
    fp = fopen(tempFileName, "rb");
    if(fp != NULL) {
        fseek(fp, 0, SEEK_END);
        fileSize = ftell(fp);
        rewind(fp);
        send(sd, &fileSize, sizeof(fileSize), 0);
        while (!feof(fp)) {
            int bytesRead = fread(buffer, 1, sizeof(buffer), fp);
            send(sd, buffer, bytesRead, 0);
        }
        fclose(fp);
    }
}

void server::communicateWithClient() {
    creatNewSocket();
    selectClient();
}


