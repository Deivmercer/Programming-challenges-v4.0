#include <iostream>
#include <winsock.h>
#include <mstcpip.h>
#include <time.h>
#include <string.h>

#pragma comment(lib,"ws2_32.lib")

#define PORT					23365
#define TIMEOUT					10000
#define BUFFER_LENGTH			1024
#define MAX_USERNAME_LENGTH		20
#define MAX_LENGTH				255
#define MAX_IP_LENGTH			39

struct User {

	SOCKET communication_socket_id;
	char username[MAX_USERNAME_LENGTH] = { '\0' };
};

int main()
{
	WSADATA wsa_data;
	SOCKET request_socket_id;
	sockaddr_in server_address, client_address;
	User user;
	//User* user;
	unsigned long start, now;
	int result;
	char message[MAX_LENGTH];
	bool connected;
	if (WSAStartup(0x0202, &wsa_data) != 0)
	{
		std::cout << "Cannot initialize WinSock." << std::endl;
		system("pause");
		return -1;
	}
	request_socket_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (request_socket_id == INVALID_SOCKET)
	{
		std::cout << "Cannot start socket." << std::endl;
		WSACleanup();
		system("pause");
		return -1;
	}
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = 0;
	server_address.sin_port = htons(PORT);
	if (bind(request_socket_id, (struct sockaddr*) & server_address, sizeof(server_address)) == SOCKET_ERROR)
	{
		std::cout << "Cannot bind socket." << std::endl;
		closesocket(request_socket_id);
		WSACleanup();
		system("pause");
		return -1;
	}
	if (listen(request_socket_id, 1) == SOCKET_ERROR)
	{
		std::cout << "Cannot listen to socket." << std::endl;
		closesocket(request_socket_id);
		WSACleanup();
		system("pause");
		return -1;
	}
	std::cout << "Server started and listening to port " << ntohs(server_address.sin_port) << std::endl;
	while (true)
	{
		//user = (User)malloc(sizeof(User));
		std::cout << "Waiting for a new client..." << std::endl;
		user.communication_socket_id = accept(request_socket_id, (struct sockaddr*) & client_address, NULL);
		if (user.communication_socket_id != INVALID_SOCKET)
		{
			std::cout << "Connection to the client estabilished." << std::endl;
			connected = true;
			start = clock();
			now = start;
			while (now - start < TIMEOUT && strlen(user.username) == 0 && connected)
			{
				result = recv(user.communication_socket_id, (char*)user.username, sizeof(user.username), NULL);
				if (result <= 0)
				{
					closesocket(user.communication_socket_id);
					std::cout << "Lost connection to the client." << std::endl;
					connected = false;
				}
				else
				{
					if (result >= MAX_USERNAME_LENGTH)
						result = MAX_USERNAME_LENGTH - 1;
					user.username[result] = '\0';
					result = send(user.communication_socket_id, (char*)now, sizeof(unsigned long), NULL);
					if (result != sizeof(unsigned long))
						result = -1;
				}
			}
			if (strlen(user.username) >= 1)
			{
				std::cout << "Client username: " << user.username << ". Waiting for his first message..." << std::endl;
				while (connected)
				{
					result = recv(user.communication_socket_id, (char*)message, sizeof(message), NULL);
					if (result <= 0)
					{
						closesocket(user.communication_socket_id);
						std::cout << "Lost connection to the client." << std::endl;
						connected = false;
					}
					else
					{
						if (result >= MAX_LENGTH)
							result = MAX_LENGTH - 1;
						message[result] = '\0';
						std::cout << user.username << " wrote:" << message << std::endl;
					}
				}
			}
			else
			{
				std::cout << "The client didn't identified. Closing his connection.";
				closesocket(user.communication_socket_id);
			}
		}
	}
	closesocket(request_socket_id);
	WSACleanup();
	system("pause");
	return 0;
}