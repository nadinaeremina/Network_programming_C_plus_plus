// ����� - ����������

#include "Socket.h" // ���������� ������������ ����

Socket::Socket()
{
	// ��������� wsaData // ���������� � ���������� 'NO_ERROR'
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) // ���� �� ����� �� ������
	{
		// &wsaData ���������� �� ������, ������ ��� � �-��� ��� ��������� (������ ��� �������� �����)
		// 'WSAStartup' ���������� � '&wsaData' �����-�� ������, ���� ��� ������� ������������
		// �� ��� ������ ��, ��� �������, �� ���-��, ������ ��� �� ���-�� �����

		cout << "WSAStartup Error\n";

		system("pause"); // ������ ������� �� �����
		WSACleanup(); // ������� ���-��
		exit(10); // ������� � ������, ������� ���������� ������, ���� 'WSAStartup' ��������� �� �������
	}

	// ���� ��� ������������ - ������� ����� // ��� 1 ������������ ����� 'TCP' // ��� ���������� UDP
	// 1 // TCP - 1  
	// dsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// �-��� 'socket' ���������� ��� ���������� ������ 'SOCKET', ������� �� ������ � ���������� 'dsock'

	// 2 // UDP - 2
	dsock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (dsock == INVALID_SOCKET) // ����������� �� ������� // ���� � ������ ����� 0
	{
		cout << "Socket error on creation\n";

		system("pause");
		WSACleanup();
		exit(11); // 'exit' � ������� 11
	}

	// ��������� �������
	// ���� � ������� 2 - � ����� ��������� �� ������ - �� ������� �� ��������� - ������ ������ ���������� ���
	int val = 120000;
	setsockopt(dsock, SOL_SOCKET, SO_RCVTIMEO, (char*)&val, sizeof(int));
}

Socket::~Socket()
{
	WSACleanup(); // ������� ��� ����� // � �# - ��� 'Shutdown' � 'Close'
	// ��������� �������� ������ � ���� ����� � ��������� ���
}

bool Socket::SendData(char* buffer) // ����� ��� �������� ������
{
	send(dsock, buffer, strlen(buffer), 0); // �����, ����� ������ � �������� �� ������
	return true;
}

bool Socket::RecvData(char* buffer, int size) // ����� ��� ��������� ������
{
	int i = recv(dsock, buffer, size, 0); 
	// 'buffer' - ��� ��������� (���� �� �������� ������ �-��� - �� �������� � ������� �-���)
	buffer[i] = '\0';
	return true;
}

void Socket::CloseConnection() // ��� �������� ����-�� (������)
{
	closesocket(dsock);
}

string Socket::SendDataMessage() // �������� ��������� ������������ 
{
	// ������� ���������
	char msg[MAXSTRLEN];

	// ���������� ���������� ������������ (��, ��� �� ������)
	cin.ignore();

	// ����������� � ������������ ���������
	cout << "Your message: ";

	// ���������� ��� // ���������
	cin.get(msg, MAXSTRLEN);

	// ��������� �� ������������ � �������� �� ���� �������/�������
	SendData(msg);

	return msg;
}

void ServerSocket::StartHosting(int port) // ��� ����������� � �������
{
	Bind(port);
	Listen();
}

void ServerSocket::Listen() // ������������� �� ����� ������
{
	cout << "Waiting for client..." << endl;
	
	// ����� ���������� ������������� // ������ ���-�� - ������� �� ������� �������� (1)
	// ���� �� ����� ������������ �������� "TCP' - �� ������ ������ ������� �� �������
	// ��� ��������� ����������� ����-�� ����� ������ � �������
	if (listen(dsock, 1) == SOCKET_ERROR) // ��������� ������ �� ������
	{
		cout << "Listening arror\n";
		system("pause");
		WSACleanup();
		ERROR(15);
	}

	// � ��������� ������ - �������� ����� ������� // ��������� ���-�� // ���� ����������� ������������
	// ��� ������ �� ����������� - �� ������� ����� ����� ������������
	dclientsock = accept(dsock, NULL, NULL);

	while (dclientsock == SOCKET_ERROR) // ���� ��� ������
	{
		// �������� �������� �����
		dclientsock = accept(dsock, NULL, NULL);
	}

	// �������� 'dsock' � ������� �� ����� �������
	// ����� ����� � �-��� ��� �������� ������ �� ������������ �������� ��� ������
	dsock = dclientsock;
}

void ServerSocket::Bind(int port)
{
	addr.sin_family = AF_INET;

	// �� ������� ������� �� ������������ � ���������� �����
	
	inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
	addr.sin_port = htons(port);
	// � 'addr' �������������� ���-��� �� ������, �� ��� �� ����� ������� �����

	cout << "Binding to port:" << port << endl;

	if (bind(dsock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) // ���� ������� �� ������
	{
		cout << "Binding port error\n";
		system("pause");
		WSACleanup();
		exit(14);
	}
}
 
void ClientSocket::ConnectToServer(const char* ip, int port)
{
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &addr.sin_addr);
	addr.sin_port = htons(port);

	if (connect(dsock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) // ���� �� ������ �����������
	{
		cout << "Failed to connect to server error\n";
		system("pause");
		WSACleanup();
		exit(13);
	}
}
