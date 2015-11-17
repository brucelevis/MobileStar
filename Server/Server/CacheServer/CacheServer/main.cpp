#include "CacheServer.h"

int main(int argc, char* argv[])
{
	CacheServer::getInstance();

	if (!CacheServer::getInstance()->initialize(1))
	{
		//Send Error Log!
		return -1;
	}

	CacheServer::getInstance()->run();

	return 0;
}
