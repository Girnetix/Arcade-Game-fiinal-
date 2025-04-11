#include"TextInput.h"

// Реализация конструктора
TextInput::TextInput()
    : currentLanguage(0), shiftActive(false), capsLockActive(false),
    languageSwitchTriggered(false), cursorBlinkTimer(0.0), cursorVisible(true),
    autoRepeatInitialDelay(0.5), autoRepeatInterval(0.05), cursorPosition(0), inputBuffer(L"") {
}

// Метод обновления состояния ввода
void TextInput::update(double deltaTime) {
    updateModifierKeys();
    updateCursorBlink(deltaTime);
    processKeyboardInput(deltaTime);
}

// Получение текущего текста ввода
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

// Получение текущей позиции курсора
size_t TextInput::getCursorPosition() const {
    return cursorPosition;
}

// Получение текста для отображения с курсором
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

// Очистка буфера ввода
void TextInput::clear() {
    inputBuffer=L"";
    cursorPosition = 0;
}

// Переключение языка ввода
void TextInput::toggleLanguage() {
    currentLanguage = (currentLanguage + 1) % 2;
}

// Обновление состояний модификаторов (Shift, Caps Lock и т.д.)
void TextInput::updateModifierKeys() {
    auto shiftState = Keyboard::GetKey(Key::LSHIFT);
    shiftActive = (shiftState.bHeld || shiftState.bPressed);

    // Обработка Caps Lock: переключение происходит при нажатии.
    auto capsState = Keyboard::GetKey(Key::CAPS_LOCK);
    if (capsState.bPressed) {
        capsLockActive = !capsLockActive;
    }

    // Обработка смены языка: если одновременно удерживаются Shift и Alt, переключаем язык.
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

// Обновление мигания курсора
void TextInput::updateCursorBlink(double deltaTime) {
    cursorBlinkTimer += deltaTime;
    if (cursorBlinkTimer >= 0.5) {
        cursorVisible = !cursorVisible;
        cursorBlinkTimer = std::fmod(cursorBlinkTimer, 0.5);
    }
}


// Определяет, какой символ вставить в зависимости от языка и режима Shift
std::wstring TextInput::translate(const std::tuple<Key, std::wstring, std::wstring, std::wstring, std::wstring>& mapping) {
    if (currentLanguage == 0) { // английский
        return shiftActive ? std::get<2>(mapping) : std::get<1>(mapping);
    }
    else { // русский
        return shiftActive ? std::get<4>(mapping) : std::get<3>(mapping);
    }
}

// Вставляем символ (строку) в позицию курсора и смещаем курсор
void TextInput::insertCharacter(const std::wstring& ch) {
    inputBuffer.insert(cursorPosition, ch);
    cursorPosition += ch.size();
}

// Действие при завершении ввода (Enter). Можно вызывать колбэк или отправлять чат.
void TextInput::onInputComplete(const std::wstring& text) {
    // Реализуйте здесь обработку завершённого ввода:
    // например, отправку сообщения в чат или установку имени игрока.
}

// Обрабатываем ввод: добавляем символы, автоповтор и специальные клавиши.
void TextInput::processKeyboardInput(double deltaTime) {
    // Обработка спец. клавиш: Backspace удаляет символ перед курсором
    auto backState = Keyboard::GetKey(Key::BACKSPACE);
    if (backState.bPressed) {
        if (cursorPosition > 0 && !inputBuffer.empty()) {
            inputBuffer.erase(cursorPosition - 1, 1);
            cursorPosition--;
        }
    }
    // Обработка Enter – завершаем ввод (например, отправляем текст)
    auto enterState = Keyboard::GetKey(Key::RETURN);
    if (enterState.bPressed) {
        onInputComplete(inputBuffer);
        clear();
    }

    // Проходим по карте клавиатуры и обрабатываем буквы/символы
    for (const auto& mapping : KeyboardMap) {
        auto state = Keyboard::GetKey(std::get<0>(mapping));
        if (state.bPressed) {
            // Если клавиша нажата, сразу вставляем символ
            std::wstring charToAdd = translate(mapping);
            insertCharacter(charToAdd);
            keyAutoRepeatTimers[std::get<0>(mapping)] = 0.0; // начинаем отсчёт автоповтора
        }
        else if (state.bHeld) {
            // Если клавиша удерживается, обновляем таймер автоповтора
            keyAutoRepeatTimers[std::get<0>(mapping)] += deltaTime;
            if (keyAutoRepeatTimers[std::get<0>(mapping)] >= autoRepeatInitialDelay) {
                // После первичной задержки вычисляем число повторений
                double repeatTime = keyAutoRepeatTimers[std::get<0>(mapping)] - autoRepeatInitialDelay;
                int numRepeats = static_cast<int>(repeatTime / autoRepeatInterval);
                if (numRepeats > 0) {
                    std::wstring charToAdd = translate(mapping);
                    for (int i = 0; i < numRepeats; i++) {
                        insertCharacter(charToAdd);
                    }
                    // Обновляем таймер, вычитая интервал повторов
                    keyAutoRepeatTimers[std::get<0>(mapping)] -= numRepeats * autoRepeatInterval;
                }
            }
        }
        else if (state.bReleased) {
            // Убираем таймер автоповтора, если клавиша отпущена
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

   

  