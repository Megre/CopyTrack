#pragma once


class Clipboard
{
public:
	Clipboard();

	BOOL Track();

private:

	void PrintMem(LPVOID lpMem, size_t size);
	void EnumFormats();
};

