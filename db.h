//db.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee


#ifndef DB_H
#define DB_H

#if defined(_MSC_VER_)
#pragma once
#endif

#ifndef MYSQLDB_H
#include "MysqlDb.h"
#endif


#include <memory>
#include <vector>

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN

//////////////////////////////////////////////////////////////////////
template<class T>
class RowIterator
{
	typedef RowIterator<T> my_t;
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
		_row = _t->next();
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
		return _row == rhs._row;
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
//只有一个result_set(结果集)
template<class Result, class Row>
class ResultWrapp
{
	typedef ResultWrapp<Result_SET, Row> my_t;
	
public:
	typedef std::auto_ptr<my_t> pointer;	
	typedef Result result_t;	
	typedef Row db_row_t;
	typedef RowIterator<my_t> iterator;	
	
	ResultSetWrapp(Result_SET &res)
	{
		_res = res;
	}

	Result_SET* operator->()
	{
		return _res;
	}

	~ResultSetWrapp()
	{
		_res.Release();
	}

	iterator begin()
	{
		return iterator(this, _res.Begin());
	}
	
	iterator end()
	{
		return iterator(this, _res.End() );
	}
	
	///next row
	db_row_t next()
	{
		return _res.next();
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

	result_t _res;
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
	bool Open(const char *ip, unsigned short port, const char* db, const char *user, const char *pwd)
	{
		return _db.Open(ip, port, db, user, pwd);
	}

	template<class Handle>
	void Open(Handle *h)
	{
		_db.Open(h);
	}

	result_set_t* Query(const char *query_str)
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

NAMESP_END

#endif/*DB_H*/