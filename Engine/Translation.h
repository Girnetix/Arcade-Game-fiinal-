#pragma once
#include <string>
#include <map>
#include <fstream>
#include <codecvt>
#include "Console.h"

void InitTranslationTable(const std::string& filename);
void ClearTranslationTable();
const wchar_t* Translate(const std::wstring& wstr);

extern std::map<std::wstring, std::wstring> translationTable;

enum Language
{
	English,
	Russian
} extern language;
