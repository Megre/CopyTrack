#pragma once
#include "CBFormateBase.h"
class CBLinkSource: public CBFormateBase
{
public:
	CBLinkSource(LPVOID lpMem, size_t size);

	bool parse(Document& jsonDoc);

private:
	void parseLinkSource(string& html, Document& jsonDoc, Value& jsonObj);

	BOOL IsVisibleChar(BYTE c);
	string FindFile(char* buf);
	char* FindFileStart(char* buf);
	ULONG FindAFile(char* szPathStart);
	string parseLinkSource(LPVOID lpMem, size_t size);

};

