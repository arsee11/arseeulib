
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

	void put(const char* buf, size_t size)
	{
		if(buf == nullptr || size == 0 )
			return;

		if(_size + size > _maxsize)
			return;

		if(_wptr+size > _end)
		{
			memcpy(_wptr, buf, _end-_wptr);
			memcpy(_buf, buf+(_end-_wptr), size-(_end-_wptr));
			_wptr = _buf+(_end-_wptr);
		}
		else
		{
			memcpy(_wptr,  buf, size);
			_wptr += size;
		}
		_size += size;
	}

	size_t get(char* buf, size_t size)
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

	size_t geti(char* buf, size_t size)
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

	const char* const rptr()const { return _rptr; }
	const char* const rptr(long n)
	{
		if(_size < n)
			return nullptr;
	
		if( _rptr + n > _end)
		{
			_rptr = _buf+(n-(_rptr-_end));
		}
		else
		{
			_rptr += n;
		}

		_size -= n;
		
		return _rptr;
	}

public:
	size_t _maxsize;
	size_t _size = 0;
	char* _buf;
	char* _end;
	char* _rptr; //read pointer
	char* _wptr; //write pointer
};
