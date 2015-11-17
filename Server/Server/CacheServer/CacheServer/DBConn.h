//
//  DBConn.h
//  ServerLib
//
//  Created by jooyc on 13. 9. 21..
//  Copyright (c) 2013년 Team ORIYEL. All rights reserved.
//

#ifndef __DB_CONN_H__
#define __DB_CONN_H__

#include <mysql.h>


class DBConn
{
public:
    DBConn();
    ~DBConn();
    
    bool Initialize(const char* host, const char* user, const char* pwd, const char* db, const int port);
    
public:
    bool ProcessNoResultQuery(const char* query, unsigned long long* autoIncrementId = 0);
    bool ProcessResultQuery(const char* query, MYSQL_RES** resultSet);
    
    void ProcessResultSet(MYSQL_RES* resultSet);
    
    void MysqlErrorLog(const char* msg);
    
private:
    MYSQL* _conn;
};

#endif  // __DB_CONN_H__
