#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <arpa/inet.h>

#define SIZE 1024

using namespace std;

void writeFile(int dataSocket) {
    int n;
    ofstream outputFile("file1.txt", ios::binary);
    char buffer[SIZE];

    if (!outputFile) {
        cerr << "Error opening file\n";
        exit(1);
    }

    while (1) {
        n = recv(dataSocket, buffer, SIZE, 0);
        if (n <= 0) {
            break;
        }

        outputFile.write(buffer, n);
        memset(buffer, 0, SIZE);
    }
}

int main() {
    char *ip = "127.0.0.1";
    int port = 8080;
    int k;

    int controlSocket, dataSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize;

    // Create control socket
    controlSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (controlSocket == -1) {
        cerr << "Error creating socket\n";
        exit(1);
    }

    cout << "Socket created\n";

    // set up the control server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    serverAddr.sin_port = htons(port);

    // bind the socket to the control address
    k = bind(controlSocket, (sockaddr *)&serverAddr, sizeof(serverAddr));
    if (k == -1) {
        cerr << "Binding failed\n";
        exit(1);
    }

    cout << "Binding successful\n";

    // listen for incoming connections
    k = listen(controlSocket, 1);
    if (k == -1) {
        cerr << "Listen Failed\n";
        exit(1);
    }

    cout << "Listening for connections\n";

    // accept incoming connection
    clientAddrSize = sizeof(clientAddr);
    dataSocket = accept(controlSocket, (sockaddr *)&clientAddr, &clientAddrSize);
    if (dataSocket == -1) {
        cerr << "Error accepting connection\n";
        exit(1);
    }

    cout << "Connection accepted\n";

    writeFile(dataSocket);
    cout << "File received\n";
}