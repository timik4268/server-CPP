#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

#include <WinSock2.h>
#include "Custom_text.h"
#include <iostream>

using namespace std;

SOCKET Connection;
char nickname[256];


void Client_Handler()
{

	char msg[256];
	char temp_nickname[256];

	while (true)
	{
		recv(Connection, temp_nickname, sizeof(temp_nickname), NULL);
		recv(Connection, msg, sizeof(msg), NULL);
		Custom_text::SetColor(Custom_text::LightCyan, Custom_text::Black);
		cout << "[" << temp_nickname << "]: ";
		Custom_text::SetColor(Custom_text::Green, Custom_text::Black);
		cout << msg << endl;
	}
}

int main()
{
	WSAData wsaData;
	WORD DLLVersionLib = MAKEWORD(1, 2); // загружаемая версия библиотеки
	if (WSAStartup(DLLVersionLib, &wsaData) != 0)
	{
		cout << "Error!" << endl;
		exit(1);
	}

	SOCKADDR_IN addr_struct;
	int sizeOFaddr = sizeof(addr_struct);
	addr_struct.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr_struct.sin_port = htons(1111);
	addr_struct.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr_struct, sizeof(addr_struct)) != 0)
	{
		cout << "Error: failed connect to server! Try to again later  or start the new server." << endl;
		return 1;
	}

	cout << "Connected!)" << endl;
	cout << "Create your nickname: ";
	cin.getline(nickname, sizeof(nickname));

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

	char msg[256];
	while (true)
	{
		cin.getline(msg, sizeof(msg));
		send(Connection, nickname, sizeof(nickname), NULL);
		send(Connection, msg, sizeof(msg), NULL);
		Sleep(10);
	}

	return 0;
}
