#include "CBUNICODETEXT.h"
#include <string>
#include <codecvt>
#include <locale>

using namespace std;

CBUNICODETEXT::CBUNICODETEXT(LPVOID lpMem, size_t size): CBFormateBase(lpMem, size) {

}

bool CBUNICODETEXT::parse(Document& jsonDoc) {
	if (m_size < 1) return false;

	string str = convertWstr(m_lpMem, m_size);
	trimRight(str);

	Value value;
	value.SetString(str.c_str(), (SizeType)str.size(), jsonDoc.GetAllocator());
	jsonDoc.AddMember("UnicodeText", value.Move(), jsonDoc.GetAllocator());

	return true;
}