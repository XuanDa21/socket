#include "viewServer.h"

viewServer::viewServer() {}

void viewServer::printErrServer() {
    if(ser.getError()) {
        cout << "Error!" << endl;
    }
    else {
        cout << "Setup is done!" << endl;
    }
}

void viewServer::setInit() {
    string ip;
    int port;
    cout << "Ip: ";
    getline(cin, ip);
    cout << "Port: ";
    cin >> port;
    ser.setIpAndPort(ip.c_str(), port);
}

void viewServer::startServer() {
    setInit();
    ser.init();
    printErrServer();
    cout << "Waiting for connections ..." << endl;
    while(true) {
        ser.communicateWithClient();
    }
}

