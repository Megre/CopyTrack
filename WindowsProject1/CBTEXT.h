#pragma once

#include <Windows.h>
#include "CBFormateBase.h"


class CBTEXT : public CBFormateBase
{
public:
	CBTEXT(LPVOID lpMem, size_t size);

	bool parse(Document& jsonDoc);
};

