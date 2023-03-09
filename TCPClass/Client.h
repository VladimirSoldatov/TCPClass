#pragma once
#include"WS2tcpip.h"
#include <string>
#include<iostream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
class Client
{
	WSADATA wsData; 
	WORD ver;
	SOCKET connection;
	sockaddr_in  hint;
	string ipAddress;
	int port;
public:
	Client(string _ipAddress, int _port);
	void startConnect();
	~Client();
	
	


};

