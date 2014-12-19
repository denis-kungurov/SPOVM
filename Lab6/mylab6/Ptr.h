#ifndef PTR_H
#define PTR_H

#include <iostream>
#include "memory.h"


template<typename T>
class Ptr {
	int id;
	T *obj;

public:
	Ptr();
	Ptr(T value);
	Ptr(const Ptr& ptr);
	~Ptr();
	T &operator*();
	T *operator->();
	Ptr &operator=(Ptr<T> &val);
	Ptr &operator=(T &value);

private:
	void create_object();

};

template<typename T>
Ptr<T>::Ptr()
{
	create_object();
}

template<typename T>
Ptr<T>::Ptr(T value)
{
	create_object();
	*obj = value;
}

template<typename T>
Ptr<T>::Ptr(const Ptr& ptr)
{
	obj = ptr.obj;
	id = ptr.id;
	Memory::add_link(id);
}

template<typename T>
Ptr<T>::~Ptr()
{
	Memory::delete_link(id);
}

template<typename T>
void Ptr<T>::create_object()
{
	MemoryInfo info = Memory::get_new(sizeof(T));
	obj = reinterpret_cast<T *>(info.pointer);
	id = info.id;
}

template<typename T>
T &Ptr<T>::operator*()
{
	return *obj;
}

template<typename T>
T *Ptr<T>::operator->()
{
	return obj;
}

template<typename T>
Ptr<T> &Ptr<T>::operator=(Ptr<T> &val)
{
	Memory::delete_link(id);
	obj = val.obj;
	id = val.id;
	Memory::add_link(id);
	return *this;
}

template<typename T>
Ptr<T> &Ptr<T>::operator=(T &value)
{
	Memory::delete_link(id);
	create_object();
	*obj = value;
	return *this;
}
#endif // PTR_H
