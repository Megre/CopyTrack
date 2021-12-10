#pragma once


class Clipboard
{
public:
	Clipboard();

	BOOL Track();

private:
	BOOL IsVisibleChar(BYTE c);
	void PrintMem(LPVOID lpMem, size_t size);
	LPVOID DuplicateMem(HANDLE inputHandle, HGLOBAL* pOutputHandle, size_t* pOutSize);
	void PrintSourceURL(LPVOID lpMem, size_t size);
	void FindFile(char* buf);
	char* FindFileStart(char* buf);
	ULONG FindAFile(char* szPathStart);
	void PrintLinkSource(LPVOID lpMem, size_t size);
	void EnumFormats();

};

