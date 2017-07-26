//class_serialize_handler.h
//copyright	: Copyright (c) 2015 arsee.
//license	: GNU GPL v2.
//author	: arsee

#ifndef CLASS_SERAILIZE_HANDLER_H
#define CLASS_SERAILIZE_HANDLER_H

#include <string>
#include <map>
#include "rqhlib/json/json.h"

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

#ifndef CLASS_SERAILIZE_H
#include "class_serialize.h"
#endif

NAMESP_BEGIN

typedef std::string (*Serialize)(void* memory, size_t size);

inline std::string code(const char* buf, int size)
{
	std::string str;
	for (int i = 0; i < size; ++i)
	{
		char ch[8] = { 0 };
		sprintf(ch, "%02x", (unsigned char)buf[i]);
		str += ch;		
	}

	return str;
}

inline std::string SerializeBase(void* buf, size_t size)
{
	return "\""+code((const char*)buf, size)+"\"";
}

inline std::string SerializeString(void* buf, size_t size)
{
	string str="\"";
	str+=(const char*)buf;
	str+="\"";
	return str;
}


inline int xtoi(char xch)
{
	if (xch == 'A' || xch == 'a')
		return 10;

	if (xch == 'B' || xch == 'b')
		return 11;

	if (xch == 'C' || xch == 'c')
		return 12;

	if (xch == 'D' || xch == 'd')
		return 13;

	if (xch == 'E' || xch == 'e')
		return 14;

	if (xch == 'F' || xch == 'f')
		return 15;

	return atoi(&xch);
}

inline char atobx(const char byte[2])
{
	char high = xtoi(byte[0]);
	char low = xtoi(byte[1]);

	return high * 16 + low;
}

inline const char* encode(const std::string& str, int size)
{
	char *buf = new char[size];
	memset(buf, 0, size);
	char byte[2] = { 0 };
	for (int i = 0, j=0; i < size*2; i++){		
		byte[i%2] = str[i];
		if (i % 2 != 0)
		{
			buf[j] = atobx(byte);
			++j;
		}
	}

	return buf;
}

inline void* UnserializeBase(const char* buf, size_t size)
{
	return (void*)encode((const char*)buf, size);
}

inline void* UnserializeString(const char* buf, size_t size)
{
	char* val = new char[strlen(buf)+1];
	strcpy(val, buf);
	return (void*)val;
}


struct UsBase
{
	virtual ~UsBase(){}
	virtual void* operator()(Json::Value& schema, const std::string&attr_name, std::string& attr_type , size_t size)
	{
		return UnserializeBase(schema.asString().c_str(), size);
	}
};

struct UsString:UsBase
{
	virtual void* operator()(Json::Value& schema, const std::string&attr_name, std::string& attr_type , size_t size)
	{
		return UnserializeString(schema.asString().c_str(), size);
	}
};

class SerializeHandler
{
public:
	typedef std::map<std::string, Serialize> handler_map_t;;
	typedef std::map<std::string, UsBase*> us_map_t;;

	static SerializeHandler& instance()
	{
		static SerializeHandler _myselft;
		return _myselft;
	}
	
	std::string Handle(const char* class_name, void* memory, size_t size)
	{
		handler_map_t::iterator i = _maps.find(class_name);
		if( i != _maps.end() )
			return (i->second)(memory, size);
		
		return std::string();
	}

	void* HandleUs(Json::Value& schema, const std::string&attr_name, std::string& attr_type , size_t size)
	{
		us_map_t::iterator i = _usmaps.find(attr_type);
		if( i != _usmaps.end() )
			return (i->second)->operator()(schema, attr_name, attr_type, size);
		
		return nullptr; 
	}

	void Registry(const char* class_name, Serialize handle)
	{
		std::string name = class_name;
		_maps[name] = handle;
	}
	void RegistryUs(const char* class_name, UsBase* handle)
	{
		std::string name = class_name;
		_usmaps[name] = handle;
	}

private:
	SerializeHandler()
	{
		Registry( typeid(int).name(),SerializeBase);
		Registry( typeid(float).name(), SerializeBase);
		Registry( typeid(std::string).name(), SerializeString);

		RegistryUs( typeid(int).name(),new UsBase);
		RegistryUs( typeid(float).name(),new UsBase);
		RegistryUs( typeid(std::string).name(),new UsString);
	}

	~SerializeHandler()
	{
		for(auto i:_usmaps)
			delete i.second;
	}

	handler_map_t _maps;
	us_map_t _usmaps;

};	

inline std::string ss(std::string&& class_name, void* memory, size_t size)
{
	return SerializeHandler::instance().Handle(
		class_name.c_str(), (void*)memory, size);
}


template<class V>
std::string SerializeVector(V& v, size_t size)
{
	std::string stream="{ \"v\":[";

	std::string element_type = Typeof<typename V::value_type>()();
	size_t i=0;	
	for(i=0; v.size()>0&&i<v.size()-1; i++)
	{
		typename V::value_type e = v[i];
		stream+=ss(element_type.c_str(),(void*)&e, sizeof(e))+",";
	}
	if(i>0)
	{
		typename V::value_type e = v[i];
		stream+=ss(element_type.c_str(),(void*)&e, sizeof(e));
	}

	stream+="]}";

	return stream;
}


template<class T>
struct Coder
{
	std::string code(void* buf, size_t size)
	{
		T* obj = static_cast<T*>(buf);
		return ClassSerializer<T>()(ss, *obj);
	}
};

template<class T>
struct Coder<shared_ptr<T> >
{
	std::string code(void* buf, size_t size)
	{
		shared_ptr<T>* pobj = static_cast< shared_ptr<T>*>(buf);
		T* obj = (*pobj).get();
		return ClassSerializer<T>()(ss, *obj);
	}
};

inline void* us(Json::Value& schema, const std::string&attr_name, std::string&& attr_type , size_t size)
{
	return SerializeHandler::instance().HandleUs(schema,attr_name, attr_type, size);
}

template<class T>
struct Us:UsBase
{
	void* operator()(Json::Value& schema, const std::string&attr_name, std::string& attr_type , size_t size)
	{
		return ClassUnserialize<T>()(us, schema );
	}
};

template<class T>
struct Us<shared_ptr<T> >:UsBase
{
	void* operator()(Json::Value& schema, const std::string&attr_name, std::string& attr_type , size_t size)
	{
		shared_ptr<T>* pobj = new shared_ptr<T>( ClassUnserialize<T>()(us, schema ) );
		return pobj;
	}
};

template<class T>
struct Us<vector<T> >:UsBase
{
	void* operator()(Json::Value& schema, const std::string&attr_name, std::string& attr_type , size_t size)
	{
		typedef vector<T> v_t;
		typedef typename v_t::value_type e_t;
		v_t *v = new v_t;
		
		std::string name = "";
		std::string type = Typeof<e_t>()();
		try{
			Json::Value array = schema["v"];
			for(int i=0; i<array.size(); i++)
			{
				e_t* e = (e_t*)us(array[i], name, type.c_str(), sizeof(e_t) );
				v->push_back(*e);
			}
		}
		catch(std::runtime_error& e){
			cout<<__FILE__<<":"<<__LINE__<<" "<<e.what()<<endl;
			delete v;
			v=nullptr;
		}
	
		return (void*)v;
	}
};

NAMESP_END

#endif/*CLASS_SERAILIZE_HANDLER_H*/
