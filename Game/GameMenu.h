#pragma once
#ifndef MENU_H
#define MENU_H

#include <Engine/Engine.h>
#include <unordered_map>

class CMenu
{
public:
	class MenuObject
	{
	public:
		friend class MenuManager;
		friend class CMenu;
		MenuObject();
		MenuObject(MenuObject* parent, const std::wstring& title, const std::wstring& subtitle = L"");
		MenuObject& operator[](const std::wstring& name);
		MenuObject& operator[](int index);
		MenuObject& Enable(bool state);
		MenuObject& SetSubtitle(const std::wstring& subtitle);
		MenuObject& GetParent();
		MenuObject& SetAction(std::function<void()> function);
		bool Enabled();
		bool HasChildren();
		size_t ChildCount();
		size_t TitleLength();
		void DrawSelf();
		void Build();
		void Execute();
		int GetX();
		int GetY();
	private:
		bool bEnabled, bindedFunc;
		std::wstring title, subtitle;
		std::unordered_map<std::wstring, size_t> itemPointer;
		std::vector<MenuObject> items;
		std::function<void()> function;
		MenuObject* parent;
		struct Coord
		{
			int x, y;
		} coord;
	};

	class MenuManager
	{
	public:
		MenuManager();
		void Open(MenuObject* mo);
		void Close();
		void Draw();
		void OnUp();
		void OnDown();
		void OnBack();
		void OnSelect();
	private:
		std::list<MenuObject*> panels;
		int yMin = 0, yMax = 0, yCurs = 0;
		std::wstring lCurs = L"->", rCurs = L"<-";
	};

public:
	void DrawMenu();
public:
	CMenu();
	MenuObject mo;
	MenuManager mm;
private:
	CButton menuUp, menuDown, menuBack, menuSelect;
};

#endif
