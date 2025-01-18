#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <windows.h>
#include "interface.h"

#pragma comment(lib, "rpcrt4.lib")

#define SERVER_PORT "1234"

RPC_BINDING_HANDLE hBinding = NULL;

std::string path;

void* __RPC_USER midl_user_allocate(size_t size)
{
	return malloc(size);
}

void __RPC_USER midl_user_free(void* p)
{
	free(p);
}

void server_connect()
{
	RPC_STATUS status = 0;
	RPC_CSTR StringBinding;
	std::string IP_addr;
	std::cout << "Server IP address: ";
	std::cin >> IP_addr;
	status = RpcStringBindingComposeA(NULL, (RPC_CSTR)("ncacn_ip_tcp"), (RPC_CSTR)(IP_addr.c_str()), (RPC_CSTR)(SERVER_PORT), NULL, &StringBinding);
	if (status != RPC_S_OK) exit(status);
	status = RpcBindingFromStringBindingA(StringBinding, &hBinding);
	if (status != RPC_S_OK) exit(status);
}

void user_login()
{
	std::string username;
	std::string password;
	while (true)
	{
		std::cout << "Enter login: ";
		std::cin >> username;
		std::cout << "Enter password: ";
		std::cin >> password;
		if (Authenticate(hBinding, (unsigned char*)username.c_str(), (unsigned char*)password.c_str()))
			return;
		else
			std::cout << "Something went wrong, please try again!" << std::endl;
		username.clear();
		password.clear();
	}
	std::cout << "Connected successfully!" << std::endl;
}

void download()
{
	path.clear();
	std::cout << "Enter path to file on remote machine: ";
	std::cin >> path;
	unsigned int size = Get_File_Size(hBinding, (unsigned char*)path.c_str());
	if (size == 0)
	{
		std::cout << "Error: no access to file!" << std::endl;
		return;
	}
	unsigned char* arr = new unsigned char[size + 1];
	if (Download_File(hBinding, (unsigned char*)path.c_str(), arr, size))
	{
		path.clear();
		std::cout << "Enter path where you want to save file on local machine: ";
		std::cin >> path;
		std::ofstream file((const char*)path.c_str(), std::ios::binary);
		if (file.is_open())
		{
			file.write((const char*)arr, (std::streamsize)size);
			file.close();
			std::cout << "File saved!" << std::endl;
		}
		else
			std::cout << "Error: file not saved!" << std::endl;
	}
	else
		std::cout << "Error: no access to file!" << std::endl;
}

void upload()
{
	path.clear();
	std::cout << "Enter path to file on local machine: ";
	std::cin >> path;
	std::ifstream file((const char*)path.c_str(), std::ios::binary);
	if (file.is_open())
	{
		const auto begin = file.tellg();
		file.seekg(0, std::ios::end);
		const auto end = file.tellg();
		file.seekg(0, std::ios::beg);
		unsigned long size = (unsigned long)(end - begin);
		unsigned char* arr = new unsigned char[size + 1];
		file.read((char*)arr, (std::streamsize)size);
		file.close();
		std::cout << "Enter path where you want to save file on remote machine: ";
		path.clear();
		std::cin >> path;
		if (Upload_File(hBinding, (unsigned char*)path.c_str(), arr, size))
		{
			std::cout << "File uploaded!" << std::endl;
			return;
		}
		else
			std::cout << "Error: file not uploaded!" << std::endl;
	}
	else
		std::cout << "Error: no access to file!" << std::endl;
}

void remove()
{
	path.clear();
	std::cout << "Enter path to file you want to remove: ";
	std::cin >> path;
	if (Delete_File(hBinding, (unsigned char*)path.c_str()))
		std::cout << "File deleted!" << std::endl;
	else
		std::cout << "Error: file not deleted!" << std::endl;
}

void main_menu(void)
{
	while (true)
	{
		std::cout << "Choose an action:" << std::endl;
		std::cout << "1 - Upload file" << std::endl;
		std::cout << "2 - Download file" << std::endl; 
		std::cout << "3 - Remove file" << std::endl;
		std::cout << "4 - Exit program" << std::endl;
		std::cout << "> ";
		int inp = 0;
		std::cin >> inp;
		while (getchar() != '\n');
		if (inp == 1)
			upload();
		else if (inp == 2)
			download();
		else if (inp == 3)
			remove();
		else if (inp == 4)
			break;
		else
			std::cout << "Wrong input!" << std::endl;
	}
}

int main()
{
	server_connect();
	user_login();
	main_menu();

	return 0;
}