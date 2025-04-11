#include "Translation.h"

std::map<std::wstring, std::wstring> translationTable{};
Language language = Russian;

void InitTranslationTable(const std::string& filename)
{
	std::fstream file;
	file.open(filename, std::fstream::in);

	if (!file.is_open())
		return;

	std::string original, translation;
	std::string* current_string = nullptr; // ��������� �� ������� ������
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	char ch;

	while (file.read(&ch, sizeof(char))) {
		if (ch == '>') {
			// ������ ����� ������ ���������
			if (!original.empty() && !translation.empty()) {
				translationTable.emplace(std::wstring(converter.from_bytes(original)), std::wstring(converter.from_bytes(translation))); // ��������� ����
				pConsole->CPrintF(L"Added translation pair: (original: %s | translation: %s)", converter.from_bytes(original).c_str(), converter.from_bytes(translation).c_str());
				original.clear();
				translation.clear();
			}
			current_string = &original;
		}
		else if (ch == '<') {
			// ������ ������ ��������
			current_string = &translation;
		}
		else if (ch == '$') {
			// ����� �����
			if (!original.empty() && !translation.empty()) {
				translationTable.emplace(std::wstring(converter.from_bytes(original)), std::wstring(converter.from_bytes(translation))); // ��������� ��������� ����
				pConsole->CPrintF(L"Added translation pair: (original: %s | translation: %s)", converter.from_bytes(original).c_str(), converter.from_bytes(translation).c_str());
			}
			break;
		}
		else if (ch == '\n' || ch == '\r') {
			// ���������� ������� ����� ������
			continue;
		}
		else {
			// ��������� ������ � ������� ������
			if (current_string) {
				current_string->push_back(ch);
			}
		}
	}

	file.close();
}

void ClearTranslationTable()
{
	translationTable.clear();
}

const wchar_t* Translate(const std::wstring& wstr)
{
	auto translationPair = translationTable.find(wstr);
	
	if (translationPair != translationTable.end())
	{
		if (language == Language::English)
			return translationPair->first.c_str();
		else
			return translationPair->second.c_str();
	}

	return wstr.c_str();
}
