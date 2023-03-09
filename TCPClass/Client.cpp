#include "Client.h"
Client::Client(string _ipAddress, int _port): ipAddress{ _ipAddress }, port{ _port }
	{
		setlocale(LC_ALL, "RUSSIAN");
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		//������������� winSock
		ver = MAKEWORD(2, 2); //������ ������ ������ https://learn.microsoft.com/en-us/windows/win32/winsock/initializing-winsock
		int wsOK = WSAStartup(ver, &wsData);// �������� ��������� ������ � ������ ������
		if (wsOK != 0)
		{
			cerr << "�� ���������� ���������������� WinSock!\n";
			throw exception("������ 403");
		}

		//�������� ������
		connection = socket(AF_INET, SOCK_STREAM, 0);  // https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket
		if (connection == INVALID_SOCKET)
		{
			cerr << "�� ���� ������� �����!\n";
			throw exception("������ 404");
		}
		else
			cout << "Connection SOCKET " << connection << endl;

		// ������� ����� ������� � IP - �������
		 //��������� ��������� �� ������;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port); //����������� ���� � ������
		inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
		int connResult = connect(connection, (sockaddr*)&hint, sizeof(hint));
		if (connResult == SOCKET_ERROR)
		{
			cerr << "No connection " << WSAGetLastError() << "\n";
			throw exception("������ 503");
		}
}


void Client::startConnect()
{
	char buff[4096];
	string userInput;
	do
	{
		cout << "> ";
		cin.clear();
		cin >> userInput;
		if (userInput.size() > 0)
		{
			int sendResult = send(connection, userInput.c_str(), (int)userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				ZeroMemory(buff, 4096);
				int bytesRecieved = recv(connection, buff, 4096, 0);
				if (bytesRecieved > 0)
				{
					cout << "SERVER " << string(buff, 0, bytesRecieved);
				}
			}
		}

	} while (userInput.size() > 0);

}

Client::~Client()
{
	closesocket(connection);
	WSACleanup();
}
