#pragma once
#include"WS2tcpip.h"
#include <string>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
class Server
{
	WSADATA wsData;
	WORD ver;
	SOCKET listening;
	SOCKET clientSocket;
	sockaddr_in  hint;
	sockaddr_in client;
	string ipAddress;
	int port;
	char host[NI_MAXHOST];
	char service[NI_MAXHOST]; 
public:
	Server(string _ipAddress, int _port);

	void startLisening();
	~Server();

};

