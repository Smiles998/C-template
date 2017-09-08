#ifndef STDOBJPOLICY_H
#define STDOBJPOLICY_H

//用CountingPtr计数的对象---》对象policy
class StandardObjectPolicy
{
public:
	template<typename T>
	void dispose(T* object);


};









#endif