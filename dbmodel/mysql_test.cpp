// mysql_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>

using namespace std;

#include "db.h"

int _tmain(int argc, _TCHAR* argv[])
{
	typedef DbTrait<MYSQL_T>::db_t db_t;
	typedef db_t::presult_set_t res_t;
	typedef db_t::result_set_t::iterator row_iterator;
	
	db_t db;
	if( db.Open("students", "root", "123456") )
	{
		db.Update("set names gb2312");
		int ec;

		if( (ec=db.Update("update stutdent set sex =1 where name='' ;")) == -1)
		{
			cout<< "update error"<<endl;
		}
		cout<<"ec:"<<ec<<endl;

		if( (ec=db.Update("update student set sex =1 where name='' ;")) == -1)
		{
			cout<< "update error"<<endl;
		}
		cout<<"ec:"<<ec<<endl;

		if( (ec=db.Update("update student set sex =0 where name='王五' ;")) == -1)
		{
			cout<< "update error"<<endl;
		}
		cout<<"ec:"<<ec<<endl;

		res_t res( db.ExecuteQuery("select * from student") );
		row_iterator it = res->Begin();
		while( it != res->End() )
		{
			//operator[] test
			try
			{
				cout <<it[3] <<"|"<<it[-1]<<endl;
			}  
			catch(exception &e)
			{
				cout<<e.what()<<endl;
			}

			try
			{
				cout <<it[-1]<<endl;
			}
			catch(exception &e)
			{
				cout<<e.what()<<endl;
			}

			cout << it[0]<<"|"<<it[1] <<"|"<<it[2] <<endl;
			++it;
		}
	}

	//release  test

	return 0;
}

