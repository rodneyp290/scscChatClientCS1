#include <string>


class ClientSocket
{
private:
	int fSocketFileDesc;
	void * get_inet_addr(struct sockaddr * aSocketAddr);
public:
	ClientSocket();
	bool connectSocket(std::string aIpAddress, std::string aPort);
	int sendMsg(std::string aMessage);
	std::string recvMsg();
	void closeSocket();
};
