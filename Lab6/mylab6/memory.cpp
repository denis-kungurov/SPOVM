#include "memory.h"

Memory Memory::memory_manager;

Memory::Memory()
{

}

Memory::~Memory()
{

}

MemoryInfo Memory::get_new(int size)
{
	MemoryInfo result;
	result.pointer = ::operator new(size);
	result.num_of_links = 1;
	result.id = memory_manager.init_new_link(result);
	printf(" New memory allocated \n");
	return result;
}

int Memory::init_new_link(MemoryInfo info)
{
	int id = num_of_links.size();
	num_of_links[id] = info;
	return id;
}

void Memory::delete_object(int id)
{
	:: operator delete(num_of_links[id].pointer);
	num_of_links.erase(id);
}

void Memory::add_link(int id)
{
	memory_manager.num_of_links[id].num_of_links += 1;
}

void Memory::delete_link(int id)
{
	memory_manager.num_of_links[id].num_of_links -= 1;
	if (memory_manager.num_of_links[id].num_of_links == 0) {
		memory_manager.delete_object(id);
		printf(" Memory free \n");
	}
}