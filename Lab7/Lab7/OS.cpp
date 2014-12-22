#include "OS.h"

OS::OS()
{
	currentFolder = "";
	offsetToMove = 0;
	Console = GetStdHandle(STD_OUTPUT_HANDLE);
	osFile.open(_OSFIle, ios::binary | ios::in | ios::out);
	osFile.read((char*)&mft, sizeof(MFT));
}

OS::~OS()
{
	osFile.close();
}

void OS::ShowFiles()
{
	cout << "\t\tFree space: " << mft.FreeSpace << " bytes" << endl;
	Entity temp;
	if (currentFolder == "")
	{
		_FindNotNULLOffsets
		{
			temp = ReadEntity(mft.FilesOffsets[i]);
			cout << " " << temp.Name;
			if (strcmp(temp.Type, ((string)_FOLDER).c_str()) == 0)
				cout << " <" << temp.Type << ">";
			cout << endl;
			if (strcmp(temp.Type, ((string)_FOLDER).c_str()) == 0)
				ShowFolder(temp);
		}
	}
	else
	{
		temp = FindFolder(currentFolder);
		cout << temp.Name << endl;
		ShowFolder(temp);
	}
	cout << endl;
}


void OS::ShowFolder(Entity folder, int lvl)
{
	for (int i = 0; i < _FilesParts; i++)
	{
		if (folder.Blocks[i] != 0)
		{
			Entity temp = ReadEntity(folder.Blocks[i]);
			for (int j = 0; j < lvl; j++)
				cout << "    ";
			cout << "  |---";
			cout << temp.Name;
			if (strcmp(temp.Type, ((string)_FOLDER).c_str()) == 0)
				cout << " <" << temp.Type << ">";
			cout << endl;
			if (strcmp(temp.Type, ((string)_FOLDER).c_str()) == 0)
				ShowFolder(temp, lvl + 1);
		}
	}
}

void OS::CreateEntity(string type)
{
	string fileName;
	cin >> fileName;
	Entity file;
	strcpy_s(file.Type, type.c_str());
	strcpy_s(file.Name, fileName.c_str());
	if (currentFolder == "")
	{
		if (AddEntityToMFT(file) == 0)
			cout << " Not enough free space" << endl;
	}
	else
		if (AddEntityToFolder(file) == 0)
			cout << " Folder is full" << endl;
}

void OS::Remove()
{
	string fileName;
	cin >> fileName;
	Entity temp;
	if (currentFolder == "")
	{
		_FindNotNULLOffsets
		{
			temp = ReadEntity(mft.FilesOffsets[i]);
			if (strcmp(temp.Name, fileName.c_str()) != 0) continue;
			if (strcmp(temp.Type, ((string)_FOLDER).c_str()) == 0)
				RemoveFolder(temp);
			RemoveEntity(mft.FilesOffsets[i]);
			mft.FilesOffsets[i] = 0;
			SaveMFT();
			return;
		}
	}
	else
	{
		Entity some;
		temp = FindFolder(currentFolder);
		for (int i = 0; i < _FilesParts; i++)
			if (temp.Blocks[i] != 0)
			{
				some = ReadEntity(temp.Blocks[i]);
				if (strcmp(some.Name, fileName.c_str()) != 0) continue;
				if (strcmp(some.Type, ((string)_FOLDER).c_str()) == 0)
					RemoveFolder(some);
				RemoveEntity(temp.Blocks[i]);
				temp.Blocks[i] = 0;
				SaveEntity(FindFolderOffset(currentFolder), temp);
				return;
			}
	}
	cout << "Entity doesn't exist" << endl;
}

void OS::RemoveFolder(Entity folder)
{
	for (int i = 0; i < _FilesParts; i++)
	{
		if (folder.Blocks[i] != 0)
		{
			Entity temp = ReadEntity(folder.Blocks[i]);
			if (strcmp(temp.Type, ((string)_FOLDER).c_str()) == 0)
				RemoveFolder(temp);
			RemoveEntity(folder.Blocks[i]);
		}
	}
}

void OS::RemoveEntity(size_t offset)
{
	mft.FreeBlocksOffsets[(offset - _MFTSize) / _BlockSize] = offset;
	mft.FreeSpace += _BlockSize;
	osFile.seekp(offset);
	osFile.write((char*)&Block(), sizeof(Entity));
	osFile.flush();
}


size_t OS::AddEntityToMFT(Entity entity)
{
	for (int i = 0; i < _MaxFileCount; i++)
		if (mft.FilesOffsets[i] == 0)
		{
			_FindFreeBlock
			{
				mft.FilesOffsets[i] = mft.FreeBlocksOffsets[j];
				mft.FreeBlocksOffsets[j] = 0;
				mft.FreeSpace -= _BlockSize;
				SaveEntity(mft.FilesOffsets[i], entity);
				return mft.FilesOffsets[i];
			}
			break;
		}
	return 0;
}


size_t OS::AddEntityToFolder(Entity entity)
{
	Entity folder = FindFolder(currentFolder);
	for (int i = 0; i < _FilesParts; i++)
		if (folder.Blocks[i] == 0)
		{
			_FindFreeBlock
			{
				folder.Blocks[i] = mft.FreeBlocksOffsets[j];
				mft.FreeBlocksOffsets[j] = 0;
				mft.FreeSpace -= _BlockSize;
				SaveEntity(folder.Blocks[i], entity);
				SaveEntity(FindFolderOffset(currentFolder), folder);
				return folder.Blocks[i];
			}
		}
	return 0;
}

