#include "Stream.h"

void CStream::Read(void* data, uint32_t size)
{
	if (extractionOffset + size <= bufferSize)
	{
		memcpy(data, &buffer[extractionOffset], size);
		extractionOffset += size;
	}
}

void CStream::Write(const void* data, uint32_t size)
{
	if (bufferSize + size <= maxBufferSize)
	{
		buffer.insert(buffer.end(), (char*)data, (char*)data + size);
		bufferSize += size;
	}
}


CStream& CStream::operator>>(std::string& data)
{
	uint32_t stringSize;
	*this >> stringSize;
	data.resize(stringSize);
	Read((char*)data.data(), stringSize);
	return *this;
}

CStream& CStream::operator>>(std::wstring& data)
{
	uint32_t stringSize;
	*this >> stringSize;
	data.resize(stringSize);
	Read((wchar_t*)data.data(), stringSize * 2);
	return *this;
}


CStream& CStream::operator<<(const char* data)
{
	uint32_t stringSize = strlen(data);
	*this << stringSize;
	Write(data, stringSize);
	return *this;
}

CStream& CStream::operator<<(const wchar_t* data)
{
	uint32_t stringSize = wcslen(data);
	*this << stringSize;
	Write(data, stringSize * 2);
	return *this;
}

CStream& CStream::operator<<(const std::string& data)
{
	*this << data.data();
	return *this;
}

CStream& CStream::operator<<(const std::wstring& data)
{
	*this << data.data();
	return *this;
}


void CFileStream::Create()
{
	try
	{
		file.open(fileName, std::fstream::out);
		file.close();
		file.open(fileName, std::fstream::in | std::fstream::out | std::fstream::binary);
	}
	catch (const std::exception& ex)
	{
		file.close();
	}
}

void CFileStream::Create(const std::string& fileName)
{
	try
	{
		file.open(fileName, std::fstream::out);
		file.close();
		file.open(fileName, std::fstream::in | std::fstream::out | std::fstream::binary);
		this->fileName = fileName;
	}
	catch (const std::exception& ex)
	{
		file.close();
	}
}

void CFileStream::Open()
{
	try
	{
		file.open(fileName, std::fstream::in | std::fstream::out | std::fstream::binary);
		file.read((char*)&bufferSize, sizeof(bufferSize));
		if (bufferSize > MAX_FILE_STREAM_SIZE)
			throw std::exception("Opening file is too big");
		buffer.resize(bufferSize);
		file.read(buffer.data(), bufferSize);
	}
	catch (const std::exception& ex)
	{
		Create(fileName);
	}
}

void CFileStream::Open(const std::string& fileName)
{
	try
	{
		file.open(fileName, std::fstream::in | std::fstream::out | std::fstream::binary);
		file.read((char*)&bufferSize, sizeof(bufferSize));
		if (bufferSize > MAX_FILE_STREAM_SIZE)
			throw std::exception("Opening file is too big");
		buffer.resize(bufferSize);
		file.read(buffer.data(), bufferSize);
		this->fileName = fileName;
	}
	catch (const std::exception& ex)
	{
		Create(fileName);
	}
}

void CFileStream::Close()
{
	if (!file.is_open())
		return;

	file.close();
	
	file.open(fileName, std::fstream::trunc | std::fstream::out | std::fstream::binary);
	
	file.write((const char*)&bufferSize, sizeof(bufferSize));
	file.write(buffer.data(), bufferSize);
	file.close();
}

void CFileStream::Clear()
{
	buffer.clear();
	buffer.reserve(MAX_FILE_STREAM_SIZE);
	extractionOffset = bufferSize = 0;
}

void CFileStream::Delete()
{
	std::remove(fileName.c_str());
}

bool CFileStream::isOpen()
{
	return file.is_open();
}

bool CFileStream::endOfFile()
{
	return file.eof();
}

int CFileStream::GetFileSize()
{
	return bufferSize;
}
