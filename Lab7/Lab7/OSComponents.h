#pragma once
#include <iostream>
#include <stdio.h>
#include "Constants.h"
using namespace std;

struct MFT
{
	size_t FreeSpace = _OSSize - _MFTSize;
	size_t FreeBlocksOffsets[_AllBlocksCount];
	size_t FilesOffsets[_MaxFileCount];
	MFT()
	{
		for (int i = 0; i < _AllBlocksCount; i++)
		{
			FreeBlocksOffsets[i] = _BlockSize * i + _MFTSize;
			if (i< _MaxFileCount) FilesOffsets[i] = 0;
		}
	}
};

struct Block
{
	char Data[_BlockSize];
	Block()
	{
		for (int i = 0; i < _BlockSize; i++) Data[i] = 0;
	}
};

struct Entity
{
	char Name[_MaxName];
	char Type[10];
	size_t Blocks[_FilesParts];
	Entity()
	{
		for (int i = 0; i < _MaxName; i++)
		{
			if (i < 10) Type[i] = 0;
			Name[i] = 0;
			if (i < _FilesParts) Blocks[i] = 0;
		}
	}
};