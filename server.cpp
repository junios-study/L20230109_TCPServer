#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

using namespace std;

int main()
{
	srand((unsigned int)time(nullptr));

	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (Result != 0)
	{
		cout << "Winsock dll error : " << GetLastError() << endl;
		exit(-1);
	}

	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "INVALID_SOCKET : " << GetLastError() << endl;
		exit(-1);
	}

	SOCKADDR_IN ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	//ZeroMemory(&ListenSockAddr, sizeof(ListenSockAddr));
	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.s_addr = INADDR_ANY;
	ListenSockAddr.sin_port = htons(40211);

	Result = bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));
	if (Result == SOCKET_ERROR)
	{
		cout << "can't bind : " << GetLastError() << endl;
		exit(-1);
	}

	Result = listen(ListenSocket, 5);
	if (Result == SOCKET_ERROR)
	{
		cout << "can't bind : " << GetLastError() << endl;
		exit(-1);
	}


	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int ClientSocketAddrLength = sizeof(ClientSockAddr);
	SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSocketAddrLength);
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "accept fail : " << GetLastError() << endl;
		exit(-1);
	}

	while (true)
	{
		int FirstNumber = rand() % 10000;
		int SecondNumber = rand() % 9999 + 1;
		int OperatorIndex = rand() % 5;
		char Operator[5] = { '+', '-', '*', '/', '%' };

		char Buffer[1024] = { 0, };
		sprintf_s(Buffer, 1024, "%d%c%d", FirstNumber, Operator[OperatorIndex], SecondNumber);
		//		sprintf(Buffer, "%d%c%d", FirstNumber, Operator[OperatorIndex], SecondNumber);

		int SentByte = send(ClientSocket, Buffer, (int)(strlen(Buffer) + 1), 0);
		if (SentByte < 0)
		{
			cout << "Error : " << GetLastError() << endl;
			continue;
		}
		else if (SentByte == 0)
		{
			cout << "Disconnected : " << GetLastError() << endl;
			continue;
		}
		else
		{
			cout << "Sent byte : " << SentByte << ", " << Buffer << endl;
		}

		char RecvBuffer[1024] = { 0, };
		int RecvByte = recv(ClientSocket, RecvBuffer, sizeof(RecvBuffer), 0);
		if (RecvByte < 0)
		{
			cout << "Error : " << GetLastError() << endl;
			continue;
		}
		else if (RecvByte == 0)
		{
			cout << "Disconnected : " << GetLastError() << endl;
			continue;
		}
		else
		{
			cout << "Recv byte : " << RecvByte << ", " << RecvBuffer << endl;
		}
	}
	closesocket(ClientSocket);
	closesocket(ListenSocket);

	WSACleanup();

	return 0;
}