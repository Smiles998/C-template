#ifndef COUNTERPTRREWRITE_H
#define COUNTERPTRREWRITE_H

#include "simplerefcount.h"
#include "stdobjpolicy.h"

template<typename T,typename CounterPolicy=SimpleReferenceCount,typename ObjectPolicy=StandardObjectPolicy>
class CountingPtr :private CounterPolicy, private StandardObjectPolicy
{
private:
	//typedef两个简单的别名
	typedef CounterPolicy CP;
	typedef ObjectPolicy OP;

	T* object_pointed_to;//所引用的对象，如果没有引用任何对象，则为null

public:
	//缺省构造函数
	CountingPtr()
	{
		this->object_pointed_to = nullptr;
	}

	//一个针对转型的构造函数(转型自一个内建的指针)
	explicit CountingPtr(T* p)
	{
		this->init(p);//保证其根据counter policy转化为一个计数指针
	}

	//拷贝构造函数
	CountingPtr(CountingPtr<T, CP, OP> const& cp)
		:CP((CP const)& cp), OP((OP const&)cp)//拷贝两个policy
	{
		this->attach(cp);
	}

	//析构函数
	~CountingPtr()
	{
		this->detach();//减少计数值，如果计数值为0，则释放该计数器指针
	}

	//拷贝赋值运算符(要考虑给自己赋值)
	CountingPtr<T, CP, OP>& operator=(CountingPtr<T, CP, OP> const& cp)
	{
		if (this->object_pointed_to != cp.object_pointed_to)
		{
			this->detach();//对本对象的计数值进行减1 ,如果计数值为0，则释放计数器
			CP::operator=((CP const&)cp);//对policy进行赋值
			OP::operator=((OP const&)cp);//对policy进行赋值
			this->attach(cp);//拷贝指针并增加计数值
		}
		return *this;
	}

	//使之成为智能指针的运算符
	T* operator->() const
	{
		return this->object_pointed_to;
	}

	T& operator*() const
	{
		return *this->object_pointed_to;
	}
	

	//转型构造函数
	friend 
		template<typename S>  //s可能是void或者T的基类
	CountingPtr(CountingPtr<S,OP,CP> const& cp)
		:OP((OP const&)cp), CP((CP const&)cp), object_pointed_to(cp.object_pointed_to)
	{
		CP::increment(cp.object_pointed_to);
	}

	operator bool() const
	{
		return this->object_pointed_to != (*T)0;
	}






private:
	//辅助函数:用普通指针进行初始化（前提是普通指针存在）
	void init(T* p)
	{
		if (p)
		{
			CounterPolicy::init(p);//将其转化为具有计数功能的指针
		}
		this->object_pointed_to = p;
	}

	//拷贝指针，并且增加计数值（前提是指针存在）
	void attach(CountingPtr<T,CP,OP> const& cp)
	{
		this->object_pointed_to = cp.object_pointed_to;
		if (cp.object_pointed_to)
			CounterPolicy::increment(cp.object_pointed_to);
	}

	//减少计数值，如果计数值为0，则释放该计数器指针
	void detach()
	{
		if (this->object_pointed_to != nullptr)
		{
			CounterPolicy::decrement(this->object_pointed_to);
			if (CounterPolicy::is_zero(this->object_pointed_to))
			{
				//如果有必要的话，释放计数器
				CounterPolicy::dispose(this->object_pointed_to);
				//使用object policy来释放所指向的对象
				ObjectPolicy::dispose(this->object_pointed_to);

			}



		}

	}





};







#endif