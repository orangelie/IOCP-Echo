/**
* 
* file IOCPServer.hpp
* author Mawile
* version 1.0.0
* 
*/

#pragma once
#pragma comment(lib, "ws2_32") //< ws2_32.a

#include "IOCPError.hpp"
#include <process.h> //< _beginthreadex | GetSystemInfo
#include <iostream> //< std::cin | std::cout

/// <summary>
/// mawile
/// </summary>
namespace mawile {
	/// <summary>
	/// IOCPServer Class
	/// </summary>
	class IOCPServer {
	public:
		/// <summary>
		/// Initializing Function WSA IOCP Server
		/// </summary>
		/// <param name="CallbackFunction">: Client Acceptance Callback Function</param>
		/// <param name="_Port">: Server Port</param>
		/// <returns>IOCPError Status</returns>
		IOCPError Initialize(_beginthreadex_proc_type, int);


		/// <summary>
		/// Listening Function WSA IOCP Server
		/// </summary>
		/// <param name="isInfinityListening">: Client infinite listenability</param>
		/// <returns>IOCPError Status</returns>
		IOCPError Listen(bool);

		IOCPServer();
		~IOCPServer();

	private:
		LPPER_CompletionSock perCompletionSock;
		DWORD hAcceptableThread;
		HANDLE CompletionKey;

	};


	/// <summary>
	/// CompletionPort CallbackFunction
	/// </summary>
	/// <param name="hCompletionArgument">: HANDLE</param>
	/// <returns>Nothing</returns>
	unsigned int _stdcall hCompletionThread(LPVOID);


	/// <summary>
	/// Error Handling Function
	/// </summary>
	/// <param name="hIOCPError"></param>
	void IOCPErrorResult(IOCPError);
}