#include "viewClient.h"
viewClient::viewClient() {}

string viewClient::setMessage() {
    getline(cin, this -> message);
    return message;
}

string viewClient::getMessage() {
    return this -> message;
}

void viewClient::setInit() {
    string ip;
    int port;
    cout << "Ip: ";
    getline(cin, ip);
    cout << "Port: ";
    cin >> port;
    cli.setIpAndPort(ip.c_str(), port);
}

void viewClient::printErrClient() {
    if(cli.getError()) {
        cout << "Can't connect to server" << endl;
    }
    else {
        cout << "Connected!" << endl;
    }
}

void viewClient::startClient() {
    setInit();
    cli.init();
    printErrClient();
    cin.ignore(1000,'\n');
    while(true) {
        cout << "Enter command: ";
        setMessage();
        cli.communicateWithServer(getMessage());
        cout << cli.getMessageFromServer() << endl;
    }
}


