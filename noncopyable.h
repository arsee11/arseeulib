//noncopyable.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-11-12
//create 
//****************************

namespace arsee{

class noncopyable
{
protected:
  noncopyable() {}
  ~noncopyable() {}
private:
  noncopyable( const noncopyable& );
  const noncopyable& operator=( const noncopyable& );
};

}/*namespace arsee*/