////class_info_test.cpp
//
//#include "../class_info.h"
//#include <iostream>
//
//using namespace std;
//using namespace arsee;
//
//#define BUILD_ATTR(TYPE, NAME) \
//        TYPE get_##NAME(){ return NAME; }\
//        void set_##NAME(const TYPE& val){ NAME = val; }\
//        TYPE NAME
//
//#define REGISTER_ATTR(CLASS, TYPE, NAME) \
//        class_info.add_attr<TYPE, &CLASS::get_##NAME, &CLASS::set_##NAME>(#NAME, this)
//
//
//extern char const class_Object[]="Object";
//
//struct Object
//{
//        Object()
//        {
//                REGISTER_ATTR(Object, int, a);
//                REGISTER_ATTR(Object, float,b);
//        }
//
//        BUILD_ATTR(int,   a);
//        BUILD_ATTR(float, b);
//
//        ClassInfo<Object,class_Object> class_info;
//};
//
//
//int main()
//{
//        Register<Object, class_Object> r = ClassInfo<Object, class_Object>::r;
//        Object *o = (Object*)ClassInfoBase::get_object("Object");
//
//       
//
//        int a = 1111;
//        o->class_info.attrs["a"]->set((char*)&a);
//        float b = 11.11;
//        o->class_info.attrs["b"]->set((char*)&b);
//
//		ClassInfoBase::attr_iterator i = o->class_info.attrs.begin();
//		for (; i != o->class_info.attrs.end(); ++i)
//		{
//			const char* buf = i->second->get();
//			cout << i->first << ":" << buf << endl;
//		}
//
//        cout<<"a:"<<o->get_a()<<",b:"<<o->get_b()<<endl;
//}