#pragma once
#ifndef TEXT_INPUT_H

#include "Input.h"
#include <unordered_map>

class TextInput {
public:
    TextInput();

    // Обновление состояния ввода
    void update(double deltaTime);

    // Получение текущего текста ввода
    const std::wstring& getText() const;

    void MoveCursorToEnd();

    void SetText(const std::wstring& text);

    // Получение текущей позиции курсора
    size_t getCursorPosition() const;

    // Получение текста для отображения с курсором
    std::wstring getDisplayText() const;

    // Очистка буфера ввода
    void clear();

    // Переключение языка ввода
    void toggleLanguage();

    void MoveCursorLeft();

    void MoveCursorRight();

private:
    std::wstring inputBuffer;       // Буфер ввода текста.
    size_t cursorPosition;          // Позиция курсора в буфере.
    int currentLanguage;            // Текущий язык: 0 = английский, 1 = русский.
    bool shiftActive;               // Флаг: удерживается ли Shift.
    bool capsLockActive;            // Флаг: включён ли Caps Lock.
    bool languageSwitchTriggered;   // Используется для предотвращения повторного переключения языка при удержании.
    
    // Для мигания курсора
    double cursorBlinkTimer;
    bool cursorVisible;
    
    // Автоповтор: задержка и интервал (в секундах)
    double autoRepeatInitialDelay;
    double autoRepeatInterval;
    
    // Хранит накопленные таймеры для автоповтора для каждой клавиши
    std::unordered_map<Key, double> keyAutoRepeatTimers;

    // Обновление состояний модификаторов (Shift, Caps Lock и т.д.)
    void updateModifierKeys();

    // Обновление мигания курсора
    void updateCursorBlink(double deltaTime);

    // Обработка ввода с клавиатуры
    void processKeyboardInput(double deltaTime);

    // Определение символа для вставки на основе текущего состояния
    std::wstring translate(const const std::tuple<Key, std::wstring, std::wstring, std::wstring, std::wstring>& mapping);

    // Вставка символа в буфер ввода
    void insertCharacter(const std::wstring& ch);

    // Обработчик завершения ввода
    void onInputComplete(const std::wstring& text);
};


#endif