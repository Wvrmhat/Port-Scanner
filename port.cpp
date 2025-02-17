#include <iostream>
#include <WinSock2.h>
#include <vector>
#include <thread>
#include <string>
#include <mutex>    // makes sure multiple threads will not print to console at same time
#include <iomanip> 
#pragma comment(lib, "ws2_32.lib");

using namespace std;
mutex printMutex;

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
    address.sin_port = htons(port);     // specifies port, htons converts port to binary representation
    address.sin_addr.s_addr = inet_addr(ip.c_str());    // converts ip address to c style string

    // struct sockaddr contains info about the socket
    int result = connect(sock, (struct sockaddr*)&address, sizeof(address)); // used for connecting to a specified remote IP address
    closesocket(sock);

    // returns result unless we get socket error
    return result != SOCKET_ERROR;
}

string getServiceName(int port) {
    struct servent *service = getservbyport(htons(port), "tcp");
    // if we can retrieve a service name, then we return service name, otherwise we save it as unkown
    return service ? service ->s_name: "unknown";   
}

// scans a range of ports with targetIP
void scanPorts(const string &ip, int startPort, int endPort) {
    for(int port = startPort; port <= endPort; port++) {
        if(isOpen(ip, port)) {
            string serviceName = getServiceName(port);
            lock_guard<mutex> lock(printMutex); // prevents threads from 

            // sets width to 5 chars and ouputs port and service name
            cout << "\033[32m]"     // makes colour for output 
            << "Port " << setw(5) << port
            << "| Protocol: TCP"
            << "| Service: " << "serviceName"
            << endl;
        }
    }


}


int main() {
    initWinsock();

    string targetIP;
    int startPort, endPort, numOfThreads;

    cout << "Enter target IP: ";
    cin >> targetIP;
    cout << "Enter start port: ";
    cin >> startPort;
    cout << "Enter end port ";
    cin >> endPort;
    // including threads
    cout << "Enter number of Threads: ";
    cin >> numOfThreads;
    // calculates total number of ports in  range, and divides by number of threads (e.g if we have 100 ports and use 50 threads, it means each thread will take 2 ports to scan)
    int range = (endPort - startPort + 1) / numOfThreads;
    vector <thread> threads;
    
    // for iterating through the threads, which uses starting port and multiplies by range
    for(int i = 0; i < numOfThreads; i++) {
        int rangeStart = startPort + i * range; // starting point for i thread
        
        // calculates endPort, checks if current thread is last, where i is total number of threads. If enPort is last then rangeEnd = endport, otherwise it equals rangestart. 
        int rangeEnd = (i == numOfThreads -1) ? endPort : rangeStart + range -1; 

        threads.emplace_back(scanPorts, targetIP, rangeStart, rangeEnd); // creates the thread
    }
    // waits until current thread finishes to start next thread
    for(auto &t : threads) {
        t.join();
    }

    cout << "Scan completed.";



    return 0;
}