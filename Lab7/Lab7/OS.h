#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Constants.h"
#include "OSComponents.h"
#include <Windows.h>
#include <conio.h>

class OS
{
	MFT mft;
	string currentFolder;
	fstream osFile;
	size_t offsetToMove;
	HANDLE Console;
	vector<string> SplitFolderName(string Name);

	void RemoveFolder(Entity folder);
	void RemoveEntity(size_t offset);
	void SaveMFT();
	void SaveEntity(size_t offset, Entity block);
	void EditFile(size_t offsetToFile);
	Entity FindFolder(string folderName);
	size_t FindFolderOffset(string folderName);
	size_t FindSubFolderOffset(vector<string> path, Entity baseFolder, int index);
	int KeyProcessing();
	Entity ReadEntity(size_t offset);
	Block ReadBlock(size_t offset);
	void SaveBlock(size_t offset, Block block);
	size_t GetFreeBlockOffset();
	int Trim(char *str);
	void SaveFile(Entity file, size_t offsetToFile);
public:
	OS();
	~OS();
	void ShowFiles();
	void CreateEntity(string type);
	size_t AddEntityToFolder(Entity entity);
	size_t AddEntityToMFT(Entity entity);
	void ShowFolder(Entity folder, int lvl = 0);
	void Remove();
	void Move();
	void Paste();
	void OpenFile();
	void ChangeFolder();
	string GetCurrentFolder();
	void Format();
};