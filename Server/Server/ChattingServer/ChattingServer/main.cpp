#include "ChattingServer.h"

int main(int argc, char* argv[])
{
	ChattingServer::getInstance();

	if (!ChattingServer::getInstance()->initialize(1))
	{
		//Send Error Log!
		return -1;
	}

	ChattingServer::getInstance()->run();

	return 0;
}
