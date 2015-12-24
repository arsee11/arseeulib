//iobject.h
//copyright	: Copyright (c) 2015 arsee.
//license	: GNU GPL v2.
//author	: arsee


#ifndef IOBJECT_H
#define IOBJECT_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif


NAMESP_BEGIN

class ClassInfoBase;

class IObject
{  
public:
	virtual ~IObject(){};
	virtual const char* get_class_name()=0;
	virtual ClassInfoBase* get_class_info()=0;
	virtual IObject& operator+=(const IObject& rhs)=0;
};

NAMESP_END;

#endif /*IOBJECT_H*/
