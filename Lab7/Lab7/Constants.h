#define _OSSize 0x110000
#define _BlockSize 0x100
#define _MFTSize 0x10000
#define _AllBlocksCount ( _OSSize - _MFTSize ) / _BlockSize
#define _OSFile "e://File.bin"
#define _MaxFileCount 1000
#define _MaxName 100
#define _FOLDER "FOLDER"
#define _FILE "FILE"
#define _FilesParts 25
#define _FindNotNULLOffsets for ( int i = 0; i < _MaxFileCount; i++) if ( mft.FilesOffsets[i] != 0)
#define _FindFreeBlocks for ( int j = 0; j < _AllBlocksCount; j++) if ( mft.FreeBlocksOffsets[i] != 0)