#pragma once
#include "CBFormateBase.h"

class CBHTMLFormat: public CBFormateBase
{
public:
	CBHTMLFormat(LPVOID lpMem, size_t size);

	bool parse(Document& jsonDoc);

private:
	void parseHtml(string& html, Document& jsonDoc, Value& jsonObj);
};

