#pragma once
#ifndef MEMORY_H
#define MEMORY_H

#include <exception>

template<typename Type>
class SmartPointer
{
public:
	SmartPointer() :ptr(nullptr), refCount(new uint32_t(0)) {}
	SmartPointer(Type* ptr) :ptr(ptr), refCount(new uint32_t(1)) {}
	SmartPointer(SmartPointer<Type>& Other)
	{
		if (this != &Other)
		{
			ptr = Other.ptr;
			refCount = Other.refCount;
			(*refCount)++;
		}
	}
	SmartPointer(SmartPointer<Type>&& Other) noexcept
	{
		if (this != &Other)
		{
			ptr = Other.ptr;
			refCount = Other.refCount;
			Other.ptr = nullptr;
			Other.refCount = nullptr;
		}
	}
	SmartPointer<Type>& operator=(SmartPointer<Type>& Right)
	{
		if (this != &Right)
		{
			reset();
			ptr = Right.ptr;
			refCount = Right.refCount;
			if (ptr != nullptr)
				(*refCount)++;
		}
		return *this;
	}
	SmartPointer<Type>& operator=(SmartPointer<Type>&& Right) noexcept
	{
		if (this != &Right)
		{
			reset();
			ptr = Right.ptr;
			refCount = Right.refCount;
			Right.ptr = nullptr;
			Right.refCount = nullptr;
		}
		return *this;
	}
	Type& operator*()
	{
		return *ptr;
	}
	Type* operator->()
	{
		return ptr;
	}
	Type* Get()
	{
		return ptr;
	}
	~SmartPointer()
	{
		reset();
	}
	void reset()
	{
		if (refCount)
		{
			if (*refCount > 0)
				(*refCount)--;
			if (*refCount == 0)
			{
				if (ptr != nullptr)
					delete ptr;
				delete refCount;
			}
			ptr = nullptr;
			refCount = nullptr;
		}
	}
private:
	Type* ptr = nullptr;
	uint32_t* refCount = nullptr;
};

template<typename Type>
class SmartPointer<Type[]>
{
public:
	SmartPointer() :ptr(nullptr), refCount(new uint32_t(0)) {}
	SmartPointer(Type* ptr) :ptr(ptr), refCount(new uint32_t(1)) {}
	SmartPointer(SmartPointer<Type[]>& Other)
	{
		if (this != &Other)
		{
			ptr = Other.ptr;
			refCount = Other.refCount;
			(*refCount)++;
		}
	}
	SmartPointer(SmartPointer<Type[]>&& Other) noexcept
	{
		if (this != &Other)
		{
			ptr = Other.ptr;
			refCount = Other.refCount;
			(*refCount)++;
			Other.reset();
		}
	}
	SmartPointer<Type[]>& operator=(SmartPointer<Type[]>& Right) 
	{
		if (this != &Right)
		{
			reset();
			ptr = Right.ptr;
			refCount = Right.refCount;
			if (ptr != nullptr)
				(*refCount)++;
		}
		return *this;
	}
	SmartPointer<Type[]>& operator=(SmartPointer<Type[]>&& Right) noexcept
	{
		if (this != &Right)
		{
			reset();
			ptr = Right.ptr;
			refCount = Right.refCount;
			if (ptr != nullptr)
				(*refCount)++;
			Right.reset();
		}
		return *this;
	}
	Type& operator*()
	{
		return *ptr;
	}
	Type* operator->()
	{
		return ptr;
	}
	Type* Get()
	{
		return ptr;
	}
	Type& operator[](int index)
	{
		if (index < 0)
			throw std::exception("Array out of range");
		return ptr[index];
	}
	~SmartPointer()
	{
		reset();
	}
	void reset()
	{
		if (refCount)
		{
			if (*refCount > 0)
				(*refCount)--;
			if (*refCount == 0)
			{
				if (ptr != nullptr)
					delete[] ptr;
				delete refCount;
			}
			ptr = nullptr;
			refCount = nullptr;
		}
	}
private:
	Type* ptr = nullptr;
	uint32_t* refCount = nullptr;
};

#endif
