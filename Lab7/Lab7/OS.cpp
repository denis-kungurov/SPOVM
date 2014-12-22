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

void OS::Move()
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
			offsetToMove = mft.FilesOffsets[i];
			mft.FilesOffsets[i] = 0;
			return;
		}
	}
	else
	{
		Entity folder;
		folder = FindFolder(currentFolder);
		for (int i = 0; i < _FilesParts; i++)
			if (folder.Blocks[i] != 0)
			{
				temp = ReadEntity(folder.Blocks[i]);
				if (strcmp(temp.Name, fileName.c_str()) != 0) continue;
				offsetToMove = folder.Blocks[i];
				folder.Blocks[i] = 0;
				SaveEntity(FindFolderOffset(currentFolder), folder);
				return;
			}
	}
	cout << " Entity doesn't exist" << endl;
}

void OS::Paste()
{
	if (offsetToMove == 0) return;
	if (currentFolder == "")
	{
		for (int i = 0; i < _MaxFileCount; i++)
			if (mft.FilesOffsets[i] == 0){
				mft.FilesOffsets[i] = offsetToMove;
				break;
			}
	}
	else
	{
		Entity folder;
		folder = FindFolder(currentFolder);
		for (int i = 0; i < _FilesParts; i++)
			if (folder.Blocks[i] == 0)
			{
				folder.Blocks[i] = offsetToMove;
				SaveEntity(FindFolderOffset(currentFolder), folder);
				break;
			}
	}
	offsetToMove = 0;
}

void OS::SaveMFT()
{
	osFile.seekp(0);
	osFile.write((char*)&mft, sizeof(MFT));
}

void OS::SaveEntity(size_t offset, Entity block)
{
	SaveMFT();
	osFile.seekp(offset);
	osFile.write((char*)&block, sizeof(Entity));
	osFile.flush();
}

void OS::SaveBlock(size_t offset, Block block)
{
	osFile.seekp(offset);
	osFile.write((char*)&block, sizeof(Block));
	osFile.flush();
}

Entity OS::ReadEntity(size_t offset)
{
	Entity file;
	osFile.seekg(offset);
	osFile.read((char*)&file, sizeof(Entity));
	return file;
}

Block OS::ReadBlock(size_t offset)
{
	Block block;
	osFile.seekg(offset);
	osFile.read((char*)&block, sizeof(Block));
	return block;
}

int OS::KeyProcessing()
{
	int word;
	COORD pos;
	PCONSOLE_SCREEN_BUFFER_INFO  currentPos = (PCONSOLE_SCREEN_BUFFER_INFO)malloc(sizeof(PCONSOLE_SCREEN_BUFFER_INFO));
	word = _getch();
	if (word == 8)	//backspace
	{
		GetConsoleScreenBufferInfo(Console, currentPos);
		pos.X = currentPos->dwCursorPosition.X == 0 ? currentPos->dwCursorPosition.X : currentPos->dwCursorPosition.X - 1;
		pos.Y = currentPos->dwCursorPosition.Y;
		SetConsoleCursorPosition(Console, pos);
		cout << " ";
		SetConsoleCursorPosition(Console, pos);
	}
	else if (word == 224)	//control arrows
	{
		word = _getch();
		GetConsoleScreenBufferInfo(Console, currentPos);
		pos.X = currentPos->dwCursorPosition.X;
		pos.Y = currentPos->dwCursorPosition.Y;

		switch (word)
		{
		case 75:
			pos.X == 0 ? pos.X : pos.X--;
			SetConsoleCursorPosition(Console, pos);
			break;
		case 72:
			pos.Y == 0 ? pos.Y : pos.Y--;
			SetConsoleCursorPosition(Console, pos);
			break;
		case 77:
			pos.X == 79 ? pos.X : pos.X++;
			SetConsoleCursorPosition(Console, pos);
			break;
		case 80:
			pos.Y++;
			SetConsoleCursorPosition(Console, pos);
		}

	}
	else if (word == 13)	//enter
	{
		cout << endl;
	}
	else if (word == 27)	//esc
	{
		return 0;
	}
	else cout << (char)word;
}

size_t OS::GetFreeBlockOffset()
{
	for (int i = 0; i < _AllBlocksCount; i++)
		if (mft.FreeBlocksOffsets[i] != 0)
		{
			size_t temp = mft.FreeBlocksOffsets[i];
			mft.FreeBlocksOffsets[i] = 0;
			mft.FreeSpace -= _BlockSize;
			return temp;
		}
	return 0;
}

void OS::EditFile(size_t offsetToFile)
{
	Entity file = ReadEntity(offsetToFile);
	Block temp;
	char tempText[_BlockSize];
	string text = "";
	for (int i = 0; i < _FilesParts; i++)
		if (file.Blocks[i] != 0)
		{
			temp = ReadBlock(file.Blocks[i]);
			strcpy_s(tempText, temp.Data);
			text += string(tempText);
		}
	system("cls");
	cout << text;
	SetConsoleCursorPosition(Console, COORD{ 0, 0 });
	while (true)
	{
		if (_kbhit())
		{
			if (!KeyProcessing()) break;
		}
	}
	SaveFile(file, offsetToFile);
	system("cls");
}

