#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

void main()
{
	// ������ ��������� ����� ������ 
	const int MAXSTRLEN = 255;

	// ���-��� �� ������������� ������� ����� ���-�� ������� - ������� ������� ���-��
	WSADATA wsaData;

	// ����� ����� ��������� ���������� ������ �������
	SOCKET dsock;

	// ��� �������� ���������� ������ � ����� ������������� ������
	sockaddr_in addr;

	// �������� ���-��� � windows-socket, ������� �� ���-�� // �������������� win-socket
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	// ���� �� �������� ��������� - ������ ����� �� �� �������� � �-��� ������, ������� ����� �����
	// � �� �������� � ��� ���-��, � ������� � ��� �-��� ������� ��� ������

	// �������� ���������� ������ ������� - ������� �����
	dsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// ��� ������������ ������ 'Sream' - TCP

	// ����������� �����
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr); // �� ������ ������� ��� sin_addr
	// � sin_addr  ����� ������� ����� ip-����� 
	// sin_addr ��������� � ��������� in_addr - ����-�� ���� ����� ������� ������ ���� ��� ������ - in_addr
	// ip �������� �� ����� ������ - � � ����� ���� ������, ������� ����� ���� �������

	// ������ ��������� sin_port
	addr.sin_port = htons(20000);

	// ������ 'accept' ����� ����� 'connect'
	connect(dsock, (SOCKADDR*)&addr, sizeof(addr));

	// ����� �������� ��������� ������� - ������� �����
	char buf[MAXSTRLEN];
	const char* text = "Hello, server!";
	cout << "\nPress Enter to send hello to the server\n";

	// ���������� ������� ENTER
	cin.get();

	// �� ������������ ����� ���������� ���������
	send(dsock, text, strlen(text), 0);

	// ������ ������ ������ �������� ��������� // ���������� ������ ����������� ���������
	int i = recv(dsock, buf, MAXSTRLEN, 0);

	// ���������� ������ ��������� ������
	buf[i] = '\0';

	cout << buf << endl;

	// ��������� �����
	closesocket(dsock);

	WSACleanup();
	system("pause");
}
