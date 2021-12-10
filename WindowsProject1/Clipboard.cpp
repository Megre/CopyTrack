#include <Windows.h>
#include <iostream>
#include <WinUser.h>
#include <io.h>
#include "Clipboard.h"

using namespace std;

#define FORMAT_NAME_LEN     512

Clipboard::Clipboard() {

}

BOOL Clipboard::IsVisibleChar(BYTE c) {
    return !(c >= 0 && c <= 0x1F) && c != 0x7F;
}

void Clipboard::PrintMem(LPVOID lpMem, size_t size) {

    BYTE* p = (BYTE*)lpMem;
    cout << "<<" << endl;

    while (size-- > 0) {
        if (IsVisibleChar(*p)) {
            cout << (*p) << ",";
        }        
        ++p;
    }
    cout << endl << ">>" << endl;
}

LPVOID Clipboard::DuplicateMem(HANDLE inputHandle, HGLOBAL *pOutputHandle, size_t *pOutSize) {
     *pOutSize = GlobalSize(inputHandle);
    if (*pOutSize == 0) return NULL;

    LPVOID mem = GlobalLock(inputHandle);
    if (mem == NULL) return NULL;

    *pOutputHandle = GlobalAlloc(GHND, *pOutSize);
    if (*pOutputHandle == NULL) {
        GlobalUnlock(inputHandle);
        return NULL;
    };

    LPVOID memCopy = GlobalLock(*pOutputHandle);
    if (memCopy == NULL) {
        GlobalFree(*pOutputHandle);
        GlobalUnlock(inputHandle);
        return NULL;
    }

    memset(memCopy, 0, *pOutSize);
    CopyMemory(memCopy, mem, *pOutSize);
 
    GlobalUnlock(inputHandle);
    GlobalUnlock(*pOutputHandle);

    return mem;
}

void Clipboard::PrintSourceURL(LPVOID lpMem, size_t size) {
    const char* szTag = "SourceURL:";
    char* p = (char*)lpMem;

    p = strstr(p, szTag);
    if (p == NULL) return;

    p += strlen(szTag);
    while (p < (((char*)lpMem) + size) && *p != 0x0D) {
        cout << *p;
        ++p;
    }
    cout << endl;
}


void Clipboard::PrintLinkSource(LPVOID lpMem, size_t size) {
    char* buf = (char*) malloc(size + 1);
    if (buf == NULL) return;

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
    FindFile(buf);

    free(buf);
}

void Clipboard::FindFile(char* buf) {
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
        cout << pFinalPathStart << endl;
    }
}

char* Clipboard::FindFileStart(char* buf) {
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

ULONG Clipboard::FindAFile(char* szPathStart) {
    char* p = szPathStart, *pEnd = p + 1;
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

    return (ULONG) (pEndSave == NULL ? 0 : (pEndSave - szPathStart));
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

    char szFormatName[FORMAT_NAME_LEN + 1];
    UINT format = 0;    
    while (format = EnumClipboardFormats(format)) {
        szFormatName[0] = 0;
        int length = GetClipboardFormatNameA(format, szFormatName, sizeof(szFormatName));

        HANDLE handle = GetClipboardData(format);
        if (handle == NULL) continue;

        HANDLE dulHandle;
        size_t size;
        LPVOID lpMem = DuplicateMem(handle, &dulHandle, &size);
        if (lpMem == NULL) continue;

        if (0 == strcmp(szFormatName, "Link Source")) {
            PrintLinkSource(lpMem, size);
        }
        else if (0 == strcmp(szFormatName, "HTML Format")) {
            PrintSourceURL(lpMem, size);            
        }
                
        GlobalFree(dulHandle);
    }

    CloseClipboard();

    return TRUE;
}