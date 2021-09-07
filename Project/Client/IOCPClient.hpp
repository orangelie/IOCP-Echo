/**
*
* file IOCPClient.hpp
* author Mawile
* version 1.0.0
*
*/

#pragma once
#pragma comment(lib, "ws2_32") //< ws2_32.a

#include "../IOCompletionPort/IOCPError.hpp"
#include <iostream> //< std::cin | std::cout

/**
		@brief Loopback IP
*/
#define TCPIP_LOOPBACK ("127.0.0.1")

/// <summary>
/// mawile
/// </summary>
namespace mawile {
	/// <summary>
	/// IOCPClient Class
	/// </summary>
	class IOCPClient {
	public:
		/// <summary>
		/// Initializing Function WSA IOCP Client
		/// </summary>
		/// <param name="_ipAddress">: Server IpAddress</param>
		/// <param name="_Port">: Server Port</param>
		/// <returns>IOCPError Status</returns>
		IOCPError Initialize(const char*, int);


		/// <summary>
		/// Connecting Function WSA IOCP Client
		/// </summary>
		/// <param name="InfinityLoopConnetion">: Continuous connection until connection to the server is reached</param>
		/// <returns>IOCPError Status</returns>
		IOCPError Connect(bool);

		IOCPClient();
		~IOCPClient();

	private:
		LPPER_CompletionSock perCompletionSock;
		WSAEVENT hEvent;

	};


	/// <summary>
	/// Error Handling Function
	/// </summary>
	/// <param name="hIOCPError"></param>
	void IOCPErrorResult(IOCPError);
}