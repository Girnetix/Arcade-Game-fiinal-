#include"TextInput.h"

// ���������� ������������
TextInput::TextInput()
    : currentLanguage(0), shiftActive(false), capsLockActive(false),
    languageSwitchTriggered(false), cursorBlinkTimer(0.0), cursorVisible(true),
    autoRepeatInitialDelay(0.5), autoRepeatInterval(0.05), cursorPosition(0), inputBuffer(L"") {
}

// ����� ���������� ��������� �����
void TextInput::update(double deltaTime) {
    updateModifierKeys();
    updateCursorBlink(deltaTime);
    processKeyboardInput(deltaTime);
}

// ��������� �������� ������ �����
const std::wstring& TextInput::getText() const {
    return inputBuffer;
}

void TextInput::MoveCursorToEnd() {
    cursorPosition = inputBuffer.size();
}


void TextInput::SetText(const std::wstring& text)
{
    inputBuffer = text;
}

// ��������� ������� ������� �������
size_t TextInput::getCursorPosition() const {
    return cursorPosition;
}

// ��������� ������ ��� ����������� � ��������
std::wstring TextInput::getDisplayText() const {
    std::wstring display = inputBuffer;
    if (cursorVisible) {
        if (cursorPosition > display.size())
            display.push_back(L'_');
        else
            display.insert(cursorPosition, L"_");
    }
    return display;
}

// ������� ������ �����
void TextInput::clear() {
    inputBuffer=L"";
    cursorPosition = 0;
}

// ������������ ����� �����
void TextInput::toggleLanguage() {
    currentLanguage = (currentLanguage + 1) % 2;
}

// ���������� ��������� ������������� (Shift, Caps Lock � �.�.)
void TextInput::updateModifierKeys() {
    auto shiftState = Keyboard::GetKey(Key::LSHIFT);
    shiftActive = (shiftState.bHeld || shiftState.bPressed);

    // ��������� Caps Lock: ������������ ���������� ��� �������.
    auto capsState = Keyboard::GetKey(Key::CAPS_LOCK);
    if (capsState.bPressed) {
        capsLockActive = !capsLockActive;
    }

    // ��������� ����� �����: ���� ������������ ������������ Shift � Alt, ����������� ����.
    auto altState = Keyboard::GetKey(Key::LALT);
    if ((shiftState.bHeld || shiftState.bPressed) && (altState.bHeld || altState.bPressed)) {
        if (!languageSwitchTriggered) {
            toggleLanguage();
            languageSwitchTriggered = true;
        }
    }
    else {
        languageSwitchTriggered = false;
    }
}

// ���������� ������� �������
void TextInput::updateCursorBlink(double deltaTime) {
    cursorBlinkTimer += deltaTime;
    if (cursorBlinkTimer >= 0.5) {
        cursorVisible = !cursorVisible;
        cursorBlinkTimer = std::fmod(cursorBlinkTimer, 0.5);
    }
}


// ����������, ����� ������ �������� � ����������� �� ����� � ������ Shift
std::wstring TextInput::translate(const std::tuple<Key, std::wstring, std::wstring, std::wstring, std::wstring>& mapping) {
    if (currentLanguage == 0) { // ����������
        return shiftActive ? std::get<2>(mapping) : std::get<1>(mapping);
    }
    else { // �������
        return shiftActive ? std::get<4>(mapping) : std::get<3>(mapping);
    }
}

// ��������� ������ (������) � ������� ������� � ������� ������
void TextInput::insertCharacter(const std::wstring& ch) {
    inputBuffer.insert(cursorPosition, ch);
    cursorPosition += ch.size();
}

// �������� ��� ���������� ����� (Enter). ����� �������� ������ ��� ���������� ���.
void TextInput::onInputComplete(const std::wstring& text) {
    // ���������� ����� ��������� ������������ �����:
    // ��������, �������� ��������� � ��� ��� ��������� ����� ������.
}

// ������������ ����: ��������� �������, ���������� � ����������� �������.
void TextInput::processKeyboardInput(double deltaTime) {
    // ��������� ����. ������: Backspace ������� ������ ����� ��������
    auto backState = Keyboard::GetKey(Key::BACKSPACE);
    if (backState.bPressed) {
        if (cursorPosition > 0 && !inputBuffer.empty()) {
            inputBuffer.erase(cursorPosition - 1, 1);
            cursorPosition--;
        }
    }
    // ��������� Enter � ��������� ���� (��������, ���������� �����)
    auto enterState = Keyboard::GetKey(Key::RETURN);
    if (enterState.bPressed) {
        onInputComplete(inputBuffer);
        clear();
    }

    // �������� �� ����� ���������� � ������������ �����/�������
    for (const auto& mapping : KeyboardMap) {
        auto state = Keyboard::GetKey(std::get<0>(mapping));
        if (state.bPressed) {
            // ���� ������� ������, ����� ��������� ������
            std::wstring charToAdd = translate(mapping);
            insertCharacter(charToAdd);
            keyAutoRepeatTimers[std::get<0>(mapping)] = 0.0; // �������� ������ �����������
        }
        else if (state.bHeld) {
            // ���� ������� ������������, ��������� ������ �����������
            keyAutoRepeatTimers[std::get<0>(mapping)] += deltaTime;
            if (keyAutoRepeatTimers[std::get<0>(mapping)] >= autoRepeatInitialDelay) {
                // ����� ��������� �������� ��������� ����� ����������
                double repeatTime = keyAutoRepeatTimers[std::get<0>(mapping)] - autoRepeatInitialDelay;
                int numRepeats = static_cast<int>(repeatTime / autoRepeatInterval);
                if (numRepeats > 0) {
                    std::wstring charToAdd = translate(mapping);
                    for (int i = 0; i < numRepeats; i++) {
                        insertCharacter(charToAdd);
                    }
                    // ��������� ������, ������� �������� ��������
                    keyAutoRepeatTimers[std::get<0>(mapping)] -= numRepeats * autoRepeatInterval;
                }
            }
        }
        else if (state.bReleased) {
            // ������� ������ �����������, ���� ������� ��������
            keyAutoRepeatTimers.erase(std::get<0>(mapping));
        }
    }
}

void TextInput::MoveCursorLeft() {
    if (cursorPosition > 0) {
        cursorPosition--;
    }
}
void TextInput::MoveCursorRight() {
    if (cursorPosition < inputBuffer.size()) {
        cursorPosition++;
    }
}

   

  