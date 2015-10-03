
#ifndef __NETWORK_DEFINES_H__
#define __NETWORK_DEFINES_H__

#include "BasicDefines.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <stdint.h>

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

const int MAX_SEQUENCE_NUMBER = 1000000;
const int MIN_SEQUENCE_NUMBER = 1;
const int MAX_SEQUENCE_NUMBER_DISTANCE = 500000;

typedef union _SessionId_t
{
	char id[16];

	boost::uuids::uuid uid;
	bool operator==(_SessionId_t const& u1) const { return uid == u1.uid; }
	_SessionId_t& operator=(_SessionId_t const& u1) { uid = u1.uid; return *this; }
}SessionId_t;

class SessionHelper
{
public:
	static SessionId_t& SessionGenerate(SessionId_t& u1) { u1.uid = boost::uuids::random_generator()(); return u1; }
	static SessionId_t& SessionClear(SessionId_t& u1) { u1.uid = boost::uuids::nil_generator()(); return u1; }
	static bool SessionIsNull(const SessionId_t& u1) { return u1.uid.is_nil(); }
};

const int UUID_STR_LEN = 36;

static void UnParseSessionIdToSessionIdStr(char* sessionIdStr, SessionId_t sessionId)
{
	std::string tmp = boost::lexical_cast<std::string>(sessionId.uid);
	memcpy(sessionIdStr, tmp.c_str(), UUID_STR_LEN);
}

//////////CustomGetCurrentTime

typedef long Time_t;

#if OS_PLATFORM == PLATFORM_WINDOWS

#include <chrono>

static Time_t CustomGetCurrentTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

#else
#include <sys/time.h>

static Time_t CustomGetCurrentTime()
{
	struct timeval  tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	return (Time_t)((tv.tv_sec) * 1000);
}
#endif

#endif //__NETWORK_DEFINES_H__
