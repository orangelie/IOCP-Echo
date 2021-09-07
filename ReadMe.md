# **IOCP-Echo**

+ It is an IOCP-Echo-Server using an Overlapped I/O Network Model.
+ Overlapped I/O入出力ネットワークモデルを利用したIOCP-Echoサーバーです。
+ Overlapped I/O 네트워크 모델을 이용한 IOCP 에코서버입니다.

<br></br>
## **クライアントを特定できます。**

+ Clients can be specified.
+ 클라이언트를 특정할 수 있습니다.

```cpp
/**
		@struct PER_CompletionSock
*/
typedef struct {
	SOCKET hSocket; //< SocketHandle
	SOCKADDR_IN hAddr; //< SocketAddrIn
	unsigned int hNumberOfClient; //< ClientNumber
} PER_CompletionSock, * LPPER_CompletionSock;
```

<br></br>
## **Available References**

```cpp

/**
		@param (Server-IPAddress, Sever-Port)
*/
mawile::IOCPError mawile::IOCPClient::Initialize(const char*, int);


/**
		@param (Continuous connection until connection to the server is reached)
*/
mawile::IOCPError mawile::IOCPClient::Connect(bool);


/**
		@param (Client Acceptance Callback Function, Server-Port)
*/
mawile::IOCPError mawile::IOCPServer::Initialize(_beginthreadex_proc_type, int);


/**
		@param (Client infinite listenability)
*/
mawile::IOCPError mawile::IOCPServer::Listen(bool);

```

<br></br>
## **IOCPServerMain.cpp**
```cpp
#include "IOCPServer.hpp"

int main() {
	mawile::IOCPServer* iocpServer = new mawile::IOCPServer;

	mawile::IOCPError ErrorResult1 = iocpServer->Initialize(mawile::hCompletionThread, 8080);
	if (ErrorResult1 != mawile::IOCPError::Success) {
		mawile::IOCPErrorResult(ErrorResult1);
		return (-1);
	}

	mawile::IOCPError ErrorResult2 = iocpServer->Listen(false);
	if (ErrorResult2 != mawile::IOCPError::Success) {
		mawile::IOCPErrorResult(ErrorResult2);
		return (-1);
	}

	delete iocpServer;
	return (0);
}
```

<br></br>
## **IOCPClientMain.cpp**
```cpp
#include "IOCPClient.hpp"

int main() {
	mawile::IOCPClient* iocpClient = new mawile::IOCPClient;

	mawile::IOCPError ErrorResult1 = iocpClient->Initialize(TCPIP_LOOPBACK, 8080);
	if (ErrorResult1 != mawile::IOCPError::Success) {
		mawile::IOCPErrorResult(ErrorResult1);
		return (-1);
	}

	mawile::IOCPError ErrorResult2 = iocpClient->Connect(false);
	if (ErrorResult2 != mawile::IOCPError::Success) {
		mawile::IOCPErrorResult(ErrorResult2);
		return (-1);
	}

	delete iocpClient;
	return (0);
}
```

<br></br>
# **Pictures**

![](https://github.com/Mawi1e/IOCP-Echo/blob/main/Pictures/1_1.PNG)
![](https://github.com/Mawi1e/IOCP-Echo/blob/main/Pictures/1_2.PNG)
![](https://github.com/Mawi1e/IOCP-Echo/blob/main/Pictures/2_1.PNG)
![](https://github.com/Mawi1e/IOCP-Echo/blob/main/Pictures/2_2.PNG)
![](https://github.com/Mawi1e/IOCP-Echo/blob/main/Pictures/3_1.PNG)
![](https://github.com/Mawi1e/IOCP-Echo/blob/main/Pictures/3_2.PNG)