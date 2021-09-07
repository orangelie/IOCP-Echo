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