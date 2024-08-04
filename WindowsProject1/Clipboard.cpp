#include <Windows.h>
#include <iostream>
#include <WinUser.h>
#include <io.h>
#include "Clipboard.h"
#include "CBFormateBase.h"
#include "CBTEXT.h"
#include "CBUNICODETEXT.h"
#include "CBHTMLFormat.h"
#include "CBLinkSource.h"

using namespace std;

#define FORMAT_NAME_LEN     512

Clipboard::Clipboard() {

}



void Clipboard::PrintMem(LPVOID lpMem, size_t size) {

    BYTE* p = (BYTE*)lpMem;
    cout << "<<" << endl;

    while (size-- > 0) {
		char c = *p;
        if (!(c >= 0 && c <= 0x1F) && c != 0x7F) {
            cout << (*p) << ",";
        }        
        ++p;
    }
    cout << endl << ">>" << endl;
}


void Clipboard::EnumFormats() {
    UINT format = 0;
    char szFormatName[FORMAT_NAME_LEN + 1];
    while (format = EnumClipboardFormats(format)) {
        memset(szFormatName, 0, sizeof(szFormatName));
        int length = GetClipboardFormatNameA(format, szFormatName, sizeof(szFormatName));
        cout << szFormatName << ": " << format << endl;
    }
}

BOOL Clipboard::Track() {
	if (!OpenClipboard(NULL)) return FALSE;

	Document jsonDoc;
	jsonDoc.SetObject();	

	char szFormatName[FORMAT_NAME_LEN + 1];
	UINT format = 0;
	while (format = EnumClipboardFormats(format)) {
        try
        {
			HANDLE handle = GetClipboardData(format);
            if (handle == NULL) continue;

			if (!IsClipboardFormatAvailable(format)) continue;

			LPVOID lpMem = GlobalLock(handle);
			if (lpMem == NULL) continue;		            

			int length = GetClipboardFormatNameA(format, szFormatName, sizeof(szFormatName));
			if (length > 0) {
				if (0 == strcmp(szFormatName, "HTML Format")) {
					CBHTMLFormat(lpMem, GlobalSize(handle)).parse(jsonDoc);
				}
				else if (0 == strcmp(szFormatName, "Link Source")) {
					CBLinkSource(lpMem, GlobalSize(handle)).parse(jsonDoc);
				}
			}
			else {
				if (CF_TEXT == format) {
					CBTEXT(lpMem, GlobalSize(handle)).parse(jsonDoc);
				}
				else if (CF_UNICODETEXT == format) {
					CBUNICODETEXT(lpMem, GlobalSize(handle)).parse(jsonDoc);
				}
			}

			GlobalUnlock(handle);
        }
        catch (...) {
            continue;
        }	                
	} // while

	CloseClipboard();

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	jsonDoc.Accept(writer);
    cout << buffer.GetString() << endl;

	return TRUE;
}