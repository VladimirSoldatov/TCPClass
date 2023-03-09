#include <iostream>
using namespace std;
#include"Server.h"
#include"Client.h"

int main()
{
    bool serverStatus;
    cout << "Runas as server?(0/1)\n";
    cin >> serverStatus;

    if (serverStatus)
    {
        try
        {
            Server server("127.0.0.1", 54000);
            server.startLisening();
        }
        catch (exception ex)

        {
            cout << ex.what();
        }
        
    }
    else
    {
        try
        {
            Client client("127.0.0.1", 54000);
            client.startConnect();
        }
        catch(exception ex)
        {
            cout << ex.what();
        }

    }
}
