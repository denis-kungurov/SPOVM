CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -D _XOPEN_SOURCE -lpthread
WCC=cl
WINC=/I"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include"
WINC2=/I"C:\Program Files (x86)\Windows Kits\8.1\Include\um"
WINC3=/I"C:\Program Files (x86)\Windows Kits\8.1\Include\shared"
WLINK=/link uuid.lib libcmt.lib /LIBPATH:"C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86" /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib"

linux:
	$(CC) $(CFLAGS) main.c linfun.c -o main

win:
	$(WCC) $(WINC) $(WINC2) $(WINC3) /TC main.c winfun.c $(WLINK)
