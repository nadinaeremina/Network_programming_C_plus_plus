// ��� ������������ ���� - ����� ��������� ������ ������ � �� ���������

#pragma once
#include <iostream>
#include <string>
#include "WinSock2.h"
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;
const int MAXSTRLEN = 255;

class Socket
{
protected:
	WSADATA wsaData;
	SOCKET dsock;
	SOCKET dclientsock;
	sockaddr_in addr; // ��-�, ������� ������ �����

public:
	Socket();
	~Socket();

	bool SendData(char*); // ����� ��� �������� ������
	bool RecvData(char*, int); // ����� ��� ��������� ������
	void CloseConnection(); // ��� �������� ����-��
	string SendDataMessage(); // �������� ��������� ������������ 
};

// ���� � �� �� ���������� ����� �������� ��� � ��������� ������, ��� � � ����������
class ServerSocket : public Socket 
{
public:
	void StartHosting(int); // ��-�� ���� 2 ������
	void Listen();
	void Bind(int);
};

class ClientSocket: public Socket
{
public:
	void ConnectToServer(const char*, int); // ��� ����������� � �������
	// ���������, � ������ ������ �������������� �������
};


