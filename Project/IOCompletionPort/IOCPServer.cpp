#include "IOCPServer.hpp"

namespace mawile {
	void IOCPErrorResult(IOCPError hIOCPError) {
		switch (hIOCPError) {
		case IOCPError::InWSAStartup:
			MessageBoxW(0, L"InWSAStartup", L"##IOCPError##", MB_ICONWARNING);

			break;
		case IOCPError::InWSASocket:
			MessageBoxW(0, L"InWSASocket", L"##IOCPError##", MB_ICONWARNING);

			break;
		case IOCPError::InBind:
			MessageBoxW(0, L"InBind", L"##IOCPError##", MB_ICONWARNING);

			break;
		case IOCPError::InListen:
			MessageBoxW(0, L"InListen", L"##IOCPError##", MB_ICONWARNING);

			break;
		case IOCPError::InAccept:
			MessageBoxW(0, L"InAccept", L"##IOCPError##", MB_ICONWARNING);

			break;
		case IOCPError::InConnect:
			MessageBoxW(0, L"InConnect", L"##IOCPError##", MB_ICONWARNING);

			break;
		case IOCPError::InWSASend:
			MessageBoxW(0, L"InWSASend", L"##IOCPError##", MB_ICONWARNING);

			break;
		case IOCPError::INWSARecv:
			MessageBoxW(0, L"INWSARecv", L"##IOCPError##", MB_ICONWARNING);

			break;
		}
	}
	
	unsigned int _stdcall hCompletionThread(LPVOID hCompletionArgument) {
		HANDLE						hCompletionKey = (HANDLE)hCompletionArgument;
		LPPER_CompletionSock		hCompletionSock;
		LPPER_CompletionIo			hCompletionIo;
		DWORD						TransferredBytes;
		DWORD						dwFlag;

		while (true) {
			GetQueuedCompletionStatus(hCompletionKey, &TransferredBytes, (PULONG_PTR)&hCompletionSock, (LPOVERLAPPED*)&hCompletionIo, INFINITE);

			/**
					@brief Client Disconnection Point
			*/
			if (TransferredBytes <= 0) {
				std::cout
					<< "[-] Client Disconnection : " << hCompletionSock->hNumberOfClient << "\n\n";

				closesocket(hCompletionSock->hSocket);
				delete hCompletionSock;
				delete hCompletionIo;
				continue;
			}

			hCompletionIo->hWsaBuf.buf[TransferredBytes] = '\0';
			hCompletionIo->hWsaBuf.len = TransferredBytes;

			std::cout
				<< "[ Received Message ]\nMessage: "
				<< hCompletionIo->hWsaBuf.buf
				<< "\nClientNumber: "
				<< hCompletionSock->hNumberOfClient
				<< "\n\n";

			WSASend(hCompletionSock->hSocket, &hCompletionIo->hWsaBuf, 1, 0, 0, 0, 0);

			memset(&hCompletionIo->hOverlapped, 0x00, sizeof OVERLAPPED);
			hCompletionIo->hWsaBuf.buf = hCompletionIo->Message;
			hCompletionIo->hWsaBuf.len = MESSAGE_MAXIMUM_SIZE;
			dwFlag = 0x00;
			WSARecv(hCompletionSock->hSocket, &hCompletionIo->hWsaBuf, 1, 0, &dwFlag, &hCompletionIo->hOverlapped, 0);
		}


		return 0;
	}

	IOCPError IOCPServer::Initialize(_beginthreadex_proc_type CallbackFunction, int _Port) {
		std::cout
			<< "*[[ IOCPServer ]]*\n\n";
		
		WSADATA			wsa =			{ 0 };
		SYSTEM_INFO		systemInfo =	{ 0 };
		SOCKADDR_IN		addr =			{ 0 };

		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
			return IOCPError::InWSAStartup;

		(this->CompletionKey) = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

		GetSystemInfo(&systemInfo);
		(this->hAcceptableThread) = systemInfo.dwNumberOfProcessors;
		for (unsigned int i = 0; i < (this->hAcceptableThread); ++i)
			_beginthreadex(0, 0, CallbackFunction, (this->CompletionKey), 0, 0);

		SOCKET hServer = WSASocketW(PF_INET, 0, 0, 0, 0, WSA_FLAG_OVERLAPPED);
		if (hServer == INVALID_SOCKET)
			return IOCPError::InWSASocket;

		addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		addr.sin_family =			AF_INET;
		addr.sin_port =				htons(_Port);

		(this->perCompletionSock->hSocket) = hServer;
		memcpy(&(this->perCompletionSock->hAddr), &(addr), sizeof addr);


		return IOCPError::Success;
	}

	IOCPError IOCPServer::Listen(bool isInfinityListening = false) {
		unsigned int	cbCount =	0x00;
		DWORD			Flag =		0x00;

		if (bind((this->perCompletionSock->hSocket), (SOCKADDR*)&(this->perCompletionSock->hAddr), sizeof(this->perCompletionSock->hAddr)) == SOCKET_ERROR)
			return IOCPError::InBind;

		if (isInfinityListening) {
			if (listen((this->perCompletionSock->hSocket), SOMAXCONN) == SOCKET_ERROR)
				return IOCPError::InListen;
		}
		else {
			if (listen((this->perCompletionSock->hSocket), (this->hAcceptableThread)) == SOCKET_ERROR)
				return IOCPError::InListen;
		}

		while (true) {
			SOCKADDR_IN hAddrIn =		{ 0 };
			int			hAddrSize =		sizeof hAddrIn;

			/**
					@brief Client Connection Point
			*/
			SOCKET hClient = accept((this->perCompletionSock->hSocket), (SOCKADDR*)&hAddrIn, &hAddrSize);
			if (hClient == INVALID_SOCKET)
				return IOCPError::InAccept;

			LPPER_CompletionSock hCompletionClient = new PER_CompletionSock;
			hCompletionClient->hSocket = hClient;
			hCompletionClient->hNumberOfClient = cbCount++;
			memcpy(&(hCompletionClient->hAddr), &(hAddrIn), hAddrSize);

			CreateIoCompletionPort((HANDLE)hClient, (this->CompletionKey), (ULONG_PTR)hCompletionClient, 0);

			LPPER_CompletionIo hCompletionIo = new PER_CompletionIo;
			memset(&hCompletionIo->hOverlapped, 0x00, sizeof OVERLAPPED);
			hCompletionIo->hWsaBuf.buf = hCompletionIo->Message;
			hCompletionIo->hWsaBuf.len = MESSAGE_MAXIMUM_SIZE;
			Flag = 0x00;

			std::cout
				<< "[+] Client Connection : " << cbCount - 1 << "\n\n";

			WSARecv(hClient, &hCompletionIo->hWsaBuf, 1, 0, &Flag, &hCompletionIo->hOverlapped, 0);
		}


		return IOCPError::Success;
	}

	IOCPServer::IOCPServer() {
		(this->perCompletionSock) = new PER_CompletionSock;
	}

	IOCPServer::~IOCPServer() {
		delete (this->perCompletionSock);
		WSACleanup();
	}
}