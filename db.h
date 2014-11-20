#ifndef DB_H
#define DB_H

#ifndef MYSQLDB_H
#include "MysqlDb.h"
#endif


#include <memory>
#include <vector>

#define DB_TYPE MYSQL_T

#define MYSQL_T 0

template<class DB_T>
class Db;


////////////////////////////////////////////////////////////////////////////////////////
template<int db>
struct DbTrait;

template<>
struct DbTrait<MYSQL_T>
{
	typedef  Db<MysqlDb> db_t;
};

//////////////////////////////////////////////////////////////////////
template<class T>
class DbRowIterator
{
	typedef DbRowIterator<T> my_t;
	typedef typename T::db_row_t::column_t column_t;
	typedef typename T::db_row_t row_t;

public:
	my_t()
	:_t(NULL)
	{}
	
	my_t(T *t, row_t &row)
	{
		set(t, row);
	}

	my_t(const my_t &other)
	{
		_t = other._t;
		_row = other._row;
	}

	my_t& operator=(const my_t &rhs)
	{
		_t = rhs._t;
		_row = rhs._row;
		return (*this);
	}

	my_t& operator++()
	{
		_row = _t->NextRow();
		return (*this);
	}
	
	my_t& operator--()
	{
	}

	T operator*()
	{
		return (*_t);
	}
	
	bool operator==(const my_t &rhs)
	{
		return row == rhs._row;
	}
	
	bool operator!=(const my_t &rhs)
	{
		return _row != rhs._row;
	}
	
	column_t operator[](int i) throw(exception)
	{
		if( i<0 || i>=_t->ColsCount() )
			throw exception("idx out of rang");

		return _row.At(i);
	}

	column_t& operator[](const char *col_name)
	{
		
	}

private:
	void set(T *t, typename T::db_row_t &row)
	{	
		_t = t;	
		_row = row;
	}

	void swap(const my_t &lhs, const my_t &rhs)
	{
	}
	
	T *_t;
	row_t _row;
};	


////////////////////////////////////////////////////////////////////////////////////////
template<class RETSULT_SET, class ROW>
class ResultSetWrapp
{
	typedef ResultSetWrapp<RETSULT_SET, ROW> my_t;
	
public:
	typedef std::shared_ptr<my_t> pointer;	
	typedef RETSULT_SET result_set_t;	
	typedef ROW db_row_t;
	typedef DbRowIterator<my_t> iterator;
	
	
	ResultSetWrapp(RETSULT_SET &res)
	{
		_res = res;
	}

	RETSULT_SET* operator->()
	{
		return _res;
	}

	~ResultSetWrapp()
	{
		_res.Release();
	}

	iterator Begin()
	{
		return iterator(this, _res.Begin());
	}
	
	iterator End()
	{
		return iterator(this, _res.End() );
	}

	db_row_t NextRow()
	{
		return _res.NextRow();
	}
	
	db_row_t Row(unsigned i)
	{
	}

	unsigned ColsCount()
	{
		return _res.ColsCount();
	}

	unsigned RowsCount()
	{
		return _res.RowsCount();
	}

private:
	void swap(const my_t &lhs, const my_t &rhs)
	{
	}

	result_set_t _res;
};	





////////////////////////////////////////////////////////////////////////////////////////
template<class DB_T>
class Db
{
public:
	typedef DB_T db_t;
	typedef typename db_t::db_row_t db_row_t;
	typedef typename ResultSetWrapp<typename db_t::result_set_t, db_row_t>::pointer presult_set_t;	
	typedef typename ResultSetWrapp<typename db_t::result_set_t, db_row_t> result_set_t;	

public:
	bool Open(const char* db, const char *user, const char *pwd)
	{
		return _db.Open(db, user, pwd);
	}

	result_set_t* ExecuteQuery(const char *query_str)
	{
		return new result_set_t( _db.ExecuteQuery(query_str) );
	}

	int Update(const char *query_str)
	{
		return _db.Update(query_str);
	}

	bool PreStatement(const char *query_str)
	{
		return false;
	}

	bool BindParam()
	{
		return false;
	}

	//@return failed NULL, success res 
	result_set_t* Execute()
	{
		return NULL;
	}

	bool BeginTrans()
	{
		return _db.BeginTrans();
	}

	bool RollBack()
	{
		return _db.RollBack();
	}

	bool Commit()
	{
		return _db.Commit();
	}

	void Close()
	{
		_db.Close();
	}

private:
	db_t _db;
};


#endif/*DB_H*/