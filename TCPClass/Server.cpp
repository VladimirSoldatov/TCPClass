#include "Server.h"
#include<iostream>
using namespace std;
Server::Server(string _ipAddress, int _port) :ipAddress{ _ipAddress }, port{_port}
{

	ver = MAKEWORD(2, 2);
    int winSock = WSAStartup(ver, &wsData);
    if (winSock != 0)
    {
        cerr << "Couldn't inisialize WinSock\n";
        throw exception("Ошибка 403");
    }
    
    //Создание сокета
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);  // https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket
    if (listening == INVALID_SOCKET)
    {
        cerr << "Не могу создать сокет!\n";
        throw exception("Ошибка 404\n");
    }
    else
        cout << "Listen SOCKET\n" << listening << endl;;
    // Связать сокет с портом и IP - адресом
    sockaddr_in  hint; //Сктрутура подсказок по сокету;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;
    bind(listening, (sockaddr*)&hint, sizeof(hint));
    //Указать, что сокет включен для прослушивания порта
    listen(listening, SOMAXCONN);
    cout << "My private server\n";
    
    int clientSize = sizeof(client);
    clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    cout << "Client SOCKET" << clientSocket << endl;;
    ZeroMemory(host, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);
    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
        //https://learn.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-getnameinfo
    {
        cout << host << " connection on port " << service << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connection on port " << service << endl;
    }
    //Закрыть прослушивающий сокет
    closesocket(listening);


}

void Server::startLisening()
{
    char buff[4096];
    while (true)
    {
        ZeroMemory(buff, 4096);
        int bytesRecieved;
        bytesRecieved = recv(clientSocket, buff, 4096, 0);

        if (bytesRecieved == SOCKET_ERROR)
        {
            cerr << "Ошибка получения в recv, выходим\n";
            break;
        }
        if (bytesRecieved == 0)
        {
            cout << "Client отключился\n";
            break;
        }
        //Возвращаем эхо сообщение на клиент

        if (bytesRecieved > 0)
        {
            cout << "Host: " << host << " port: " << service << " send me" << buff << endl;
            if (strcmp(buff, "exit") == 0)
            {
                send(clientSocket, "Good-Bye", (int)strlen("Good-Bye") + 1, 0);
                break;
            }

            if (strcmp(buff, "password") == 0)
            {
                send(clientSocket, "All correct", (int)strlen("All correct") + 1, 0);

            }
            else
            {
                send(clientSocket, buff, (int)strlen(buff) + 1, 0);
            }


        }
    }

}

Server::~Server()
{
    //Закрыть  сокет
    int result = closesocket(clientSocket);

    // Выключаем WinSock
    WSACleanup();
}
