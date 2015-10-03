#include "FrontServer.h"

int main(int argc, char* argv[])
{
	FrontServer::getInstance();

	if (!FrontServer::getInstance()->initialize(1))
	{
		//Send Error Log!
		return -1;
	}

	FrontServer::getInstance()->run();

	return 0;
}
