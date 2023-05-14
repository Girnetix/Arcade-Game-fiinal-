#include "GameMenu.h"

CMenu::MenuObject::MenuObject()
{
	bEnabled = true;
	bindedFunc = false;
	title = L"root";
	subtitle = L"";
	parent = nullptr;
}

CMenu::MenuObject::MenuObject(MenuObject* parent, const std::wstring& title, const std::wstring& subtitle)
{
	bEnabled = true;
	bindedFunc = false;
	this->title = title;
	this->subtitle = subtitle;
	this->parent = parent;
}

CMenu::MenuObject& CMenu::MenuObject::operator[](const std::wstring& name)
{
	if (itemPointer.count(name) == 0)
	{
		itemPointer[name] = items.size();
		items.push_back(MenuObject(this, name));
		auto &obj = items.back();
		obj.coord.x = (pWindow->GetScrWidth() - obj.title.length()) / 2;
		obj.coord.y = items.size() - 1;
		return *this;
	}

	return items[itemPointer[name]];
}

CMenu::MenuObject& CMenu::MenuObject::operator[](int index)
{
	return items[index];
}

CMenu::MenuObject& CMenu::MenuObject::Enable(bool state)
{
	bEnabled = state;
	return *this;
}

CMenu::MenuObject& CMenu::MenuObject::SetSubtitle(const std::wstring& subtitle)
{
	this->subtitle = subtitle;
	return *this;
}

CMenu::MenuObject& CMenu::MenuObject::GetParent()
{
	return *parent;
}

CMenu::MenuObject& CMenu::MenuObject::SetAction(std::function<void()> function)
{
	this->function = function;
	bindedFunc = true;
	return *this;
}

bool CMenu::MenuObject::Enabled()
{
	return bEnabled;
}

bool CMenu::MenuObject::HasChildren()
{
	return !items.empty();
}

size_t CMenu::MenuObject::ChildCount()
{
	return items.size();
}

size_t CMenu::MenuObject::TitleLength()
{
	return title.length();
}

void CMenu::MenuObject::DrawSelf()
{
	for (auto& object : items)
		pWindow->PrintMsg(object.coord.x, object.coord.y, object.Enabled() ? FG_WHITE : FG_DARK_GREY, object.title.c_str());
}

void CMenu::MenuObject::Build()
{ 
	for (auto& object : items)
	{
		if (object.HasChildren())
			object.Build();
		
		object.coord.y += (pWindow->GetScrHeight() - object.parent->ChildCount()) / 2;
	}
}

void CMenu::MenuObject::Execute()
{
	function();
}

int CMenu::MenuObject::GetX()
{
	return coord.x;
}

int CMenu::MenuObject::GetY()
{
	return coord.y;
}

CMenu::MenuManager::MenuManager() {}

void CMenu::MenuManager::Open(MenuObject* mo)
{
	Close();
	panels.push_back(mo);
}

void CMenu::MenuManager::Close()
{
	panels.clear();
}

void CMenu::MenuManager::Draw()
{
	if (panels.empty()) return;

	MenuObject *mo = panels.back();

	yMin = (pWindow->GetScrHeight() - mo->ChildCount()) / 2, yMax = yMin + mo->ChildCount();

	mo->DrawSelf();

	MenuObject pickedObject = (*mo)[yCurs];
	int length = pickedObject.TitleLength();
	pWindow->PrintMsg(pickedObject.GetX() - 2, pickedObject.GetY(), FG_CYAN, lCurs.c_str());
	pWindow->PrintMsg(pickedObject.GetX() + length, pickedObject.GetY(), FG_CYAN, rCurs.c_str());
	pWindow->PrintMsgInCenter(40, FG_WHITE, pickedObject.subtitle.c_str());
}

void CMenu::MenuManager::OnUp()
{
	yCurs--;
	if (yCurs + yMin < yMin)
		yCurs = panels.back()->ChildCount() - 1;
}

void CMenu::MenuManager::OnDown()
{
	yCurs++;
	if (yCurs + yMin > yMax - 1)
		yCurs = 0;
}

void CMenu::MenuManager::OnBack()
{
	if (panels.size() > 1)
		panels.pop_back();
}

void CMenu::MenuManager::OnSelect()
{
	MenuObject* mo = &(*panels.back())[yCurs];
	if (mo->HasChildren() && mo->Enabled())
		panels.push_back(mo);
	else if (mo->bindedFunc && mo->Enabled())
		mo->Execute();
}

void CMenu::DrawMenu()
{
	mm.Draw();

	if (menuUp.ButtonIsPressed())		menuUp.Execute();
	if (menuDown.ButtonIsPressed())		menuDown.Execute();
	if (menuBack.ButtonIsPressed())		menuBack.Execute();
	if (menuSelect.ButtonIsPressed())	menuSelect.Execute();
}

CMenu::CMenu()
{
	menuUp.SetButtonAction(Key::UP, Key::Key_W, [&]() {
		mm.OnUp();
		});
	menuDown.SetButtonAction(Key::DOWN, Key::Key_S, [&]() {
		mm.OnDown();
		});
	menuBack.SetButtonAction(Key::ESCAPE, [&]() {
		mm.OnBack();
		});
	menuSelect.SetButtonAction(Key::RETURN, [&]() {
		mm.OnSelect();
		});
}
