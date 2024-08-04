#include "CBLinkSource.h"
#include <io.h>

using namespace std;

CBLinkSource::CBLinkSource(LPVOID lpMem, size_t size) 
	: CBFormateBase(lpMem, size) {

}

bool CBLinkSource::parse(Document& jsonDoc) {
	if (m_size < 1) return false;

	string file = parseLinkSource(m_lpMem, m_size);
	if (file == "") return false;

	Value jsonStr(kStringType);
	jsonStr.SetString(file.c_str(), jsonDoc.GetAllocator());
	jsonDoc.AddMember("LinkSource", jsonStr.Move(), jsonDoc.GetAllocator());

	return true;
}


void CBLinkSource::parseLinkSource(string& html, Document& jsonDoc, Value& jsonStr) {

}

BOOL CBLinkSource::IsVisibleChar(BYTE c) {
	return !(c >= 0 && c <= 0x1F) && c != 0x7F;
}

string CBLinkSource::parseLinkSource(LPVOID lpMem, size_t size) {
	char* buf = (char*)malloc(size + 1);
	if (buf == NULL) return "";

	BYTE* pbMem = (BYTE*)lpMem;
	BYTE* pSearch = pbMem;
	char* pSave = buf;
	memset(buf, 0, size + 1);
	while (pSearch < pbMem + size) {
		if (IsVisibleChar(*pSearch)) {
			*pSave = *pSearch;
			++pSave;
		}
		++pSearch;
	}
	string file = FindFile(buf);

	free(buf);

	return move(file);
}

string CBLinkSource::FindFile(char* buf) {
	char* pPathStart = buf;
	char* pFinalPathStart = NULL;
	int maxPathLen = 0;

	while ((pPathStart = FindFileStart(pPathStart)) != NULL) {
		int len = FindAFile(pPathStart);
		if (len > maxPathLen) {
			maxPathLen = len;
			pFinalPathStart = pPathStart;
		}
		++pPathStart;
	}

	if (pFinalPathStart != NULL) {
		*(pFinalPathStart + maxPathLen) = 0;
		return move(convertMultiByte(pFinalPathStart, maxPathLen));
	}

	return "";
}

char* CBLinkSource::FindFileStart(char* buf) {
	char path[3];
	char* p = buf;
	path[2] = 0;
	while (p < buf + strlen(buf)) {
		path[0] = *p;
		path[1] = *(p + 1);
		if (_access(path, 0) == 0) {
			return p;
		}

		++p;
	}
	return NULL;
}

ULONG CBLinkSource::FindAFile(char* szPathStart) {
	char* p = szPathStart, * pEnd = p + 1;
	char* pEndSave = NULL;
	char c;

	while (pEnd <= szPathStart + strlen(szPathStart)) {
		c = *pEnd;
		*pEnd = 0;
		if (_access(szPathStart, 0) == 0) {
			pEndSave = pEnd;
		}

		*pEnd = c;
		++pEnd;
	}

	return (ULONG)(pEndSave == NULL ? 0 : (pEndSave - szPathStart));
}
