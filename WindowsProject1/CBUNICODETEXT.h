#pragma once
#include "CBFormateBase.h"

class CBUNICODETEXT: public CBFormateBase
{
public:
	CBUNICODETEXT(LPVOID lpMem, size_t size);

	bool parse(Document& jsonDoc);
};

