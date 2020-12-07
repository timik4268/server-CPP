#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

#define log_

#include <WinSock2.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

char global_temp_nickname[256];

SOCKET Connections[100];
int counter = 0;

void Client_Handler(int index)
{
	char nickname[256];
	char msg[256];
	while (true)
	{
		recv(Connections[index], nickname, sizeof(nickname), NULL);
		recv(Connections[index], msg, sizeof(msg), NULL);
		for (int i = 0; i < counter; i++)
		{
			if (i == index)
			{
				continue;
			}
			else
			{
				send(Connections[i], nickname, sizeof(nickname), NULL);
				send(Connections[i], msg, sizeof(msg), NULL);
			}
		}
	}
}

int main()
{
#ifdef _log
	cout << "[log] Server start" << endl;
#endif 
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(1, 2); // загружаемая версия библиотеки
	if (WSAStartup(DLLVersion, &wsaData) != 0)
	{
		cout << "[log] Error!" << endl;
		exit(1);
	}
	else
	{
		#ifdef _log
			cout << "[log] Server started" << endl;
		#endif 

	}

	SOCKADDR_IN addr;
	int sizeOFaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	for (int i = 0; i < 100; i++)
	{
		SOCKET newConnection;
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeOFaddr);

		if (newConnection == 0)
		{
			#ifdef log
				cout << "[log] Error! #2" << endl;
			#endif // log

		}
		else
		{
			#ifdef log
				cout << "[log] Client connected." << endl;
			#endif // log

			Connections[i] = newConnection;
			counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Client_Handler, (LPVOID)(i), NULL, NULL);
		}
	}
	return 0;
}
