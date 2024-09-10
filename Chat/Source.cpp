#include <iostream>
#include "Socket.h"

using namespace std;

int Menu();
void StartServer(int);
void ConnecttoServer(int);

int main()
{
	int nChoice; // ����� ���-�� � ����
	int port = 24242; // ����� ������� �����

	nChoice = Menu();

	switch (nChoice)
	{
	case 1: 
		StartServer(port);
		break;
	case 2:
		ConnecttoServer(port);
		break;
	default:
		break;
	}

	system("pause");
	return 0;
}

int Menu()
{
	int res;

	cout << "1) Start server" << endl;
	cout << "2) Connect to server" << endl;
	cout << "3) Exit" << endl;

	cin >> res;

	// ����������� ���������� ����
	if (res > 0 && res < 4)
		return res;
	else
	{
		cout << "Incorrect input\n";
		return Menu(); // ����������� - ��������
	}
}

void StartServer(int port)
{
	string ipAddress; // ����� ��������� �� ������������ ip, � �������� �� ����� ������������
	char rMsg[MAXSTRLEN]; // ���������, ������� �� ��������
	string sMsg; // ���������, ������� �� ��������

	ServerSocket server;
	cout << "Starting server..." << endl;

	// ��������� ������
	// 1 // TCP 
	//server.StartHosting(port);

	// 2 // UDP
	server.Bind(port);

	while (true)
	{
		cout << "\tWaiting..." << endl;

		// �������� ������
		server.RecvData(rMsg, MAXSTRLEN);

		// �������
		cout << "Received: " << rMsg << endl;
		
	    sMsg = server.SendDataMessage();

		// ���������� ������� ���� ���������
		if (strcmp(rMsg, "end") == 0 || sMsg == "end") // ���� �� �������� ��� ��������� ��������� "end"
			break;

		// ������ ����� ������ ��������� ������
	}
}

void ConnecttoServer(int port)
{
	string ipAddress; // ����� ��������� �� ������������ ip, � �������� �� ����� ������������
	char rMsg[MAXSTRLEN]; // ���������, ������� �� ��������
	string sMsg; // ���������, ������� �� ��������

	// ����������� ip-�����
	cout << "Enter IP-adress: ";

	// ���������
	cin >> ipAddress;

	// ������� ����� ��� �������
	ClientSocket client;

	// ����������
	client.ConnectToServer(ipAddress.c_str(),  port); // 'ipAddress' ��������������� � 'c_str'

	while (true)
	{
		sMsg = client.SendDataMessage();
		cout << "\tWaiting..." << endl;

		// ������ �������� �����
		client.RecvData(rMsg, MAXSTRLEN);
		cout << "Received: " << rMsg << endl;

		if (strcmp(rMsg, "end") == 0 || sMsg == "end") // ���� �� �������� ��� ��������� ��������� "end"
			break;
	}

	client.CloseConnection(); // ������ ��������� ����-��
}
