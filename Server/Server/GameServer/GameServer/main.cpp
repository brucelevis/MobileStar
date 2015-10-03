#include "GameServer.h"

int main(int argc, char* argv[])
{
	GameServer::getInstance();

	if (!GameServer::getInstance()->initialize(1))
	{
		//Send Error Log!
		return -1;
	}

	GameServer::getInstance()->run();

	return 0;
}
