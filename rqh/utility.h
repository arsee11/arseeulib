//utility.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-19
//record 
//****************************

#ifndef UTILITY_H
#define UTILITY_H

///////////////////////////////////////////////
//ArgCounter Begin
template<class... args>
struct ArgCounter{};

template<>
struct ArgCounter<> 
{
    enum{ value = 0 };
};

template<class T, class... Args>
struct ArgCounter<T, Args...>
{ 
	enum{ value = 1 + ArgCounter<Args...>::value };
};
//ArgCounter End
///////////////////////////////////////////////
 
 
///////////////////////////////////////////////
//IfElseThen Begin
/*template<bool pre, class A, class B>
struct IfElseThen;

template<class A, class B>
struct IfElseThen<true, A, B>
{
	typedef A value;
};

template<class A, class B>
struct IfElseThen<false, A, B>
{
	typedef B value;
};
*/
//IfElseThen End
///////////////////////////////////////////////


///////////////////////////////////////////////
//ArgIteration Begin
template<class... Args>
struct ArgIteration;

template<class T, class... Args>
struct ArgIteration<T, Args...>
{
	template<class...PARAMS>
	static void Handle(PARAMS&... params)
	{
		T::Execute(params...);		
		ArgIteration<Args...>::Handle(params...);
	}	
};

template<class T>
struct ArgIteration<T>
{
	template<class...PARAMS>
	static void Handle(PARAMS&... params)
	{
		T::Execute(params...);
	}
};


//template<class T, class... Args>
//struct ArgErase<T, Args...>
//{
//	typedef ArgPack<Args...>  result;
//	
//	
//};

//template<class T>
//struct ArgIteration<T>
//{
//	typedef T type;
//};

//ArgIteration End
///////////////////////////////////////////////


///////////////////////////////////////////////
//Iteration Begin
template<int N, template<int>class FUNCTION>
struct Iteration
{
	template<class... PARAMS>
	static void Handle( PARAMS&... Params)
	{
		if(N>1)
			FUNCTION<N-1>()(Params...);
					
		Iteration<N-1, FUNCTION>::Handle(Params...);
	}	
};


template< template<int>class FUNCTION >
struct Iteration<1, FUNCTION >
{
	template< class... PARAMS>
	static void Handle(PARAMS&... Params)
	{
		FUNCTION<0>()(Params...);
	}
};


//template<int N, template<int>class FUNCTION>
//struct Iteration
//{
//	template<class... PARAMS>
//	static auto Handle( PARAMS&... Params)->decltype(FUNCTION<N-1>()(Params...))
//	{
//		if(N>1)
//			auto result = FUNCTION<N-1>()(Params...);
//					
//		Iteration<N-1, FUNCTION>::Handle(Params...);
//	}	
//};
//
//
//template< template<int>class FUNCTION >
//struct Iteration<1, FUNCTION >
//{
//	template< class... PARAMS>
//	static auto Handle(PARAMS&... Params)
//	{
//		FUNCTION<0>()(Params...);
//	}
//};
//Iteration End
///////////////////////////////////////////////


///////////////////////////////////////////////
//ArgAt Begin
template<int N, class... Args>
struct ArgAt;

template<int N, class T, class... Args>
struct ArgAt<N, T, Args...>
{
	template<bool pre, class A, class B>
	struct IfElseThen;

	template<class A, class B>
	struct IfElseThen<true, A, B>
	{
		typedef A value;
	};

	template<class A, class B>
	struct IfElseThen<false, A, B>
	{
		typedef typename B::result value;
	};
		
	typedef typename IfElseThen<N==0, T, ArgAt<N-1,Args...> >::value result;	
};

template<class... Args>
struct ArgPack;

template<class T, class... Args>
struct ArgPack<T, Args...>
{
	typedef ArgPack<T,ArgPack<Args...> > type;		
};

template<class T>
struct ArgPack<T>
{
	typedef ArgPack<T> type;		
};

//ArgAt End
///////////////////////////////////////////////


#endif /*UTILITY_H*/
