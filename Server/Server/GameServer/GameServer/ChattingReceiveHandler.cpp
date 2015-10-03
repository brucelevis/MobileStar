#include "ChattingReceiveHandler.h"
#include "Log.h"
#include "User.h"

ChattingReceiveHandler::ChattingReceiveHandler()
{
	
}

ChattingReceiveHandler::~ChattingReceiveHandler()
{

}

bool ChattingReceiveHandler::initialize()
{

	return true;
}

void ChattingReceiveHandler::sessionIn(const ConnectInfo* connectInfo)
{

}

void ChattingReceiveHandler::sessionOut(const ConnectInfo* connectInfo)
{
    User* user = (User*)connectInfo->userData;
	if (user == NULL)
	{
		ErrorLog("user not login");
		return;
	}

//	user->SetSession(NULL);

//	LobbyServer::getInstance()->centerSendHandler->HandleLogoutUserReq(userNo);
}

void ChattingReceiveHandler::receiveData(const ConnectInfo* connectInfo, const uint32_t cmd, const char* body, const int bodySize)
{
    /*
	switch(cmd)
	{
	case ClientLobbyPacket::FIRST_CONNECT_REQ:
		handleFirstConnectReq(connectInfo, body, bodySize);
		break;

	case ClientLobbyPacket::CHATTING_REQ:
		handleChattingReq(connectInfo, body, bodySize);
		break;

	case ClientLobbyPacket::GET_CHANNEL_LIST_REQ:
		handleGetChannelListReq(connectInfo, body, bodySize);
		break;

	case ClientLobbyPacket::MOVE_CHANNEL_REQ:
		handleMoveChannelReq(connectInfo, body, bodySize);
		break;

	case ClientLobbyPacket::GET_USER_LIST_REQ:
		handleGetUserListReq(connectInfo, body, bodySize);
		break;

	case ClientLobbyPacket::GET_USER_INFO_REQ:
		handleGetUserInfoReq(connectInfo, body, bodySize);
		break;

	case ClientLobbyPacket::GET_FRIEND_LIST_REQ:
		handleGetFriendListReq(connectInfo, body, bodySize);
		break;

	case ClientLobbyPacket::ADD_RESPONSE_FRIEND_REQ:
		handleAddResponseFriendReq(connectInfo, body, bodySize);
		break;

	default:
		ErrorLog("invalid command - type = %d", cmd);
		return ;
	}
     */

}
/*
void ClientReceiveHandler::HandleFirstConnectReq(const ConnectInfo* connectInfo, const char* body, const int bodySize)
{
	DebugLog("ClientReceiveHandler::HandleFirstConnectReq");
	ClientLobbyPacket::FirstConnectReqPacket packet;
	memset(&packet, 0, sizeof(packet));
	if (sizeof(packet) != bodySize)
	{
		ErrorLog("??");
		return;
	}
	memcpy(&packet, body, bodySize);

	int64_t userNo = packet.userNo;

	User* user = LobbyServer::GetInstance().userMgr->GetUserByUserNo(userNo);
	if (user == NULL)
	{
		ErrorLog("user not exist userNo = %d", userNo);
		LobbyServer::GetInstance().clientSendHandler->HandleFailPacket(session, ClientLobbyPacket::FIRST_CONNECT_FAIL, ClientLobbyPacket::SERVER_ERROR);
		return;
	}

	SessionId_t sid1 = user->GetTempSid();
	SessionId_t sid2 = packet.sid;

	if (memcmp(&sid1, &sid2, sizeof(packet.sid)) != 0)
	{
		char sessionIdStr1[37] = { 0, };
		char sessionIdStr2[37] = { 0, };
		UnParseSessionIdToSessionIdStr(sessionIdStr1, user->GetTempSid());
		UnParseSessionIdToSessionIdStr(sessionIdStr2, packet.sid);

		ErrorLog("Temp Session not correct %s %s", sessionIdStr1, sessionIdStr2);
		LobbyServer::GetInstance().clientSendHandler->HandleFailPacket(session, ClientLobbyPacket::FIRST_CONNECT_FAIL, ClientLobbyPacket::SERVER_ERROR);
		return;
	}

	session->userKey = userNo;

	LobbyServer::GetInstance().centerSendHandler->HandleEnterFrontUserReq(session->GetSessionId(), user);

	return;
}

void ClientReceiveHandler::HandleChattingReq(Session* const session, const char* body, int bodySize)
{
	DebugLog("ClientReceiveHandler::HandleChattingReq");

	ClientLobbyPacket::ChattingReqPacket packet;
	memset(&packet, 0, sizeof(packet));
	if (sizeof(packet) < bodySize)
	{
		ErrorLog("??");
		return;
	}
	memcpy(&packet, body, bodySize);

	User* user = LobbyServer::GetInstance().userMgr->GetUserByUserNo(session->userKey);
	if (user == NULL)
	{
		ErrorLog("user not exist userNo = %d", session->userKey);
		LobbyServer::GetInstance().clientSendHandler->HandleFailPacket(session, ClientLobbyPacket::FIRST_CONNECT_FAIL, ClientLobbyPacket::SERVER_ERROR);
		return;
	}

	char nickName[BasicPacket::MAX_NICK_NAME_LEN + 1] = { 0, };
	memcpy(nickName, user->GetNickName(), user->GetNickNameLen());

	int8_t chattingLen = packet.chattingLen;
	char chatting[BasicPacket::MAX_CHATTING_LEN + 1] = { 0, };
	memcpy(chatting, packet.chatting, chattingLen);

	Channel* channel = LobbyServer::GetInstance().channelMgr->GetChannelByChannelNo(user->GetChannelNo());

	DebugLog("channelNo = %d, nickname = %s, chatting = %s", user->GetChannelNo(), nickName, chatting);

	std::string chattingStr = std::string(chatting);
	if (chattingStr.at(0) == '/')
	{
		DebugLog("verify function call");
		if (chattingStr.length() < 5)
		{
			LobbyServer::GetInstance().clientSendHandler->HandleFailPacket(session, ClientLobbyPacket::CHATTING_FAIL, ClientLobbyPacket::SERVER_ERROR);

			return;
		}
		if (chattingStr.at(1) == 'f')
		{
			if (chattingStr.at(2) == 'a')
			{
				if (chattingStr.at(3) == ' ')
				{
					std::string nickNameStr = chattingStr.substr(4);

					if (nickNameStr.length() != 0)
					{
						const char* nickName = nickNameStr.c_str();

						LobbyServer::GetInstance().cacheSendHandler->HandleAddRequestFriendReq(session->GetSessionId(), session->userKey, nickName, nickNameStr.length());
					}
					else // 문법오류
					{
						LobbyServer::GetInstance().clientSendHandler->HandleFailPacket(session, ClientLobbyPacket::CHATTING_FAIL, ClientLobbyPacket::SERVER_ERROR);
						return;
					}

				}
				else //문법 오류
				{
					LobbyServer::GetInstance().clientSendHandler->HandleFailPacket(session, ClientLobbyPacket::CHATTING_FAIL, ClientLobbyPacket::SERVER_ERROR);
					return;
				}
			}
			else if (chattingStr.at(2) == 'd')
			{
				if (chattingStr.at(3) == ' ')
				{
					std::string nickNameStr = chattingStr.substr(4);

					if (nickNameStr.length() != 0)
					{
						const char* nickName = nickNameStr.c_str();

						LobbyServer::GetInstance().cacheSendHandler->HandleRemoveFriendReq(session->GetSessionId(), user->GetUserNo(), nickName, nickNameStr.length());
					}
					else // 문법오류
					{
						LobbyServer::GetInstance().clientSendHandler->HandleFailPacket(session, ClientLobbyPacket::CHATTING_FAIL, ClientLobbyPacket::SERVER_ERROR);
						return;
					}

				}
				else //문법 오류
				{
					LobbyServer::GetInstance().clientSendHandler->HandleFailPacket(session, ClientLobbyPacket::CHATTING_FAIL, ClientLobbyPacket::SERVER_ERROR);
					return;
				}
			}
			else//친구추가삭제가 아닐때
			{
				LobbyServer::GetInstance().clientSendHandler->HandleFailPacket(session, ClientLobbyPacket::CHATTING_FAIL, ClientLobbyPacket::SERVER_ERROR);
				return;
			}
		}
		else //친구 기능이 아닐때
		{
			LobbyServer::GetInstance().clientSendHandler->HandleFailPacket(session, ClientLobbyPacket::CHATTING_FAIL, ClientLobbyPacket::SERVER_ERROR);
			return;
		}
		return;
	}

	LobbyServer::GetInstance().clientSendHandler->HandleChattingNotify(channel, nickName, chattingLen, chatting);

	return;
}

void ClientReceiveHandler::HandleGetChannelListReq(const ConnectInfo* connectInfo, const char* body, int bodySize)
{
	DebugLog("ClientReceiveHandler::HandleGetChannelListReq");

	ClientLobbyPacket::GetChannelListReqPacket packet;
	memset(&packet, 0, sizeof(packet));
	if (sizeof(packet) < bodySize)
	{
		ErrorLog("??");
		return;
	}
	memcpy(&packet, body, bodySize);

	LobbyServer::GetInstance().centerSendHandler->HandleGetChannelListReq(session->GetSessionId());
}

void ClientReceiveHandler::HandleMoveChannelReq(const ConnectInfo* connectInfo, const char* body, int bodySize)
{
	DebugLog("ClientReceiveHandler::HandleMoveChannelReq");

	ClientLobbyPacket::MoveChannelReqPacket packet;
	memset(&packet, 0, sizeof(packet));
	if (sizeof(packet) < bodySize)
	{
		ErrorLog("??");
		return;
	}
	memcpy(&packet, body, bodySize);

	//channelNo가 이 서버에 존재하는지 확인하기.

	User* user = LobbyServer::GetInstance().userMgr->GetUserByUserNo(session->userKey);
	if (user == NULL)
	{
		ErrorLog("user not exist userNo = %lld", session->userKey);
		//send fail
		return;
	}

	if (user->GetChannelNo() == packet.channelNo)
	{
		ErrorLog("same channel Move user channel = %d, move channel = %d", user->GetChannelNo(), packet.channelNo);
		// send fail
		return;
	}

	LobbyServer::GetInstance().centerSendHandler->HandleMoveChannelReq(session->GetSessionId(), user->GetUserNo(), packet.channelNo);
}

void ClientReceiveHandler::HandleGetUserListReq(const ConnectInfo* connectInfo, const char* body, int bodySize)
{
	DebugLog("ClientReceiveHandler::HandleGetUserListReq");

	ClientLobbyPacket::GetUserListReqPacket packet;
	memset(&packet, 0, sizeof(packet));
	if (sizeof(packet) != bodySize)
	{
		ErrorLog("??");
		return;
	}
	memcpy(&packet, body, bodySize);

	User* user = LobbyServer::GetInstance().userMgr->GetUserByUserNo(session->userKey);
	if (user == NULL)
	{
		ErrorLog("user not exist userNo = %lld", session->userKey);
		//send fail
		return;
	}

	Channel* channel = LobbyServer::GetInstance().channelMgr->GetChannelByChannelNo(user->GetChannelNo());
	if (channel == NULL)
	{
		ErrorLog("channel not exist channelNo = %d", user->GetChannelNo());
		//send fail
		return;
	}

	LobbyServer::GetInstance().clientSendHandler->HandleGetUserListRes(session, channel);
}

void ClientReceiveHandler::HandleGetUserInfoReq(const ConnectInfo* connectInfo, const char* body, int bodySize)
{
	DebugLog("ClientReceiveHandler::HandleGetUserInfoReq");

	ClientLobbyPacket::GetUserInfoReqPacket packet;
	memset(&packet, 0, sizeof(packet));
	if (sizeof(packet) != bodySize)
	{
		ErrorLog("??");
		return;
	}
	memcpy(&packet, body, bodySize);

	NickNameInfo nickNameInfo;
	memset(&nickNameInfo, 0, sizeof(NickNameInfo));

	nickNameInfo.nickNameLen = packet.nickNameInfo.nickNameLen;
	memcpy(nickNameInfo.nickName, packet.nickNameInfo.nickName, BasicPacket::MAX_NICK_NAME_LEN);

	DebugLog("nickName %s %s %d %d", nickNameInfo.nickName, packet.nickNameInfo.nickName, nickNameInfo.nickNameLen, packet.nickNameInfo.nickNameLen);

	LobbyServer::GetInstance().cacheSendHandler->HandleGetUserInfoReq(session->GetSessionId(), &nickNameInfo);
}

void ClientReceiveHandler::HandleGetFriendListReq(Session* const session, const char* body, int bodySize)
{
	DebugLog("ClientReceiveHandler::HandleGetFriendListReq");

	ClientLobbyPacket::GetFriendListReqPacket packet;
	memset(&packet, 0, sizeof(packet));
	if (sizeof(packet) != bodySize)
	{
		ErrorLog("??");
		return;
	}
	memcpy(&packet, body, bodySize);

	LobbyServer::GetInstance().cacheSendHandler->HandleGetFriendListReq(session->GetSessionId(), session->userKey);
}

void ClientReceiveHandler::HandleAddResponseFriendReq(const ConnectInfo* connectInfo, const char* body, int bodySize)
{
	DebugLog("ClientReceiveHandler::HandleAddResponseFriendReq");

	ClientLobbyPacket::AddResponseFriendReqPacket packet;
	memset(&packet, 0, sizeof(packet));
	if (sizeof(packet) != bodySize)
	{
		ErrorLog("??");
		return;
	}
	memcpy(&packet, body, bodySize);

	LobbyServer::GetInstance().cacheSendHandler->HandleAddResponseFriendReq(session->GetSessionId(), session->userKey, packet.nickNameInfo.nickName, packet.nickNameInfo.nickNameLen, packet.isAccepted);

}*/