#include <string.h>
#include <iostream>
#include <sstream>
#include "ClientSocket.h"
using namespace std;

int main(int argc, char const *argv[])
{
	ClientSocket * lSocket = new ClientSocket();
	string lUsername;
	string lInput;
	if (argc < 4)
	{
		cerr << "Usage: ./scClient <address> <port> <username>" << endl;
		return 1;
	}
	if (lSocket->connectSocket(argv[1], argv[2]))
	{
		lUsername = argv[3];
		cout << "CONNECTED!" << endl;

		cout << lSocket->recvMsg() << ": " << endl;
		getline(cin,lInput);
		if (lSocket->sendMsg(lInput) == -1)
		{
			cout << "ERROR: send failed" << endl;
		}

		do
		{

			cout << lSocket->recvMsg() << endl;
			cout << "\nYou: ";
			getline(cin, lInput);
			if (lInput != ":q")
			{
				lSocket->sendMsg(lUsername + "" + lInput);
			}

		} while(lInput != ":q");

		lSocket->closeSocket();
	}
	else 
	{
		cout << "Unable to connect" << endl;
	}


	return 0;
}