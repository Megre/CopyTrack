#include "CBHTMLFormat.h"

CBHTMLFormat::CBHTMLFormat(LPVOID lpMem, size_t size)
	:CBFormateBase(lpMem, size) {

}

bool CBHTMLFormat::parse(Document& jsonDoc) {
	if (m_size < 1) return false;

	string str = "";
	if (isSingleByteEncoding(m_lpMem, m_size)) {
		str = string((const char*)m_lpMem, m_size);
	}
	else {
		str = convertMultiByte(65001, m_lpMem, m_size);
	}

	Value jsonObj(kObjectType);
	parseHtml(str, jsonDoc, jsonObj);
	jsonDoc.AddMember("HTMLFormat", jsonObj.Move(), jsonDoc.GetAllocator());
}

void CBHTMLFormat::parseHtml(string &html, Document &jsonDoc, Value& jsonObj) {
	const char* lineTag = "\r\n", *splitTag = ":";
	size_t lastpos = 0, pos = html.find(lineTag);
	while (pos != string::npos) {
		string frag = html.substr(lastpos, pos - lastpos);
		size_t posSplit = frag.find(splitTag);
		if (posSplit != string::npos) {
			string k = frag.substr(0, posSplit), 
				v = frag.substr(posSplit + 1, frag.length());

			Value keyItem(kStringType), valueItem(kStringType);
			keyItem.SetString(k.c_str(), (SizeType)k.size(), jsonDoc.GetAllocator());
			valueItem.SetString(v.c_str(), (SizeType)v.size(), jsonDoc.GetAllocator());
			jsonObj.AddMember(keyItem.Move(), valueItem.Move(), jsonDoc.GetAllocator());
		}
		else {
			const char* fstart = "<!--StartFragment-->",
				* fend = "<!--EndFragment-->";
			size_t posStart = html.find(fstart, lastpos) + strlen(fstart),
				posEnd = html.find(fend, lastpos);
			string v = html.substr(posStart, posEnd - posStart);
			Value valueItem(kStringType);
			valueItem.SetString(v.c_str(), (SizeType)v.size(), jsonDoc.GetAllocator());
			jsonObj.AddMember("HTML", valueItem.Move(), jsonDoc.GetAllocator());
			break;
		}

		lastpos = pos + strlen(lineTag);
		pos = html.find(lineTag, lastpos);
	}
}