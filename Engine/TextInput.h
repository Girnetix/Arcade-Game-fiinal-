#pragma once
#ifndef TEXT_INPUT_H

#include "Input.h"
#include <unordered_map>

class TextInput {
public:
    TextInput();

    // ���������� ��������� �����
    void update(double deltaTime);

    // ��������� �������� ������ �����
    const std::wstring& getText() const;

    void MoveCursorToEnd();

    void SetText(const std::wstring& text);

    // ��������� ������� ������� �������
    size_t getCursorPosition() const;

    // ��������� ������ ��� ����������� � ��������
    std::wstring getDisplayText() const;

    // ������� ������ �����
    void clear();

    // ������������ ����� �����
    void toggleLanguage();

    void MoveCursorLeft();

    void MoveCursorRight();

private:
    std::wstring inputBuffer;       // ����� ����� ������.
    size_t cursorPosition;          // ������� ������� � ������.
    int currentLanguage;            // ������� ����: 0 = ����������, 1 = �������.
    bool shiftActive;               // ����: ������������ �� Shift.
    bool capsLockActive;            // ����: ������� �� Caps Lock.
    bool languageSwitchTriggered;   // ������������ ��� �������������� ���������� ������������ ����� ��� ���������.
    
    // ��� ������� �������
    double cursorBlinkTimer;
    bool cursorVisible;
    
    // ����������: �������� � �������� (� ��������)
    double autoRepeatInitialDelay;
    double autoRepeatInterval;
    
    // ������ ����������� ������� ��� ����������� ��� ������ �������
    std::unordered_map<Key, double> keyAutoRepeatTimers;

    // ���������� ��������� ������������� (Shift, Caps Lock � �.�.)
    void updateModifierKeys();

    // ���������� ������� �������
    void updateCursorBlink(double deltaTime);

    // ��������� ����� � ����������
    void processKeyboardInput(double deltaTime);

    // ����������� ������� ��� ������� �� ������ �������� ���������
    std::wstring translate(const const std::tuple<Key, std::wstring, std::wstring, std::wstring, std::wstring>& mapping);

    // ������� ������� � ����� �����
    void insertCharacter(const std::wstring& ch);

    // ���������� ���������� �����
    void onInputComplete(const std::wstring& text);
};


#endif