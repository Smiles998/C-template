#ifndef SIMPLEREFCOUNT_H
#define SIMPLEREFCOUNT_H

#include <cstddef>
#include "alloc.h"

//һ���򵥵ļ�����
class SimpleReferenceCount
{
private:
	size_t* counter;

public:
	SimpleReferenceCount()
	{
		counter = nullptr;
	}

	//�������������������ֵ��ʼ��Ϊ1
	template<typename T>
	void init(T*)
	{
		counter = alloc_counter();//���������
		*counter = 1;
	}

	//�ͷŸü�����
	template<typename T>
	void dispose(T*)
	{
		dealloc_counter(counter);
	}

	//������+1��
	template<typename T>
	void increment(T*)
	{
		++*counter;
	}

	//������-1��
	template<typename T>
	void decrement(T*)
	{
		--*counter;
	}

	//��������ֵ�Ƿ�Ϊ0��
	template<typename T>
	bool is_zero(T*)
	{
		return *counter == 0;
	}



};



#endif