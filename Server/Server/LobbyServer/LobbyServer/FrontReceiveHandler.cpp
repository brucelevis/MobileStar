#include "FrontReceiveHandler.h"
#include "FrontSendHandler.h"
#include "Log.h"
#include "User.h"

FrontReceiveHandler::FrontReceiveHandler()
{

}

FrontReceiveHandler::~FrontReceiveHandler()
{

}

bool FrontReceiveHandler::initialize()
{

	return true;
}

void FrontReceiveHandler::sessionIn(const ConnectInfo* connectInfo)
{
	DebugLog("FrontReceiveHandler::SessionIn");

}

void FrontReceiveHandler::sessionOut(const ConnectInfo* connectInfo)
{
	DebugLog("FrontReceiveHandler::SessionOut");

}

void FrontReceiveHandler::receiveData(const ConnectInfo* connectInfo, const command_t cmd, const char* body, const int bodySize)
{
	switch(cmd)
	{
//	case FrontLobbyPacket::FIRST_CONNECT_RES:
//		HandleFirstConnectRes(session, body, bodySize);
//		break;
//
//	case FrontLobbyPacket::ENTER_FRONT_USER_REQ:
//		HandleEnterFrontUserReq(session, body, bodySize);
//		break;

	default:
		ErrorLog("invalid command - type = %d", cmd);
		return ;
	}

}
/*
void FrontReceiveHandler::HandleFirstConnectRes(Session* const session, const char* body, const int bodySize)
{
	DebugLog("FrontReceiveHandler::HandleFirstConnectRes");

	FrontLobbyPacket::FirstConnectResPacket packet;
	
	if (sizeof(packet) != bodySize)
	{
		ErrorLog("??");
		return;
	}
	memcpy(&packet, body, bodySize);

	return;
}

void FrontReceiveHandler::HandleEnterFrontUserReq(Session* const session, const char* body, int bodySize)
{
	DebugLog("FrontReceiveHandler::HandleEnterFrontUserReq");

	FrontLobbyPacket::EnterFrontUserReqPacket packet;

	if (sizeof(packet) != bodySize)
	{
		ErrorLog("??");
		return;
	}
	memcpy(&packet, body, bodySize);

	SessionId_t sid = packet.sid;

	UserInfo userInfo;
	memset(&userInfo, 0, sizeof(UserInfo));

	userInfo.userNo = packet.userNo;
	userInfo.nickNameLen = packet.nickNameLen;
	memcpy(userInfo.nickName, packet.nickName, userInfo.nickNameLen);

	userInfo.commonWin = packet.commonWin;
	userInfo.commonLose = packet.commonLose;
	userInfo.commonDiss = packet.commonDiss;
	userInfo.rankWin = packet.rankWin;
	userInfo.rankLose = packet.rankLose;
	userInfo.rankDiss = packet.rankDiss;
	userInfo.grade = packet.grade;
	userInfo.gradeReachedCount = packet.gradeReachedCount;
	userInfo.gold = packet.gold;
	userInfo.point = packet.point;
	userInfo.icon = packet.icon;
	userInfo.memoLen = packet.memoLen;
	memcpy(userInfo.memo, packet.memo, userInfo.memoLen);

	char sessionIdStr[37] = { 0, };
	UnParseSessionIdToSessionIdStr(sessionIdStr, packet.sid);
	DebugLog("temp sid = %s", sessionIdStr);

	int32_t failReason = LobbyServer::GetInstance().userMgr->AddUser(NULL, packet.sid, &userInfo);
	if (failReason == ALREADY_EXIST_USER)
	{
		User* user = LobbyServer::GetInstance().userMgr->GetUserByUserNo(userInfo.userNo);
		if (user == NULL)
		{
			ErrorLog("get user by userNo");
			LobbyServer::GetInstance().frontSendHandler->HandleFailPacket(session, sid, FrontLobbyPacket::ENTER_FRONT_USER_FAIL, FrontLobbyPacket::SERVER_ERROR);
			//필요한 롤백처리해야됨.
			return;

		}

		user->SetTempSid(sid);
		LobbyServer::GetInstance().frontSendHandler->HandleEnterFrontUserRes(session, sid);
	}
	else if (failReason != SUCCESS)
	{
		ErrorLog("add user fail - failReason = %d", failReason);
		//상세화시켜야됨.
		LobbyServer::GetInstance().frontSendHandler->HandleFailPacket(session, sid, FrontLobbyPacket::ENTER_FRONT_USER_FAIL, FrontLobbyPacket::SERVER_ERROR);
		return;
	}

	User* user = LobbyServer::GetInstance().userMgr->GetUserByUserNo(userInfo.userNo);
	if (user == NULL)
	{
		ErrorLog("get user by userNo");
		LobbyServer::GetInstance().frontSendHandler->HandleFailPacket(session, sid, FrontLobbyPacket::ENTER_FRONT_USER_FAIL, FrontLobbyPacket::SERVER_ERROR);
		//필요한 롤백처리해야됨.
		return;

	}

	int16_t channelNo = LobbyServer::GetInstance().channelMgr->FirstEnterUser(user);

	if (channelNo == INVALID_CHANNEL_NO)
	{
		ErrorLog("channelNo not invalid");
		LobbyServer::GetInstance().frontSendHandler->HandleFailPacket(session, sid, FrontLobbyPacket::ENTER_FRONT_USER_FAIL, FrontLobbyPacket::SERVER_ERROR);
		return;
	}

	user->SetChannelNo(channelNo);

	LobbyServer::GetInstance().frontSendHandler->HandleEnterFrontUserRes(session, sid);
}*/