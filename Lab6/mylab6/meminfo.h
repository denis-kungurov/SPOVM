#ifndef MEMINFO_H
#define MEMINFO_H


class MemoryInfo {
public:
	void *pointer;
	int id;
	int num_of_links;
	MemoryInfo();
	~MemoryInfo();
};
#endif // MEMINFO_H