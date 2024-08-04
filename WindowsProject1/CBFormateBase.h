#pragma once
#include <Windows.h>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"

using namespace rapidjson;
using namespace std;

class CBFormateBase
{
protected:
	LPVOID m_lpMem;
	size_t m_size;

public:
	CBFormateBase(LPVOID lpMem, size_t size);

	bool isSingleByteEncoding(LPVOID lpMem, size_t size);
	string convertWstr(LPVOID lpMem, size_t size);
	string convertMultiByte(LPVOID lpMem, size_t size);
	string convertMultiByte(UINT codepage, LPVOID lpMem, size_t size);
	string& trimRight(string& str);

public:
	virtual bool parse(Document& jsonDoc) = 0;
};

