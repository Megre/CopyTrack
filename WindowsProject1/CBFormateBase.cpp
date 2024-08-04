#include "CBFormateBase.h"
#include <codecvt>
#include <locale>
#include <vector>

CBFormateBase::CBFormateBase(LPVOID lpMem, size_t size) {
	m_lpMem = lpMem;
	m_size = size;
}

bool CBFormateBase::isSingleByteEncoding(LPVOID lpMem, size_t size) {
	const char* pMem = (const char*)lpMem;
	for (size_t i = 0; i < size; ++i) {
		if (static_cast<unsigned char>(pMem[i]) >= 0x80) {
			return false; 
		}
	}
	return true; 
}

string CBFormateBase::convertWstr(LPVOID lpMem, size_t size) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	LPWSTR lpwszStr = static_cast<LPWSTR>(lpMem);
	LPWSTR lpwszStrEnd = static_cast<LPWSTR>((LPVOID)((const char*)lpMem + size));
	try {
		return move(converter.to_bytes(lpwszStr, lpwszStrEnd));
	}
	catch (...) {
		return "";
	}
}

string CBFormateBase::convertMultiByte(LPVOID lpMem, size_t size) {
	UINT codepage = GetACP();
	return convertMultiByte(codepage, lpMem, size);
}

string CBFormateBase::convertMultiByte(UINT codepage, LPVOID lpMem, size_t size)
{	
	const char* str = (const char*)lpMem;
	try {		
		string localeName = "." + to_string(codepage); //locale name(.codepage) on Windows
		wstring_convert<codecvt<wchar_t, char, mbstate_t>>
			cv1(new codecvt<wchar_t, char, mbstate_t>(localeName.c_str()));
		wstring wstr = cv1.from_bytes(str, str + size);

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter1;
		return move(converter1.to_bytes(wstr));
	}
	catch (...) {
		return "";
	}

	//int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	//std::vector<char> utf8(len);
	//WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8[0], len, NULL, NULL);
	//return move(string(utf8.begin(), utf8.end()));
}

string& CBFormateBase::trimRight(string& str) {
	return str.erase(str.find_last_not_of('\0') + 1);
}