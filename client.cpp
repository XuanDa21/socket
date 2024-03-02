#include "client.h"
client::client() :ipAddress("127.0.0.2"), port(8080) {}

void client::setIpAndPort(const char* ip, int port) {
    this -> ipAddress = ip;
    this -> port = port;
}

string client::getMessageFromServer(){ 
    return this -> messageFromServer;
}

bool client::getError() {
    return this -> isError;
}

void client::init() {
    this -> clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(this -> clientSocket < 0) {
        isError = true;
    }
    string temIp = this -> ipAddress;
    memset(&this -> serverAddr, 0, sizeof(this -> serverAddr));
    this -> serverAddr.sin_family = AF_INET;
    this -> serverAddr.sin_port = htons(port);
    if(inet_pton(AF_INET, ipAddress, &this -> serverAddr.sin_addr.s_addr) <= 0) {
        isError = true;
    }
    if (connect(this -> clientSocket, (struct sockaddr*)&this -> serverAddr, sizeof(this -> serverAddr)) == -1) {
        isError = true; 
    }
}

char* client::stringToChar(string str) {
    char* charArr = &str[0];
    return charArr;
}

void client::sendMessage(string message) {
    const char* ch = message.c_str();
    send(this -> clientSocket, ch, strlen(ch), 0) != strlen(ch);
    messageToServer = message;
}

void client::recMessage() {
    char buffer[bufsize];
    memset(buffer, 0, bufsize);
    recv(this -> clientSocket, buffer, bufsize, 0);
    string recive(buffer);
    messageFromServer = recive;
}

vector<string> client::removeDupWord(string str) {
    istringstream ss(str);
    string word; 
    vector<string> strs;
    while (ss >> word)
    {
        strs.push_back(word);
    } 
    return strs;
}

void client::handleFileSendToServer() {
    vector<string> strs;
    strs = removeDupWord(messageToServer);
    if(strs[0].compare("PUTFILE") == 0) {
        const char* fileName = strs[1].c_str();
        fp = fopen(fileName, "rb");
        if(fp != NULL) {
            fseek(fp, 0, SEEK_END);
            fileSize = ftell(fp);
            rewind(fp);
            send(this -> clientSocket, &fileSize, sizeof(fileSize), 0);
            while (!feof(fp)) {
                int bytesRead = fread(buffer, 1, sizeof(buffer), fp);
                send(this -> clientSocket, buffer, bytesRead, 0);
            }
            fclose(fp);
        }
    }
}

void client::sendFileToServer() {
    handleFileSendToServer();
}

void client::receiveContentFile() {
    vector<string> strs;
    strs = removeDupWord(messageToServer);
    if(strs[0].compare("GETFILE") == 0) {
        string str = "client.jpeg";
        const char* fileName = str.c_str();
        recv(this -> clientSocket, &fileSize, sizeof(fileSize), 0);
        fp = fopen(fileName, "wb");
        while (fileSize > 0) {
            int bytesRead = recv(this -> clientSocket, buffer, sizeof(buffer), 0);
            fwrite(buffer, 1, bytesRead, fp);
            fileSize -= bytesRead;
        }
        fclose(fp);
   }
}

void client::communicateWithServer(string message) {
    sendMessage(message);
    recMessage();
    sendFileToServer();
    receiveContentFile();
}


