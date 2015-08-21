//dbmodel.cpp
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

#include "dbmodel.h"
#include <exception>

NAMESP_BEGIN

string DbModel::get_params()const
{
	string params = "";
	attr_map_t::const_iterator it = _attributes.begin();
	for(int i=0; it != _attributes.end(); ++it,++i)
	{
		if(i == 0)
			params +=it->first;
		else
			params += ","+it->first;
	}

	return params;
}

string DbModel::get_values()const
{
	string values = "";
	attr_map_t::const_iterator it = _attributes.begin();
	for(int i=0; it != _attributes.end(); ++it,++i)
	{
		if(i == 0)
		{
			if(it->second->accept_null_ && it->second->isnull_)
				values += "null";
			else
				values += "'"+it->second->str()+"'";
		}
		else
		{
			if (it->second->accept_null_ && it->second->isnull_)
				values += ",null";
			else
				values += ",'"+it->second->str()+"'";
		}
	}
   
	return values;
}

DbModel::Attribute* DbModel::get_attr(const string& key)const
{
	attr_map_t::const_iterator i = _attributes.find(key);
	if(i != _attributes.end() )
	{
		return i->second.get();
	}
	
	throw exception(string("attribute["+key+"] not found").c_str());
}

void DbModel::set_attr(const string& key, const Attribute* val)
{
	attr_map_t::iterator i = _attributes.find(key);
	if(i != _attributes.end() )
	{
		i->second->str( val->str() );
		return;
	}

	throw exception(string("attribute["+key+"] not found").c_str());
}

void DbModel::set_attr(const string& key, const string& val)
{
	attr_map_t::iterator i = _attributes.find(key);
	if(i != _attributes.end() )
	{
		i->second->str(val);
		return;
	}

	throw exception(string("attribute["+key+"] not found").c_str());
}

NAMESP_END