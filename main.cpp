#pragma comment(lib,"Ws2_32.lib")                           //подключение библиотеки с сокетами
#include "WinSock2.h"                                       //подключение сами сокетов
#include <iostream>											//для вывода данных в консоль
#include "WS2tcpip.h"                                       //дает необходимую функции для подключения, получения информации о сервере

SOCKET Connect;                                             //принимает подключаемых пользователей
SOCKET* Connections;										//хранит всех пользователей (массив)
SOCKET Listen;												//для подключения

int ClientCount = 0;										//количество подключаемых пользователей

void SendMessageToClient(int ID)							//функция рассылки сообщения всем пользователям
{
	char* buffer = new char[1024];							//буфер
	for (;; Sleep(75))										//очередной цикл без конца и края
	{
		memset(buffer, 0, sizeof(buffer));					//чистим буфер
		if (recv(Connections[ID], buffer, 1024, NULL))		//получаем сообщение сохраняем в буфер
		{
			printf(buffer);									//выводим сообщение
			printf("\n");									//а это вообще надо комментировать?
			for (int i = 0; i <= ClientCount; i++)			//теперь передаем всем пользователям сообщение
			{
				send(Connections[i], buffer, strlen(buffer), NULL);
			}
		}
	}
	delete buffer;
}

int main()
{
	setlocale(LC_ALL, "Rus");
	WSAData data;											//создаем переменную  
	WORD version = MAKEWORD(2, 2);							//указываем версию сокетов

	int res = WSAStartup(version, &data);					//инициализируем сокеты
	if (res != 0)											
	{
		return 0;
	}

	struct addrinfo hints;									//переменная для работы с сокетом
	struct addrinfo * result;


	Connections = (SOCKET*)calloc(64, sizeof(SOCKET));		//инициализруем массив

	ZeroMemory(&hints, sizeof(hints));						//очищаем hinst

	hints.ai_family = AF_INET;								//
	hints.ai_flags = AI_PASSIVE;							//ну..как бы..флаг...
	hints.ai_socktype = SOCK_STREAM;						//тип сокета
	hints.ai_protocol = IPPROTO_TCP;						//тип протокола

	getaddrinfo(NULL, "7770", &hints, &result);				//получение информации о хосте

	Listen = socket(result->ai_family, result->ai_socktype, result->ai_protocol);   //настраиваем listen
	bind(Listen, result->ai_addr, result->ai_addrlen);								//объявляем сервер
	listen(Listen, SOMAXCONN);								//даем сокет для подключения

	freeaddrinfo(result);									//просто очищаем, сокеты уже настроены

	printf("Start server...");								//подтверждение начала работы сервера
	char m_connect[] = "Connect...;;;5";					//сщщбщение о подтверждении подключения
	for (;; Sleep(100))										//бесконечный цикл
	{
		if (Connect = accept(Listen, NULL, NULL))			//проверка подключения
		{
			printf("Client connect...\n");					//сообщение о подключении
			Connections[ClientCount] = Connect;				//количество подключаемых пользователей
			send(Connections[ClientCount], m_connect, strlen(m_connect), NULL); //отправляем клиенты сообщение о подключении к сереверу
			ClientCount++;									//ну...счетчик как бы, лол
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SendMessageToClient, (LPVOID)(ClientCount - 1), NULL, NULL); //создаем поток в котором запущена функция
		}
	}
	return 1;
}