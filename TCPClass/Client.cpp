#include "Client.h"
Client::Client(string _ipAddress, int _port): ipAddress{ _ipAddress }, port{ _port }
	{
		setlocale(LC_ALL, "RUSSIAN");
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		//Инициализация winSock
		ver = MAKEWORD(2, 2); //Собщим версию сокета https://learn.microsoft.com/en-us/windows/win32/winsock/initializing-winsock
		int wsOK = WSAStartup(ver, &wsData);// Передаем структуру данных и версию сокета
		if (wsOK != 0)
		{
			cerr << "Не получилось инициализиорвать WinSock!\n";
			throw exception("Ошибка 403");
		}

		//Создание сокета
		connection = socket(AF_INET, SOCK_STREAM, 0);  // https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket
		if (connection == INVALID_SOCKET)
		{
			cerr << "Не могу создать сокет!\n";
			throw exception("Ошибка 404");
		}
		else
			cout << "Connection SOCKET " << connection << endl;

		// Связать сокет спортом и IP - адресом
		 //Сктрутура подсказок по сокету;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port); //Привязываем порт к сокету
		inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
		int connResult = connect(connection, (sockaddr*)&hint, sizeof(hint));
		if (connResult == SOCKET_ERROR)
		{
			cerr << "No connection " << WSAGetLastError() << "\n";
			throw exception("Ошибка 503");
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
