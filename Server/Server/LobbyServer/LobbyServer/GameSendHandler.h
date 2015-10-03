#ifndef __GAME_SEND_HANDLER_H__
#define __GAME_SEND_HANDLER_H__

#include "Network.h"
#include "BasicPacket.h"

class GameSendHandler
{
	public:
		GameSendHandler();
		~GameSendHandler();
		bool initialize();

		void sendFailPacket(const ConnectInfo* connectInfo, command_t cmd, int32_t failReason);

	private:
		char m_sendBuffer[5000];
};

#endif //__GAME_SEND_HANDLER_H__
