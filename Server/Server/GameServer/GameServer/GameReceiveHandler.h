#ifndef __GAME_RECEIVE_HANDLER_H__
#define __GAME_RECEIVE_HANDLER_H__

#include "Network.h"
#include "BasicPacket.h"

class GameReceiveHandler
{
	public:
		GameReceiveHandler();
		~GameReceiveHandler();
		bool initialize();

		virtual void receiveData(const ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize);
		virtual void sessionIn(const ConnectInfo* connectInfo);
		virtual void sessionOut(const ConnectInfo* connectInfo);

	private:
		char m_sendBuffer[5000];
};

#endif //__GAME_RECEIVE_HANDLER_H__
