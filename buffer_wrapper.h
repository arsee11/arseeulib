//@file buffer_wrapper.h

#ifndef BUFFER_WRAPPER_H
#define BUFFER_WRAPPER_H

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

#include <cstddef>

NAMESP_BEGIN

//cycle buffer implements
class BufferWrapper
{
public:
	BufferWrapper(size_t max_size)
		:_maxsize(max_size)
	{
		_buf = new char[_maxsize];
		_end = _buf+_maxsize;
		_rptr = _buf;
		_wptr = _rptr;
	}

	~BufferWrapper()
	{
		delete[] _buf;
	}
	
	size_t size(){ return _size;}

	void put(const char* buf, size_t size);
	size_t get(char* buf, size_t size);
	const char* const rptr()const { return _rptr; }
	const char* const rptr(long n);

private:
	size_t geti(char* buf, size_t size);

	size_t _maxsize;
	size_t _size = 0;
	char* _buf;
	char* _end;
	char* _rptr; //read pointer
	char* _wptr; //write pointer
};

NAMESP_END

#endif/*BUFFER_WRAPPER_H*/
