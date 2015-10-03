#include "LobbyServer.h"

int main(int argc, char* argv[])
{
	LobbyServer::getInstance();

	if (!LobbyServer::getInstance()->initialize(1))
	{
		//Send Error Log!
		return -1;
	}

	LobbyServer::getInstance()->run();

	return 0;
}
