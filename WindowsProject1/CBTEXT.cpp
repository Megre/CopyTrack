#include "CBTEXT.h"
#include <string>

using namespace std;

CBTEXT::CBTEXT(LPVOID lpMem, size_t size): CBFormateBase(lpMem, size) {

}

bool CBTEXT::parse(Document& jsonDoc) {
	if (m_size < 1) return false;

	string str = "";
	if (isSingleByteEncoding(m_lpMem, m_size)) {
		str = string((const char*)m_lpMem, m_size);
	}
	else {
		str = convertMultiByte(m_lpMem, m_size);
	}

	trimRight(str);

	Value text;
	text.SetString(str.c_str(), (SizeType)str.size(), jsonDoc.GetAllocator());
	jsonDoc.AddMember("Text", text.Move(), jsonDoc.GetAllocator());

	return true;
}