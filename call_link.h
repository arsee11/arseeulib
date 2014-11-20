//file:call link
#ifndef CALL_LINK_H
#deifne CALL_LINK_H

namespace arsee
{

class CallLink
{
public:
	CallLink(CallLink *link):_next(link){}
	CallLink():_next(NULL){}

	CallLink& operator+(CallLink *rhs)
	{
		CallLink *link = _next;
		while(link._next= NULL );
			link = link._next;
			
		link._next = rhs;
		return (*this);
	}
	
	template<class T>
	ret_t Call(const T &t)
	{
	}
	
protected:	
	CallLink *_next;
};

}/*namespace*/
#endif /*CALL_LINK_H*/