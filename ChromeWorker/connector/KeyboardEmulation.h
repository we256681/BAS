#ifndef KEYBOARDEMULATION_H
#define KEYBOARDEMULATION_H

#include "Variant.h"
#include "InputEventsEnumerations.h"
#include <windows.h>

class KeyboardEmulation
{
    int GetNativeCode(int key);
    std::string GetDOMCode(int Key);
    bool IsKeyDown(WPARAM wparam);


	public:
        std::map<std::string, Variant> PrepareSpecialCharacterEvent(const std::string& Char);
        bool IsKeyboardCharacter(const std::string& Char);

		std::map<std::string, Variant> PrepareKeyboardEvent(KeyEvent Event, const std::string& Char, int KeyboardPresses);
        std::map<std::string, Variant> PrepareRawKeyboardEvent(KeyEvent Event, WPARAM WindowsVirtualKeyCode, LPARAM NativeVirtualKeyCode, int KeyboardPresses);
};

#endif // KEYBOARDEMULATION_H
