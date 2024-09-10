// это заголовочный файл - здесь прописаны только классы и их заголовки

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
	sockaddr_in addr; // об-т, который хранит адрес

public:
	Socket();
	~Socket();

	bool SendData(char*); // метод для отправки данных
	bool RecvData(char*, int); // метод для получения данных
	void CloseConnection(); // для закрытия соед-ия
	string SendDataMessage(); // отправка сообщения пользователю 
};

// одно и то же приложение может работать как в серверном режиме, так и в клиентском
class ServerSocket : public Socket 
{
public:
	void StartHosting(int); // об-ет след 2 метода
	void Listen();
	void Bind(int);
};

class ClientSocket: public Socket
{
public:
	void ConnectToServer(const char*, int); // для подключения к серверу
	// указываем, к какому должны присоединиться серверу
};


