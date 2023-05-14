#pragma once
#ifndef STREAM_H
#define STREAM_H

#include <vector>
#include <string>
#include <fstream>

constexpr int KByte = 1024;
constexpr int MByte = 1024 * 1024;

constexpr int MAX_STREAM_SIZE = 2 * KByte;
constexpr int MAX_FILE_STREAM_SIZE = 10 * MByte;

class CStream
{
public:
	CStream()
	{
		bufferSize = extractionOffset = 0;
		maxBufferSize = MAX_STREAM_SIZE;
		buffer.reserve(MAX_STREAM_SIZE);
	}
	CStream(uint32_t streamSize)
	{
		bufferSize = extractionOffset = 0;
		if (streamSize <= MAX_STREAM_SIZE)
		{
			maxBufferSize = streamSize;
			buffer.reserve(streamSize);
		}
		else
		{
			maxBufferSize = MAX_STREAM_SIZE;
			buffer.reserve(MAX_STREAM_SIZE);
		}	
	}
	~CStream()
	{
		bufferSize = extractionOffset = 0;
		buffer.clear();
	}

	void SetPos(uint32_t position) { extractionOffset = position; }

	int GetPos() { return extractionOffset; }
	int GetSize() { return bufferSize; }
	int GetMaxSize() { return maxBufferSize; }

	char* Data() { return buffer.data(); }

	void Read(void* data, uint32_t size);
	void Write(const void* data, uint32_t size);

	inline CStream& operator>>(bool& data)			           { Read(&data, sizeof(data)); return *this; }
	inline CStream& operator>>(char& data)			           { Read(&data, sizeof(data)); return *this; }
	inline CStream& operator>>(wchar_t& data)				   { Read(&data, sizeof(data)); return *this; }
	inline CStream& operator>>(short& data)			           { Read(&data, sizeof(data)); return *this; }
	inline CStream& operator>>(int& data)			           { Read(&data, sizeof(data)); return *this; }
	inline CStream& operator>>(long long& data)		           { Read(&data, sizeof(data)); return *this; }
	inline CStream& operator>>(double& data)                   { Read(&data, sizeof(data)); return *this; }
	inline CStream& operator>>(float& data)                    { Read(&data, sizeof(data)); return *this; }
	inline CStream& operator>>(unsigned char& data)            { Read(&data, sizeof(data)); return *this; }
	inline CStream& operator>>(unsigned short& data)           { Read(&data, sizeof(data)); return *this; }
	inline CStream& operator>>(unsigned int& data)             { Read(&data, sizeof(data)); return *this; }
	inline CStream& operator>>(unsigned long long& data)       { Read(&data, sizeof(data)); return *this; }

	CStream& operator>>(std::string& data);
	CStream& operator>>(std::wstring& data);

	inline CStream& operator<<(const bool& data)               { Write(&data, sizeof(data)); return *this; }
	inline CStream& operator<<(const char& data)               { Write(&data, sizeof(data)); return *this; }
	inline CStream& operator<<(const wchar_t& data)			   { Write(&data, sizeof(data)); return *this; }
	inline CStream& operator<<(const short& data)              { Write(&data, sizeof(data)); return *this; }
	inline CStream& operator<<(const int& data)                { Write(&data, sizeof(data)); return *this; }
	inline CStream& operator<<(const long long& data)          { Write(&data, sizeof(data)); return *this; }
	inline CStream& operator<<(const double& data)             { Write(&data, sizeof(data)); return *this; }
	inline CStream& operator<<(const float& data)              { Write(&data, sizeof(data)); return *this; }
	inline CStream& operator<<(const unsigned char& data)      { Write(&data, sizeof(data)); return *this; }
	inline CStream& operator<<(const unsigned short& data)     { Write(&data, sizeof(data)); return *this; }
	inline CStream& operator<<(const unsigned int& data)       { Write(&data, sizeof(data)); return *this; }
	inline CStream& operator<<(const unsigned long long& data) { Write(&data, sizeof(data)); return *this; }

	CStream& operator<<(const char* data);
	CStream& operator<<(const wchar_t* data);
	CStream& operator<<(const std::string& data);
	CStream& operator<<(const std::wstring& data);

protected:
	int bufferSize, maxBufferSize;
	int extractionOffset;
	std::vector<char> buffer;
};

class CFileStream :public CStream
{
public:
	CFileStream()
	{
		bufferSize = extractionOffset = 0;
		maxBufferSize = MAX_FILE_STREAM_SIZE;
		buffer.reserve(MAX_FILE_STREAM_SIZE);
	}
	CFileStream(const std::string& fileName) :CFileStream() { this->fileName = fileName; }
	~CFileStream() { Close(); }

	void Create();
	void Create(const std::string& fileName);
	void Open();
	void Open(const std::string& fileName);
	void Close();
	void Clear();
	void Delete();

	bool isOpen();
	bool endOfFile();
	int GetFileSize();
private:
	std::string fileName;
	std::fstream file;
};

#endif