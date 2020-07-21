#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS		1
#define _CRT_SECURE_NO_WARNINGS				1

#include <iostream>
#include <string.h>
#include <process.h>
#include <WinSock2.h>
#include <time.h>

#pragma comment(lib,"ws2_32.lib")

#define TIMEOUT					10000
#define BUFFER_LENGTH			1024
#define MAX_USERNAME_LENGTH		20
#define MAX_LENGTH				255
#define MAX_IP_LENGTH			39

static unsigned long IP_to_bin(char ip_add[]);

SOCKET socket_id;
unsigned short port_number = 23365;

int main(int argc, char* argv[])
{
	WSADATA wsa_data;
	sockaddr_in socket_address;
	hostent* host_entry;
	unsigned long start, now, binary_address;
	int result;
	char buffer[BUFFER_LENGTH] = "REQ\r\n", hostname[MAX_LENGTH], * LocalIP, message[MAX_LENGTH], ip[MAX_IP_LENGTH], username[MAX_USERNAME_LENGTH];
	unsigned int* num = (unsigned int*)buffer;
	bool connected = false;
	if (argc < 2 || strlen(argv[1]) > MAX_IP_LENGTH)
	{
		do {
			std::cout << "Invalid or missing IP address. Insert the IP address you want to connect to: " << std::endl;
			std::cin.getline(ip, MAX_IP_LENGTH - 1);
		} while (strlen(ip) > MAX_IP_LENGTH);
	}
	else
		strcpy_s(ip, argv[1]);
	binary_address = IP_to_bin(ip);
	if (argc < 3 || strlen(argv[2]) > MAX_USERNAME_LENGTH)
	{
		do {
			std::cout << "Invalid or missing username. Insert your username: ";
			std::cin.getline(username, MAX_USERNAME_LENGTH - 1);
		} while (strlen(username) > MAX_USERNAME_LENGTH);
	}
	else
		strcpy_s(username, argv[2]);
	if (WSAStartup(0x0202, &wsa_data) != 0)
	{
		std::cout << "Cannot initialize WinSock." << std::endl;
		system("pause");
		return -1;
	}
	memset(&socket_address, 0, sizeof(socket_address));
	socket_address.sin_family = AF_INET;
	socket_address.sin_addr.s_addr = htonl(binary_address);
	socket_address.sin_port = htons(port_number);
	socket_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_id == INVALID_SOCKET)
	{
		std::cout << "Cannot start socket." << std::endl;
		WSACleanup();
		system("pause");
		return -1;
	}
	gethostname(hostname, 255);
	host_entry = gethostbyname(hostname);
	LocalIP = inet_ntoa(*(struct in_addr*) * host_entry->h_addr_list);
	std::cout << "Socket client " << LocalIP << ":" << ntohs(socket_address.sin_port) << std::endl;
	start = clock();
	now = start;
	while (now - start < TIMEOUT && !connected)
	{
		result = connect(socket_id, (struct sockaddr*) & socket_address, sizeof(socket_address));
		if (result == SOCKET_ERROR)
		{
			closesocket(socket_id);
			now = clock();
		}
		else {
			std::cout << "Connection to the server estabilished." << std::endl;
			start = clock();
			now = start;
			result = 0;
			while ((now - start) < TIMEOUT && !connected)
			{
				result = send(socket_id, username, strlen(username), NULL);
				if (result == strlen(username))
					connected = true;
				now = clock();
			}
		}
	}
	if (!connected)
		std::cout << "Connection timed out." << std::endl;
	else
	{
		//std::cout << "Write your first message: " << std::endl;
		while (connected)
		{
			std::cin.getline(message, MAX_LENGTH - 1);
			result = send(socket_id, message, strlen(message), NULL);
			if (result < 0)
				connected = false;
		}
		std::cout << "Lost connection to the server." << std::endl;
	}
	closesocket(socket_id);
	WSACleanup();
	system("pause");
	return 0;
}

static unsigned long IP_to_bin(char ip_add[])
{
	unsigned long add;
	unsigned char byte;
	char* token;
	if ((token = strtok(ip_add, ".")) == NULL)
		return 0x00000000;
	byte = (unsigned char)atoi(token);
	add = (unsigned long)byte * 16777216;
	if ((token = strtok(NULL, ".")) == NULL)
		return 0x00000000;
	byte = (unsigned char)atoi(token);
	add += (unsigned long)byte * 65536;
	if ((token = strtok(NULL, ".")) == NULL)
		return 0x00000000;
	byte = (unsigned char)atoi(token);
	add += (unsigned long)byte * 256;
	if ((token = strtok(NULL, ".")) == NULL)
		return 0x00000000;
	byte = (unsigned char)atoi(token);
	add += (unsigned long)byte * 1;
	return add;
}