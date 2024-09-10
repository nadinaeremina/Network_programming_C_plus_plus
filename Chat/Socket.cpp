// здесь - реализация

#include "Socket.h" // подключаем заголовочный файл

Socket::Socket()
{
	// получение wsaData // сравниваем с константой 'NO_ERROR'
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) // если не равен не ошибке
	{
		// &wsaData передается по ссылке, потому что в ф-ции она изменится (потому как передаем адрес)
		// 'WSAStartup' записывает в '&wsaData' какие-то данные, если все пройдет благополучно
		// но эти данные мы, как правило, не исп-ем, потому что мы исп-ем сокет

		cout << "WSAStartup Error\n";

		system("pause"); // ставим консоль на паузу
		WSACleanup(); // очищаем сис-му
		exit(10); // выходим с цифрой, которая обозначает ошибку, если 'WSAStartup' выполнить не удалось
	}

	// если все благополучно - создаем сокет // для 1 пользователя можно 'TCP' // для нескольких UDP
	// 1 // TCP - 1  
	// dsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// ф-ция 'socket' возвращает нам дискриптор класса 'SOCKET', который мы кладем в переменную 'dsock'

	// 2 // UDP - 2
	dsock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (dsock == INVALID_SOCKET) // подключение не удалось // если у сокета номер 0
	{
		cout << "Socket error on creation\n";

		system("pause");
		WSACleanup();
		exit(11); // 'exit' с ошибкой 11
	}

	// настройка таймера
	// Если в течении 2 - х минут сообщение от какого - то клиента не поступило - сервер должен отсоединит его
	int val = 120000;
	setsockopt(dsock, SOL_SOCKET, SO_RCVTIMEO, (char*)&val, sizeof(int));
}

Socket::~Socket()
{
	WSACleanup(); // очищаем наш сокет // в С# - это 'Shutdown' и 'Close'
	// блокируем передачу данных в этот сокет и закрываем его
}

bool Socket::SendData(char* buffer) // метод для отправки данных
{
	send(dsock, buffer, strlen(buffer), 0); // буфер, длина буфера и смещение по буферу
	return true;
}

bool Socket::RecvData(char* buffer, int size) // метод для получения данных
{
	int i = recv(dsock, buffer, size, 0); 
	// 'buffer' - это указатель (если он меняется внутри ф-ции - он меняется и снаружи ф-ции)
	buffer[i] = '\0';
	return true;
}

void Socket::CloseConnection() // для закрытия соед-ия (сокета)
{
	closesocket(dsock);
}

string Socket::SendDataMessage() // отправка сообщения пользователю 
{
	// создаем сообщение
	char msg[MAXSTRLEN];

	// сбрасываем последнего пользователя (то, что он вводил)
	cin.ignore();

	// запрашиваем у пользователя сообщение
	cout << "Your message: ";

	// записываем его // считываем
	cin.get(msg, MAXSTRLEN);

	// считываем от пользователя и передаем по сети серверу/клиенту
	SendData(msg);

	return msg;
}

void ServerSocket::StartHosting(int port) // для подключения к серверу
{
	Bind(port);
	Listen();
}

void ServerSocket::Listen() // прослушивание по опред адресу
{
	cout << "Waiting for client..." << endl;
	
	// далее включается прослушивание // вторым пар-ом - сколько мы ожидаем клиентов (1)
	// если мы будем использовать протокол "TCP' - то больше одного клиента не получим
	// все остальные последующие соед-ия будут стоять в очереди
	if (listen(dsock, 1) == SOCKET_ERROR) // произошла ошибка на сокете
	{
		cout << "Listening arror\n";
		system("pause");
		WSACleanup();
		ERROR(15);
	}

	// в противном случае - получаем сокет клиента // принимаем сед-ие // ждем подключения пользователя
	// как только он подключится - мы получим сокет этого пользователя
	dclientsock = accept(dsock, NULL, NULL);

	while (dclientsock == SOCKET_ERROR) // если нет ошибки
	{
		// пытаемся получить новый
		dclientsock = accept(dsock, NULL, NULL);
	}

	// заменяем 'dsock' у сервера на сокет клиента
	// чтобы потом в ф-ции для отправки данных не переписывать отдельно под сервер
	dsock = dclientsock;
}

void ServerSocket::Bind(int port)
{
	addr.sin_family = AF_INET;

	// на стороне сервера мы подключаемся к дефолтному порту
	
	inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
	addr.sin_port = htons(port);
	// в 'addr' сформировалась инф-ция об адресе, по кот мы будем биндить сокет

	cout << "Binding to port:" << port << endl;

	if (bind(dsock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) // если биндинг не удался
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

	if (connect(dsock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) // если не смогли поключиться
	{
		cout << "Failed to connect to server error\n";
		system("pause");
		WSACleanup();
		exit(13);
	}
}
