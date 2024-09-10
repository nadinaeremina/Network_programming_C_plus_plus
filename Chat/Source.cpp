#include <iostream>
#include "Socket.h"

using namespace std;

int Menu();
void StartServer(int);
void ConnecttoServer(int);

int main()
{
	int nChoice; // выбор пол-ля в меню
	int port = 24242; // можно указать любой

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

	// запрашиваем правильный ввод
	if (res > 0 && res < 4)
		return res;
	else
	{
		cout << "Incorrect input\n";
		return Menu(); // зацикливаем - рекурсия
	}
}

void StartServer(int port)
{
	string ipAddress; // будем считывать от пользователя ip, к которому мы будем подключаться
	char rMsg[MAXSTRLEN]; // сообщение, которое мы получаем
	string sMsg; // сообщение, которое мы посылаем

	ServerSocket server;
	cout << "Starting server..." << endl;

	// запускаем сервер
	// 1 // TCP 
	//server.StartHosting(port);

	// 2 // UDP
	server.Bind(port);

	while (true)
	{
		cout << "\tWaiting..." << endl;

		// получаем данные
		server.RecvData(rMsg, MAXSTRLEN);

		// выводим
		cout << "Received: " << rMsg << endl;
		
	    sMsg = server.SendDataMessage();

		// отправляем обратно наше сообшение
		if (strcmp(rMsg, "end") == 0 || sMsg == "end") // если мы получили или отправили сообщение "end"
			break;

		// сервер может дальше считывать данные
	}
}

void ConnecttoServer(int port)
{
	string ipAddress; // будем считывать от пользователя ip, к которому мы будем подключаться
	char rMsg[MAXSTRLEN]; // сообщение, которое мы получаем
	string sMsg; // сообщение, которое мы посылаем

	// запрашиваем ip-адрес
	cout << "Enter IP-adress: ";

	// принимаем
	cin >> ipAddress;

	// создаем сокет для клиента
	ClientSocket client;

	// подключаем
	client.ConnectToServer(ipAddress.c_str(),  port); // 'ipAddress' преобразовываем в 'c_str'

	while (true)
	{
		sMsg = client.SendDataMessage();
		cout << "\tWaiting..." << endl;

		// клиент получает ответ
		client.RecvData(rMsg, MAXSTRLEN);
		cout << "Received: " << rMsg << endl;

		if (strcmp(rMsg, "end") == 0 || sMsg == "end") // если мы получили или отправили сообщение "end"
			break;
	}

	client.CloseConnection(); // клиент закрывает соед-ие
}
