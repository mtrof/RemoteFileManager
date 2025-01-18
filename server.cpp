#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <windows.h>
#include "interface.h"

#pragma comment(lib, "rpcrt4.lib")

#define BUFFER_SIZE 128

#define LISTEN_PORT "1234"

RPC_BINDING_HANDLE hToken;

char username[BUFFER_SIZE] = { 0 };

void user_exit();

void* __RPC_USER MIDL_user_allocate(size_t size)
{
	return malloc(size);
}

void __RPC_USER MIDL_user_free(void* p)
{
	free(p);
}

int Authenticate(handle_t hBinding, unsigned char* Login, unsigned char* Password)
{
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		username[i] = Login[i];
		if (Login[i] == '\0') break;
	}
	if (LogonUserA((LPCSTR)Login, NULL, (LPCSTR)Password, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &hBinding) && ImpersonateLoggedOnUser(hBinding))
	{
		std::cout << "User " << Login << " connected" << std::endl;
		hToken = hBinding;
		return true;
	}
	else
	{
		std::cout << "Error on user " << Login << " login" << std::endl;
		return false;
	}
}

unsigned int Get_File_Size(handle_t hBinding, unsigned char* Path)
{
	if (!ImpersonateLoggedOnUser(hToken))
	{
		user_exit();
		return false;
	}
	std::ifstream file((const char*)Path, std::ios::binary);
	if (file.is_open())
	{
		const auto begin = file.tellg();
		file.seekg(0, std::ios::end);
		const auto end = file.tellg();
		file.seekg(0, std::ios::beg);
		unsigned int size = (unsigned int)(end - begin);
		file.close();
		std::cout << "File " << Path << " size: " << size << " bytes" << std::endl;
		return size;
	}
	else
	{
		std::cout << "No access to file " << Path << " by user " << username << std::endl;
		return 0;
	}
}

int Download_File(handle_t hBinding, unsigned char* Path, unsigned char* Arr, long Size)
{
	if (!ImpersonateLoggedOnUser(hToken))
	{
		user_exit();
		return false;
	}
	std::ifstream file((const char*)Path, std::ios::binary);
	if (file.is_open())
	{
		file.read((char*)Arr, (std::streamsize)Size);
		file.close();
		std::cout << "File " << Path << " downloaded by user " << username << std::endl;
		return true;
	}
	else
	{
		std::cout << "No access to file " << Path << " by user " << username << std::endl;
		return false;
	}
	return false;
}

int Upload_File(handle_t hBinding, unsigned char* Path, unsigned char* Arr, long Size)
{
	if (!ImpersonateLoggedOnUser(hToken))
	{
		user_exit();
		return false;
	}
	std::ofstream file((const char*)Path, std::ios::binary);
	if (file.is_open())
	{
		file.write((char*)Arr, (std::streamsize)Size);
		file.close();
		std::cout << "File " << Path << " uploaded by user " << username << std::endl;
		return true;
	}
	else
	{
		std::cout << "No access to file " << Path << " by user " << username << std::endl;
		return false;
	}
	return false;
}

int Delete_File(handle_t hBinding, const unsigned char* Path)
{
	if (!ImpersonateLoggedOnUser(hToken))
	{
		user_exit();
		return false;
	}
	if (!remove((LPSTR)Path))
	{
		std::cout << "File " << Path << " deleted by user " << username << std::endl;
		return true;
	}
	else
	{
		std::cout << "No access to file " << Path << " by user " << username << std::endl;
		return false;
	}
}

void user_exit()
{
	std::cout << "User " << username << " disconnected" << std::endl;
	memset(username, 0, BUFFER_SIZE);
}

int main()
{
	RPC_STATUS status = 0;
	status = RpcServerUseProtseqEpA((RPC_CSTR)("ncacn_ip_tcp"), RPC_C_PROTSEQ_MAX_REQS_DEFAULT, (RPC_CSTR)(LISTEN_PORT), NULL);
	if (status != RPC_S_OK) exit(status);
	status = RpcServerRegisterIf2(FileSystemInterface_v1_0_s_ifspec, NULL, NULL, RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH, RPC_C_LISTEN_MAX_CALLS_DEFAULT, -1, NULL);
	if (status != RPC_S_OK) exit(status);
	std::cout << "Waiting for connections on port " << LISTEN_PORT << std::endl;
	RPC_STATUS ret = RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, 0);
	return ret;
}