#pragma once
#pragma comment(lib, "ws2_32")

#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

#ifndef MESSAGE_MAXIMUM_SIZE
#define MESSAGE_MAXIMUM_SIZE (0x200)
#endif

namespace mawile {
	/// <summary>
	/// IOCPError EnumClass
	/// </summary>
	enum class IOCPError {
		Success = 0x00,
		InWSAStartup,
		InWSASocket,
		InBind,
		InListen,
		InAccept,
		InConnect,
		InWSASend,
		INWSARecv,
	};

	/// <summary>
	/// PER_CompletionSock
	/// </summary>
	typedef struct {
		SOCKET hSocket;
		SOCKADDR_IN hAddr;
		unsigned int hNumberOfClient;
	} PER_CompletionSock, * LPPER_CompletionSock;

	/// <summary>
	/// PER_CompletionIo
	/// </summary>
	typedef struct {
		OVERLAPPED hOverlapped;
		WSABUF hWsaBuf;
		char Message[MESSAGE_MAXIMUM_SIZE];
	} PER_CompletionIo, * LPPER_CompletionIo;
}