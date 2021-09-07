#include "IOCPClient.hpp"

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


	IOCPError IOCPClient::Initialize(const char* _ipAddress, int _Port) {
		std::cout
			<< "*[[ IOCPClient ]]*\n\n";

		WSADATA			wsa = { 0 };
		SOCKADDR_IN		addr = { 0 };

		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
			return IOCPError::InWSAStartup;

		SOCKET hServer = WSASocketW(PF_INET, 0, 0, 0, 0, WSA_FLAG_OVERLAPPED);
		if (hServer == INVALID_SOCKET)
			return IOCPError::InWSASocket;
		(this->perCompletionSock->hSocket) = hServer;

		addr.sin_family = AF_INET;
		addr.sin_port = htons(_Port);
		inet_pton(AF_INET, _ipAddress, &addr.sin_addr); //< addr.sin_addr.s_addr = inet_addr(_ipAddress);
		memcpy(&(this->perCompletionSock->hAddr), &(addr), sizeof addr);

		(this->hEvent) = WSACreateEvent();


		return IOCPError::Success;
	}

	IOCPError IOCPClient::Connect(bool InfinityLoopConnetion = false) {
		DWORD		dwFlag;
		DWORD		SendBytes;
		char		Message[MESSAGE_MAXIMUM_SIZE];
		WSABUF		wsaBuf;
		OVERLAPPED	hOverlapped = { 0 };
		hOverlapped.hEvent = (this->hEvent);

		/**
				@brief Infinite attempts to connect.
		*/
		if (InfinityLoopConnetion) {
			while (connect((this->perCompletionSock->hSocket), (SOCKADDR*)&(this->perCompletionSock->hAddr), sizeof(this->perCompletionSock->hAddr)));
		}

		/**
				@brief Finite attempts to connect.
		*/
		else {
			int result = connect((this->perCompletionSock->hSocket), (SOCKADDR*)&(this->perCompletionSock->hAddr), sizeof(this->perCompletionSock->hAddr));
			if (result == SOCKET_ERROR)
				return IOCPError::InConnect;
		}

		while (true) {
			dwFlag = 0x00;
			RtlSecureZeroMemory(Message, MESSAGE_MAXIMUM_SIZE);

			std::cout << "Input the Message : ";
			std::cin >> Message;
			wsaBuf.buf = Message;
			wsaBuf.len = std::strlen(Message);

			int WSASendResult = WSASend((this->perCompletionSock->hSocket), &wsaBuf, 1, &SendBytes, 0, &hOverlapped, 0);
			if (WSASendResult == SOCKET_ERROR) {
				if (WSAGetLastError() != WSA_IO_PENDING)
					return IOCPError::InWSASend;
			}

			WSAWaitForMultipleEvents(1, &(this->hEvent), TRUE, WSA_INFINITE, FALSE);
			WSAGetOverlappedResult((this->perCompletionSock->hSocket), &hOverlapped, &SendBytes, 0, 0);
			
			std::cout
				<< "[ Send Message ]\nMessage: "
				<< wsaBuf.buf
				<< "\nSize: "
				<< SendBytes
				<< "\n\n";

			int WSARecvResult = WSARecv((this->perCompletionSock->hSocket), &wsaBuf, 1, 0, &dwFlag, &hOverlapped, 0);
			if (WSARecvResult == SOCKET_ERROR) {
				if (WSAGetLastError() != WSA_IO_PENDING)
					return IOCPError::INWSARecv;
			}

			std::cout
				<< "[ Received Message ]\nMessage: "
				<< wsaBuf.buf
				<< "\n\n";
		}


		return IOCPError::Success;
	}

	IOCPClient::IOCPClient() {
		(this->perCompletionSock) = new PER_CompletionSock;
	}

	IOCPClient::~IOCPClient() {
		delete (this->perCompletionSock);
		WSACleanup();
	}
}