/////////////////////////////////////////////////////////////
void OS::SaveFile(Entity file, size_t offsetToFile)
{
	COORD size, pos;
	size_t newOffset;
	PSMALL_RECT rect = (PSMALL_RECT)malloc(sizeof(PSMALL_RECT));
	rect->Left = 0; rect->Top = 0; rect->Bottom = 80; rect->Right = 79;
	pos.X = 0; pos.Y = 0; size.X = 1; size.Y = 1;
	PCHAR_INFO buff = (PCHAR_INFO)calloc(1, sizeof(PCHAR_INFO));
	Block newBlock;
	int blockNumber = 0, blockIndex = 0;
	while (true)
	{
		if (ReadConsoleOutputA(Console, buff, size, pos, rect) == 0)
			break;
		newBlock.Data[blockIndex++] = buff[0].Char.AsciiChar;
		if (blockIndex == _BlockSize - 1)
		{
			newBlock.Data[blockIndex++] = '\0';
			if (blockNumber == 24) break;
			blockIndex = 0;
			newOffset = file.Blocks[blockNumber] == 0 ? GetFreeBlockOffset() : file.Blocks[blockNumber];
			file.Blocks[blockNumber++] = newOffset;
			SaveBlock(newOffset, newBlock);
			newBlock = Block();
		}
		rect->Bottom = 80; rect->Right = 79;
		if (++rect->Left == 80)
		{
			blockIndex = Trim(newBlock.Data);
			rect->Left = 0;
			++rect->Top;
		}
	}
	if (Trim(newBlock.Data) != 0)
	{
		newOffset = file.Blocks[blockNumber] == 0 ? GetFreeBlockOffset() : file.Blocks[blockNumber];
		file.Blocks[blockNumber++] = newOffset;
		SaveBlock(newOffset, newBlock);
	}
	for (int i = blockNumber; i < _FilesParts; i++)
		if (file.Blocks[i] != 0)
		{
			mft.FreeBlocksOffsets[(file.Blocks[i] - _MFTSize) / _BlockSize] = file.Blocks[i];
			mft.FreeSpace += _BlockSize;
			file.Blocks[i] = 0;
		}
	SaveEntity(offsetToFile, file);
}

int OS::Trim(char *str)
{
	int i = _BlockSize - 1;
	while ((str[i] == ' ' || str[i] == '\0' || str[i] == '\n') && i >= 0) i--;
	if (i == -1) return 0;
	str[i + 1] = '\n';
	return i + 2;
}

void OS::OpenFile()
{
	string fileName, folderName;
	cin >> fileName;
	Entity temp;
	if (currentFolder == "")
	{
		_FindNotNULLOffsets
		{
			temp = ReadEntity(mft.FilesOffsets[i]);
			if (strcmp(temp.Name, fileName.c_str()) != 0) continue;
			EditFile(mft.FilesOffsets[i]);
			return;
		}
	}
	else
	{
		Entity folder = FindFolder(currentFolder);
		for (int i = 0; i < _FilesParts; i++)
			if (folder.Blocks[i] != 0)
			{
				temp = ReadEntity(folder.Blocks[i]);
				if (strcmp(temp.Name, fileName.c_str()) != 0) continue;
				EditFile(folder.Blocks[i]);
				return;
			}
	}
	cout << " File doesn't exist" << endl;
}

Entity OS::FindFolder(string folderName)
{
	size_t offset = FindFolderOffset(folderName);
	if (offset != 0)
		return ReadEntity(offset);
	return Entity();
}

size_t OS::FindFolderOffset(string folderName)
{
	Entity folder;
	vector<string> folders = SplitFolderName("/" + folderName);
	_FindNotNULLOffsets
	{
		folder = ReadEntity(mft.FilesOffsets[i]);
		if (strcmp(folder.Name, folders[0].c_str()) == 0 &&
			strcmp(folder.Type, ((string)_FOLDER).c_str()) == 0)
			if (folders.size() == 1 || folders[1] == "")
				return mft.FilesOffsets[i];
			else return FindSubFolderOffset(folders, folder, 1);
	}
	return 0;
}

size_t OS::FindSubFolderOffset(vector<string> folders, Entity baseFolder, int index)
{
	Entity folder;
	for (int i = 0; i < _FilesParts; i++)
	{
		folder = ReadEntity(baseFolder.Blocks[i]);
		if (strcmp(folder.Name, folders[index].c_str()) == 0 &&
			strcmp(folder.Type, ((string)_FOLDER).c_str()) == 0)
			if (folders.size() - 1 == index)
				return baseFolder.Blocks[i];
			else
				return FindSubFolderOffset(folders, folder, index + 1);
	}
	return 0;
}

void OS::ChangeFolder()
{
	string newFolder;
	cin >> newFolder;
	if (newFolder == "/")
	{
		currentFolder = "";
		return;
	}
	Entity folder = FindFolder(currentFolder + "/" + newFolder);
	if (strcmp(folder.Name, newFolder.c_str()) == 0)
		currentFolder += "/" + newFolder;
	else cout << " Folder doesn't exist" << endl;
}

vector<string> OS::SplitFolderName(string folderName)
{
	int len = folderName.length();
	int i = 0; int k = 0;
	vector<string> words(1);
	do
	{
		char a = folderName[i];
		if (folderName[i] != '/' && folderName[i] != '\0')
			words[k].push_back(folderName[i]);
		else if (words[k] != "")
		{
			string temp;
			words.push_back(temp);
			k++;
		}
	} while (++i < len);
	return words;
}

string OS::GetCurrentFolder()
{
	return currentFolder;
}

void OS::Format()
{
	osFile.close();
	fstream file(_OSFIle, ios::binary | ios::out | ios::trunc);
	file.write((char*)(new MFT()), sizeof(MFT));
	for (int i = 0; i < _AllBlocksCount; i++)
		file.write((char*)(new Block()), sizeof(Block));
	file.close();
	currentFolder = "";
	offsetToMove = 0;
	Console = GetStdHandle(STD_OUTPUT_HANDLE);
	osFile.open(_OSFIle, ios::binary | ios::in | ios::out);
	osFile.read((char*)&mft, sizeof(MFT));
}

