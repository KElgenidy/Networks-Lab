#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>

#define BUFFER_SIZE 4096

using namespace std;

void sendFile(FILE* file, int Socket) {
    char data[BUFFER_SIZE] = {0};
    size_t bytesRead = 0;

    // Read data from file and send it over the Socket
    while ((bytesRead = fread(data, 1, BUFFER_SIZE, file)) > 0) {
        if (send(Socket, data, bytesRead, 0) == -1) {
            // Handle send error
            cerr << "Error sending data\n";
            exit(1);
        }
    }
}

int main () {
    char* ip = "127.0.0.1";
    int port = 8080;
    int k;

    int Socket;
    struct sockaddr_in serverAddress;
    FILE* file;
    char* filename = "file1.txt";

    Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (Socket == -1) {
        cerr << "Error creating Socket\n";
        exit(1);
    }

    cout << "Created client Socket\n";

    // Set up the server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(ip);
    serverAddress.sin_port = htons(port);

    k = connect(Socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (k == -1) {
        cerr << "Connection failed\n";
        exit(1);
    }

    cout << "Connected to server\n";

    file = fopen(filename, "r");
    if (file == NULL) {
        cerr << "Error opening file\n";
        exit(1);
    }

    sendFile(file, Socket);
    cout << "File sent successfully\n";

    close(Socket);
    cout << "Connection closed\n";

    return 0;
}