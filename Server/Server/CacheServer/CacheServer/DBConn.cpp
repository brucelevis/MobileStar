//
//  DBConn.cpp
//  ServerLib
//
//  Created by jooyc on 13. 9. 21..
//  Copyright (c) 2013년 Team ORIYEL. All rights reserved.
//

#include "DBConn.h"
#include "Log.h"
#include <my_global.h>

#define MYSQL_ERROR_FMT         "(%d) %s"
#define MYSQL_ERROR_ARG(conn)   mysql_errno(conn), mysql_error(conn)


DBConn::DBConn()
{
    _conn = NULL;
}

DBConn::~DBConn()
{
    if (_conn)
        mysql_close(_conn);
}

bool DBConn::Initialize(const char* host, const char* user, const char* pwd, const char* db, const int port)
{
    _conn = mysql_init(NULL);
    if (NULL == _conn)
    {
        ErrorLog("mysql init fail");
        return false;
    }
    
    //uint timeout = 10;
    //mysql_options(_conn, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
    
    if (NULL == mysql_real_connect(_conn, host, user, pwd, db, port, (const char*)NULL, 0 /* CLIENT_INTERACTIVE */))
    {
        MysqlErrorLog("mysql conn error");
        return false;
    }
    
    return true;
}

void DBConn::ProcessResultSet(MYSQL_RES* resultSet)
{
    MYSQL_ROW row;
    
    while ((row = mysql_fetch_row(resultSet)) != NULL)
    {
        for (int i = 0; i < mysql_num_fields(resultSet); i++)
        {
            DebugLog("%s", row[i] != NULL ? row[i] : "NULL");
        }
    }
    
    if (mysql_errno(_conn) != 0)
    {
        MysqlErrorLog("mysql_fetch_row() failed");
    }
    else
    {
        DebugLog("%lu rows returned", (unsigned long)mysql_num_rows(resultSet));
    }
}

bool DBConn::ProcessNoResultQuery(const char* query, unsigned long long* autoIncrementId)
{
    DebugLog("%s", query);
    
    if (0 != mysql_query(_conn, query))
    {
        MysqlErrorLog("could not excute query");
        return false;
    }
    
    if (NULL != autoIncrementId)
    {
        *autoIncrementId = mysql_insert_id(_conn);
    }
    
    if (mysql_field_count(_conn) == 0)
    {
        //DebugLog("%lu rows affected", (unsigned long)mysql_affected_rows(_conn));
        return true;
    }
    else
    {
        MysqlErrorLog("could not retrieve result set");
        return false;
    }
}

bool DBConn::ProcessResultQuery(const char* query, MYSQL_RES** resultSet)
{
    //DebugLog("%s", query);
    if (0 != mysql_query(_conn, query))
    {
        MysqlErrorLog("could not excute query");
        return false;
    }
    
    *resultSet = mysql_store_result(_conn);
    if (NULL == *resultSet)
    {
        if (mysql_field_count(_conn) != 0)
        {
            MysqlErrorLog("could not retrieve result set");
            return false;
        }
    }
    
    return true;
}

void DBConn::MysqlErrorLog(const char* msg)
{
    ErrorLog("%s, " MYSQL_ERROR_FMT, msg, MYSQL_ERROR_ARG(_conn));
}

