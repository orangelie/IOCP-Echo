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