#include "stdafx.h"

#ifndef MYSQLDB_H
#include "MysqlDb.h"
#endif	



#include <iostream>
using namespace std;

//#define LOCAL

bool MysqlDb::Open(const char *ip, unsigned short port, const char* db, const char *user, const char *pwd)
{
	_mysql = mysql_init(0);

	_connect = mysql_real_connect(_mysql, ip, user, pwd,db, port, 0, 0);

	if(_connect == NULL)
	{
		ErrorHandle();
		//cout<<mysql_error(_mysql)<<endl;
		return false;
	}

	return true;
}

MysqlDb::result_set_t MysqlDb::ExecuteQuery(const char *query_str)
{
	//ADD_LOG(query_str);
	mysql_query(_connect, query_str);
	MYSQL_RES *res = mysql_store_result(_connect);
	if(res == NULL)
	{
		//ADD_LOG(query_str);
		ErrorHandle();
		return result_set_t(NULL, NULL);
	}
	return result_set_t(this, res);
}

int MysqlDb::Update(const char *query_str)
{	
	//ADD_LOG(query_str);
	if( mysql_query(_connect, query_str) != 0 )
	{
		//ADD_LOG(query_str);
		ErrorHandle();
		return -1;
	}

	if( mysql_affected_rows(_connect) == (my_ulonglong)-1)
	{
		//ADD_LOG(query_str);
		ErrorHandle();
		//cout<<mysql_error(_mysql)<<endl;
	}
	return (int)mysql_affected_rows(_connect);
}

bool MysqlDb::BeginTrans()
{
	if(Update("start transaction")  < 0 )
	{
		ErrorHandle();
		return false;
	}
	return true;
}

bool MysqlDb::RollBack()
{
	if(Update("rollback")  < 0 )
	{
		ErrorHandle();
		return false;
	}
	return true;
}

bool MysqlDb::Commit()
{
	if(Update("commit")  < 0 )
	{
		ErrorHandle();
		return false;
	}
	return true;
}

void MysqlDb::Close()
{
	mysql_close(_mysql);
}

void MysqlDb::ErrorHandle()
{
	cout<<( mysql_error(_mysql) )<<endl;
}

 
