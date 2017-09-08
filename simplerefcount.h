#ifndef SIMPLEREFCOUNT_H
#define SIMPLEREFCOUNT_H

#include <cstddef>
#include "alloc.h"

//一个简单的计数器
class SimpleReferenceCount
{
private:
	size_t* counter;

public:
	SimpleReferenceCount()
	{
		counter = nullptr;
	}

	//分配计数器，并把它的值初始化为1
	template<typename T>
	void init(T*)
	{
		counter = alloc_counter();//分配计数器
		*counter = 1;
	}

	//释放该计数器
	template<typename T>
	void dispose(T*)
	{
		dealloc_counter(counter);
	}

	//计数器+1：
	template<typename T>
	void increment(T*)
	{
		++*counter;
	}

	//计数器-1：
	template<typename T>
	void decrement(T*)
	{
		--*counter;
	}

	//检查计数器值是否为0：
	template<typename T>
	bool is_zero(T*)
	{
		return *counter == 0;
	}



};



#endif