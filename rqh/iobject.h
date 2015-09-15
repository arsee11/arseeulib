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

class IObject
{  
public:
	virtual ~IObject()=0;
	virtual const char* get_class_name()=0;
	virtual ClassInfoBase* get_class_info()=0;
};

NAMESP_END;

#endif /*IOBJECT_H*/