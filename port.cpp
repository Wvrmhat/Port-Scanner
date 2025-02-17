#include <iostream>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib");

using namespace std;

void initWinsock() {
  
    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Failed to initialize WinSock" << endl;
        exit(1);
    }
    cout << "Winsock initialized successfull" << endl;
}

bool isOpen(const string &ip, int port) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock == INVALID_SOCKET) {
        return false; // means port is closed 
    }

    // defines address family port and IP address for connection
    sockaddr_in address;
    address.sin_family = AF_INET;       // specifies protocol family which is IPv4
    address.sin_port = htons(port);     // specifies port, htons converts port to byte representation
    address.sin_addr.s_addr = inet_addr(ip.c_str());    // converts ip address to c style string

    // struct sockaddr contains info about the socket
    int result = connect(sock, (struct sockaddr*)&address, sizeof(address)); // used for connecting to a specified remote IP address
    closesocket(sock);

    // returns result unless we get socket error
    return result != SOCKET_ERROR;
}

// scans a range of ports with targetIP
void scanPorts(const string &ip, int startPort, int endPort) {
    for(int port = startPort; port <= endPort; port++) {
        if(isOpen(ip, port)) {
            cout << "Port " << port << " is open. " << endl;
        }
    }

}

int main() {
    initWinsock();

    string targetIP;
    int startPort;
    int endPort;

    cout << "Enter target IP: ";
    cin >> targetIP;
    cout << "Enter start port: ";
    cin >> startPort;
    cout << "Enter end port ";
    cin >> endPort;

    scanPorts(targetIP, startPort, endPort);

    // if(isOpen(targetIP, port)) {
    //     cout << "Port " << port << "is open" << endl;
    // }
    // else {
    //     cout << "Port " << port << " is closed" << endl;
    // }

    return 0;
}