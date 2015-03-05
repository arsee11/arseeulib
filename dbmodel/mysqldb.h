#ifndef MYSQLDB_H
#define MYSQLDB_H

#include "../mysql/mysql.h"

struct MysqlRow;
struct MysqlRes;

////////////////////////////////////////////////////////////////////////////////////
class MysqlDb
{
public:
	typedef MysqlRes result_set_t;
	typedef MysqlRow db_row_t;
	
	friend struct MysqlRes;

public:
	MysqlDb() { _mysql = mysql_init(0); }

	bool Open(const char *ip, unsigned short port, const char* db, const char *user, const char *pwd);

	void Open(MYSQL *conn){ _mysql = conn; _connect = conn; }
	
	result_set_t ExecuteQuery(const char *query_str);

	int Update(const char *query_str);
	
	void Close();
	
	bool BeginTrans();

	bool RollBack();

	bool Commit();

private:
	void ErrorHandle();

private:
	MYSQL * _mysql;
	MYSQL * _connect;

};


////////////////////////////////////////////////////////////////////////////////////
struct MysqlRow
{
	MysqlRow()
		:row_raw(NULL)
	{}

	MysqlRow(MYSQL_ROW row)
		:row_raw(row)
	{}

	typedef char * column_t;

	bool operator==(const MysqlRow &rhs)
	{
		return row_raw == rhs.row_raw;
	}

	bool operator!=(const MysqlRow &rhs)
	{
		return row_raw != rhs.row_raw;
	}


	MYSQL_ROW row_raw;	
	
	column_t At(int idx){ return row_raw[idx]; }
};


////////////////////////////////////////////////////////////////////////////////////
//a result table
struct MysqlRes
{
	typedef MysqlRow db_row_t;

	MysqlRes()
		:mydb(NULL)
		,res_raw(NULL)
	{}

	MysqlRes(MysqlDb *db, MYSQL_RES *res)
		:mydb(db)
		,res_raw(res)
	{}
	
	MysqlRes(MysqlRes &rhs)
	{
		mydb = rhs.mydb;
		res_raw = rhs.res_raw;
	}

	MysqlRes& operator=(const MysqlRes &rhs)
	{
		mydb = rhs.mydb;
		res_raw = rhs.res_raw;

		return (*this);
	}

	unsigned ColsCount()
	{
		if(res_raw == NULL)
			return 0;

		return res_raw->field_count;
	}

	unsigned RowsCount()
	{
		if(res_raw == NULL)
			return 0;

		return res_raw->row_count;
	}

	/// next row
	db_row_t next()
	{	
		if(res_raw == NULL)
			return db_row_t(NULL);

		return db_row_t( mysql_fetch_row(res_raw) ); 
	}
	
	db_row_t begin( ){ return next(); }

	db_row_t end( )	{ return db_row_t(NULL) ; }

	void Release()
	{
		if(mydb == NULL)
			return;

		do
		{
			 //res_raw=mysql_store_result(mydb->_connect);
			 mysql_free_result(res_raw);
		}while( mysql_next_result(mydb->_connect) == 0 );
	}
	
	MysqlDb *mydb;
	MYSQL_RES *res_raw;
	
};




#endif/*MYSQL_H*/