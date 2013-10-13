#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

#include "ClientSocket.h"
using namespace std;

ClientSocket::ClientSocket()
{

}

void * ClientSocket::get_inet_addr(struct sockaddr * aSocketAddr)
{
	// get appropriate IP depending on address family (AF?); (IPv4/IPv6)
	if (aSocketAddr->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in*)aSocketAddr)->sin_addr);
	}
	return &(((struct sockaddr_in6*)aSocketAddr)->sin6_addr);
}

bool ClientSocket::connectSocket(std::string aIpAddress, std::string aPort)
{
	struct addrinfo lUserSpec, *server_info, *ptr;
	int lErrorCode; // rename
	char lRemoteConnectedIP[INET6_ADDRSTRLEN];

	memset(&lUserSpec, 0, sizeof lUserSpec);
	lUserSpec.ai_family = AF_UNSPEC;
	lUserSpec.ai_socktype = SOCK_STREAM;
 	lErrorCode = getaddrinfo(aIpAddress.c_str(), aPort.c_str(), 
 								&lUserSpec, &server_info);
 	if (lErrorCode !=0)
 	{
 		return false;
 	}
 	for(ptr = server_info; ptr != NULL; ptr = ptr->ai_next)
 	{
 		fSocketFileDesc = 
 			socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
 		
 		if (fSocketFileDesc == -1)
 		{
 			// if failed skip the rest of the loop
 			cout << gai_strerror(lErrorCode) << endl;
 			continue; 
 		}


 		if (connect(fSocketFileDesc, ptr->ai_addr, ptr->ai_addrlen) == -1)
 		{
 			// if failed skip the rest of the loop
 			cout << gai_strerror(lErrorCode) << endl;
 			continue;
 		}
		
		// break because if we got to here, we're connected
 		break;

 	}
 	// if ptr equals NULL they no results worked
 	return ptr != NULL;

}

int ClientSocket::sendMsg(std::string aMsg)
{
	int lMsgLen = aMsg.length() + 1;
	char * lMsg = new char[lMsgLen];
	strncpy(lMsg, aMsg.c_str(), lMsgLen);
	return send(fSocketFileDesc, lMsg, lMsgLen, 0);
}

std::string ClientSocket::recvMsg()
{
	int lBufferLen = 1024;
	char lBuffer[1024];
	string lMsg = "";

	int numBytes;
	//do 
	// {
		numBytes = recv(fSocketFileDesc, &lBuffer, lBufferLen, 0);
		if (numBytes < 0)
		{
			lMsg = "ERROR: unable to receive message.\n"
				"\tClose socket and try again.";
		}
		else
		{
			for (int i = 0; i < numBytes; i++)
			{
				lMsg += lBuffer[i];
			}
		}
	//} while(numBytes > 0);

	return lMsg;
}

void ClientSocket::closeSocket()
{
	close(fSocketFileDesc);
}