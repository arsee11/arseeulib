//@file buffer_wrapper.cpp


#include "../buffer_wrapper.h"
#include <string.h>

NAMESP_BEGIN

void BufferWrapper::put(const char* buf, size_t size)
{
	if(buf == nullptr || size == 0 )
		return;

	if(_size + size > _maxsize)
		return;

	if(_wptr+size > _end)
	{
		memcpy(_wptr, buf, _end-_wptr);
		memcpy(_buf, buf+(_end-_wptr), size-(_end-_wptr));
		_wptr = _buf+size-(_end-_wptr);
	}
	else
	{
		memcpy(_wptr,  buf, size);
		_wptr += size;
	}
	_size += size;
}

size_t BufferWrapper::get(char* buf, size_t size)
{
	if(buf == nullptr || size == 0 )
		return 0;

	if(_size == 0)
		return 0;

	if( _size < size)
	{
		int len = geti(buf, _size);
		_size == 0;
		return len;
	}
	
	geti(buf, size);
	_size -= size;
	return size;
	
}

size_t BufferWrapper::geti(char* buf, size_t size)
{
	if(_rptr+size > _end)
	{
		memcpy(buf, _rptr, _end-_rptr);
		memcpy(buf+(_end-_rptr), _buf, size-(_end-_rptr));
		_rptr = _buf+(size-(_end-_rptr));
	}
	else
	{
		memcpy(buf, _rptr, size);
		_rptr += size;	
	}

	return size;
}

const char* const BufferWrapper::rptr(long n)
{
	if(_size < n)
		return nullptr;

	if( _rptr + n > _end)
		_rptr = _buf+(n-(_rptr-_end));
	else
		_rptr += n;

	_size -= n;
	return _rptr;
}
NAMESP_END

