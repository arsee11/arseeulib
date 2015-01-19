//file: table.h

#ifndef TABLE_H
#define TABLE_H

#ifndef ARRAY_H
#include "array.h"
#endif

/////////////////////////////////////////////////////
namespace arsee
{

////////////////////////////////////////////////////////////
//table functions

////////////////////////////////////////////////////////////
template<class T>
class Table : 
	public Array< Array<T> >
{
public:
	typedef T value_type;
	typedef T* pointer;
	typedef const pointer const_pointer;
	typedef T& reference;
	typedef const reference const_reference;
	
	typedef std:string string_t;
	typedef Array<std::string> header_t;
	typedef Array<value_type> row_t;
	typedef Array<T> column_t;
	typedef Table<value_type> my_t;
	
private:
	struct desc_cmp_t
	{
		desc_cmp_t(int colidx)
			:_colidx(colidx)
		{}

		int operator()(const row_t &row1, const row_t &row2)
		{
			if( _colidx >= row1.size() || _colidx >= row1.size() )
				return false;

			return row1[_colidx] > row2[_colidx];
		}

		int _colidx;
	};

	struct asc_cmp_t
	{
		asc_cmp_t(int colidx)
			:_colidx(colidx)
		{}

		int operator()(const rows_t &row1, const rows_t &row2)
		{
			if(_colidx >= row1.size() || _colidx >= row2.size() )
				return false;

			return row1[_colidx] < row2[_colidx];
		}

		int _colidx;

	};
	
public:
	//Construct table and init header.
	//@headers headernamses like "col1|col2|col3|...|coln" .
	Table(const string_t &headers)
	{
		Splitor::value_type fields = Splitor::Split("|", headers);
		_headers = fileds;
	}
	
	//Construct table and init header.
	//std::iterator range[start, end).
	template<class STD_ITERATOR>
	Table(STD_ITERATOR &start, STD_ITERATOR &end)
	{
		
	}
	
	enum sort_type{ ASC, DES };
	
	//@col column idx.
	//@st ASC/DES.
	//value_type must supports operator > and <.
	void Sort(size_t col, sort_type st)
	{
		if(col >= size() )
			reutrn;
		
		if(st == ASC)
			std::sort(this->begin(), this->end(), asc_cmp_t(col) ); 
		else if(st == DES)
			std::sort(this->begin(), this->end(), desc_cmp_t(col));
	}
	
	//@col column name.
	//@st ASC/DES.
	//value_type must supports operator > and <.
	void Sort(const string &col, sort_type st)
	{
		long i = _headers.Idx(col);
		if(i == -1) 
			return;
			
		Sort(i, st);
	}
	
	my_t GroupBy(...)
	{
	}
	
	string Head(size_t idx)
	{
		return _headers[idx];
	}
	
	size_t DelCol(size_t idx)
	{
		iterator it = begin();
		for(; it!=end(); ++it)
			(*it).Del(idx);
		
		return idx;
	}
	
	long DelCol(const string& col)
	{
		long i = _headers.Idx(col);
		if(i == -1) 
			return -1;
		
		DelCol(i);
		return i;
	}
	
	size_t AddCol(const string &col, const column_t &colv)
	{
		_headers.push_back(col);
		//hard...
	}
	
	column_t GetCol(size_t ncol)
	{
		column_t col;
		iterator it = begin();
		for(; it!=end(); ++it)
			col.push_back( (*it)[ncol] ); // 不是整齐的table 怎么处理
			
		return col;
	}
	
	column_t GetCol(const string &colname)
	{
		column_t col;
		long i = _headers.Idx(col);
		if(i != -1) 
			col = GetCol(i);
		
		return col;
	}
	
	template<class STD_ITERATOR>
	size_t AddCol(const string &col, STD_ITERATOR &start, STD_ITERATOR &end);
	
	bool Find(const_reference value, size_t *x, size_t *y)
	{
		*x=0;
		*y=0;
		bool isfind = false;
		iterator it = begin();
		for(; it!=end(); ++it)
		{
			(*x)++;
			row_t::iterator itrow = (*it).begin();
			for(; itrow!=(*it).end(); ++itrow )
			{
				(*y)++;
				if( (*itrow) == value )
				{
					isfind = true;
					break;
				}
			}
		
			if(isfind)
				break;
		
			*y = 0;
		}
		
		return isfind;		
	}
	
private:
	my_t& Copy( const my_t& other);
	
private:
	header_t _headers;
};

}/*namespace*/

#endif /*TABLE_H*/