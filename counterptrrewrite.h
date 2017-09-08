#ifndef COUNTERPTRREWRITE_H
#define COUNTERPTRREWRITE_H

#include "simplerefcount.h"
#include "stdobjpolicy.h"

template<typename T,typename CounterPolicy=SimpleReferenceCount,typename ObjectPolicy=StandardObjectPolicy>
class CountingPtr :private CounterPolicy, private StandardObjectPolicy
{
private:
	//typedef�����򵥵ı���
	typedef CounterPolicy CP;
	typedef ObjectPolicy OP;

	T* object_pointed_to;//�����õĶ������û�������κζ�����Ϊnull

public:
	//ȱʡ���캯��
	CountingPtr()
	{
		this->object_pointed_to = nullptr;
	}

	//һ�����ת�͵Ĺ��캯��(ת����һ���ڽ���ָ��)
	explicit CountingPtr(T* p)
	{
		this->init(p);//��֤�����counter policyת��Ϊһ������ָ��
	}

	//�������캯��
	CountingPtr(CountingPtr<T, CP, OP> const& cp)
		:CP((CP const)& cp), OP((OP const&)cp)//��������policy
	{
		this->attach(cp);
	}

	//��������
	~CountingPtr()
	{
		this->detach();//���ټ���ֵ���������ֵΪ0�����ͷŸü�����ָ��
	}

	//������ֵ�����(Ҫ���Ǹ��Լ���ֵ)
	CountingPtr<T, CP, OP>& operator=(CountingPtr<T, CP, OP> const& cp)
	{
		if (this->object_pointed_to != cp.object_pointed_to)
		{
			this->detach();//�Ա�����ļ���ֵ���м�1 ,�������ֵΪ0�����ͷż�����
			CP::operator=((CP const&)cp);//��policy���и�ֵ
			OP::operator=((OP const&)cp);//��policy���и�ֵ
			this->attach(cp);//����ָ�벢���Ӽ���ֵ
		}
		return *this;
	}

	//ʹ֮��Ϊ����ָ��������
	T* operator->() const
	{
		return this->object_pointed_to;
	}

	T& operator*() const
	{
		return *this->object_pointed_to;
	}
	

	//ת�͹��캯��
	friend 
		template<typename S>  //s������void����T�Ļ���
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
	//��������:����ָͨ����г�ʼ����ǰ������ָͨ����ڣ�
	void init(T* p)
	{
		if (p)
		{
			CounterPolicy::init(p);//����ת��Ϊ���м������ܵ�ָ��
		}
		this->object_pointed_to = p;
	}

	//����ָ�룬�������Ӽ���ֵ��ǰ����ָ����ڣ�
	void attach(CountingPtr<T,CP,OP> const& cp)
	{
		this->object_pointed_to = cp.object_pointed_to;
		if (cp.object_pointed_to)
			CounterPolicy::increment(cp.object_pointed_to);
	}

	//���ټ���ֵ���������ֵΪ0�����ͷŸü�����ָ��
	void detach()
	{
		if (this->object_pointed_to != nullptr)
		{
			CounterPolicy::decrement(this->object_pointed_to);
			if (CounterPolicy::is_zero(this->object_pointed_to))
			{
				//����б�Ҫ�Ļ����ͷż�����
				CounterPolicy::dispose(this->object_pointed_to);
				//ʹ��object policy���ͷ���ָ��Ķ���
				ObjectPolicy::dispose(this->object_pointed_to);

			}



		}

	}





};







#endif