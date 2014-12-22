#include <string>
#include "Constants.h"
#include "OS.h"

using namespace std;

int main()
{
	/*cout << "MFT "<<sizeof(MFT) << endl;
	cout << "BLOCK "<<sizeof(Block) << endl;
	cout << "BLOCKs " << _AllBlocksCount << endl;
	cout << "Entity " << sizeof(Entity) << endl;*/
	string command;
	OS os;
	while (true)
	{
		cout << os.GetCurrentFolder() << "> ";
		cin >> command;
		if (command == "exit") break;
		else if (command == "dir") os.ShowFiles();
		else if (command == "mkfolder") os.CreateEntity(_FOLDER);
		else if (command == "mkfile") os.CreateEntity(_FILE);
		else if (command == "rm") os.Remove();
		else if (command == "format") os.Format();
		else if (command == "cd") os.ChangeFolder();
		else if (command == "move") os.Move();
		else if (command == "paste") os.Paste();
		else if (command == "open") os.OpenFile();
		else if (command == "cls") system("cls");
		else cout << " Unknown command" << endl;
	}
	return 0;
}