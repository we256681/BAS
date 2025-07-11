#include "KeyboardEmulation.h"
#include "converter.h"
#include <windows.h>
#include "emoji.h"

int KeyboardEmulation::GetNativeCode(int key)
{
    if(key == 27) return 1;
    if(key == 49) return 2;
    if(key == 50) return 3;
    if(key == 51) return 4;
    if(key == 52) return 5;
    if(key == 53) return 6;
    if(key == 54) return 7;
    if(key == 55) return 8;
    if(key == 56) return 9;
    if(key == 57) return 10;
    if(key == 48) return 11;
    if(key == 189) return 12;
    if(key == 187) return 13;
    if(key == 8) return 14;
    if(key == 9) return 15;
    if(key == 81) return 16;
    if(key == 87) return 17;
    if(key == 69) return 18;
    if(key == 82) return 19;
    if(key == 84) return 20;
    if(key == 89) return 21;
    if(key == 85) return 22;
    if(key == 73) return 23;
    if(key == 79) return 24;
    if(key == 80) return 25;
    if(key == 219) return 26;
    if(key == 221) return 27;
    if(key == 13) return 28;
    if(key == 65) return 30;
    if(key == 83) return 31;
    if(key == 68) return 32;
    if(key == 70) return 33;
    if(key == 71) return 34;
    if(key == 72) return 35;
    if(key == 74) return 36;
    if(key == 75) return 37;
    if(key == 76) return 38;
    if(key == 186) return 39;
    if(key == 222) return 40;
    if(key == 192) return 41;
    if(key == 220) return 43;
    if(key == 90) return 44;
    if(key == 88) return 45;
    if(key == 67) return 46;
    if(key == 86) return 47;
    if(key == 66) return 48;
    if(key == 78) return 49;
    if(key == 77) return 50;
    if(key == 188) return 51;
    if(key == 190) return 52;
    if(key == 191) return 53;
    if(key == 32) return 57;
    return 0;
}


std::map<std::string, Variant> KeyboardEmulation::PrepareSpecialCharacterEvent(const std::string& Char)
{
    std::map<std::string, Variant> Params;
    Params["text"] = Variant(Char);
    return Params;
}

bool KeyboardEmulation::IsKeyboardCharacter(const std::string& Char)
{
    unsigned int Code = 0;
    if(Char.size() <= 4)
    {
        for (int i = 0; i < Char.size(); ++i)
        {
            Code += (unsigned char)Char[i] << (i * 8);
        }
    }
    bool Res = !IsEmoji(Code);

    return Res;
}


bool KeyboardEmulation::IsKeyDown(WPARAM wparam)
{
    return (GetKeyState(wparam) & 0x8000) != 0;
}

std::string KeyboardEmulation::GetDOMCode(int Key)
{
    if(Key == 71) return std::string("KeyG");
    if(Key == 66) return std::string("KeyB");
    if(Key == 89) return std::string("KeyY");
    if(Key == 72) return std::string("KeyH");
    if(Key == 78) return std::string("KeyN");
    if(Key == 85) return std::string("KeyU");
    if(Key == 74) return std::string("KeyJ");
    if(Key == 77) return std::string("KeyM");
    if(Key == 73) return std::string("KeyI");
    if(Key == 75) return std::string("KeyK");
    if(Key == 188) return std::string("Comma");
    if(Key == 79) return std::string("KeyO");
    if(Key == 76) return std::string("KeyL");
    if(Key == 190) return std::string("Period");
    if(Key == 80) return std::string("KeyP");
    if(Key == 186) return std::string("Semicolon");
    if(Key == 191) return std::string("Slash");
    if(Key == 219) return std::string("BracketLeft");
    if(Key == 222) return std::string("Quote");
    if(Key == 221) return std::string("BracketRight");
    if(Key == 220) return std::string("Backslash");
    if(Key == 81) return std::string("KeyQ");
    if(Key == 65) return std::string("KeyA");
    if(Key == 90) return std::string("KeyZ");
    if(Key == 87) return std::string("KeyW");
    if(Key == 83) return std::string("KeyS");
    if(Key == 88) return std::string("KeyX");
    if(Key == 69) return std::string("KeyE");
    if(Key == 68) return std::string("KeyD");
    if(Key == 67) return std::string("KeyC");
    if(Key == 82) return std::string("KeyR");
    if(Key == 70) return std::string("KeyF");
    if(Key == 86) return std::string("KeyV");
    if(Key == 84) return std::string("KeyT");
    if(Key == 33) return std::string("PageUp");
    if(Key == 34) return std::string("PageDown");
    if(Key == 49) return std::string("Digit1");
    if(Key == 50) return std::string("Digit2");
    if(Key == 51) return std::string("Digit3");
    if(Key == 52) return std::string("Digit4");
    if(Key == 53) return std::string("Digit5");
    if(Key == 54) return std::string("Digit6");
    if(Key == 55) return std::string("Digit7");
    if(Key == 56) return std::string("Digit8");
    if(Key == 57) return std::string("Digit9");
    if(Key == 48) return std::string("Digit0");
    if(Key == 192) return std::string("Backquote");
    if(Key == 189) return std::string("Minus");
    if(Key == 187) return std::string("Equal");
    if(Key == 8) return std::string("Backspace");
    if(Key == 38) return std::string("ArrowUp");
    if(Key == 40) return std::string("ArrowDown");
    if(Key == 37) return std::string("ArrowLeft");
    if(Key == 39) return std::string("ArrowRight");
    if(Key == 45) return std::string("Insert");
    if(Key == 46) return std::string("Delete");
    if(Key == 36) return std::string("Home");
    if(Key == 35) return std::string("End");
    if(Key == 112) return std::string("F1");
    if(Key == 113) return std::string("F2");
    if(Key == 114) return std::string("F3");
    if(Key == 115) return std::string("F4");
    if(Key == 116) return std::string("F5");
    if(Key == 117) return std::string("F6");
    if(Key == 118) return std::string("F7");
    if(Key == 119) return std::string("F8");
    if(Key == 120) return std::string("F9");
    if(Key == 121) return std::string("F10");
    if(Key == 122) return std::string("F11");
    if(Key == 123) return std::string("F12");
    if(Key == 9) return std::string("Tab");
    if(Key == 20) return std::string("CapsLock");
    if(Key == 16) return std::string("ShiftLeft");
    if(Key == 17) return std::string("ControlLeft");
    if(Key == 18) return std::string("AltLeft");
    if(Key == 32) return std::string("Space");
    if(Key == 18) return std::string("AltRight");
    if(Key == 17) return std::string("ControlRight");
    if(Key == 27) return std::string("Escape");
    if(Key == 13) return std::string("Enter");

    return std::string();
}

std::map<std::string, Variant> KeyboardEmulation::PrepareRawKeyboardEvent(KeyEvent Event, WPARAM WindowsVirtualKeyCode, LPARAM NativeVirtualKeyCode, int KeyboardPresses)
{
    std::map<std::string, Variant> Params;

    Params["modifiers"] = Variant(KeyboardPresses);

    //Text that would have been generated by the keyboard if no modifiers were pressed(except for shift).Useful for shortcut(accelerator) key handling(default: "").
    Params["unmodifiedText"] = Variant(std::string());

    //Unique key identifier(e.g., 'U+0041') (default: "").
    Params["keyIdentifier"] = Variant(std::string());

    Params["windowsVirtualKeyCode"] = Variant((int)WindowsVirtualKeyCode);

    Params["nativeVirtualKeyCode"] = Variant((int)NativeVirtualKeyCode);

    std::string Text;
    std::string Key;
    std::string Code;

    unsigned int ScanCode = (unsigned int)NativeVirtualKeyCode;
    ScanCode = (ScanCode >> 16) & 0xff;
    unsigned int VirtualCode = 0;

    if(Event == KeyEventCharacter)
    {
        VirtualCode = MapVirtualKey(ScanCode,MAPVK_VSC_TO_VK);
    }else
    {
        VirtualCode = WindowsVirtualKeyCode;
    }

    if(VirtualCode == VK_CONTROL)
    {
        Key = "Control";
        Code = "ControlLeft";
        Params["location"] = Variant(1);
    }else if(VirtualCode == VK_MENU)
    {
        Key = "Alt";
        Code = "AltLeft";
        Params["location"] = Variant(1);
    } else if(VirtualCode == VK_SHIFT)
    {
        Key = "Shift";
        Code = "ShiftLeft";
        Params["location"] = Variant(1);
    } else if(VirtualCode == VK_BACK)
    {
        Key = "Backspace";
        Code = "Backspace";
    } else if(VirtualCode == VK_TAB)
    {
        Key = "Tab";
        Code = "Tab";
    } else if(VirtualCode == VK_RETURN)
    {
        Key = "Enter";
        Text = "\r";
        Code = "Enter";
    } else if(VirtualCode == VK_ESCAPE)
    {
        Key = "Escape";
        Code = "Escape";
    } else if(VirtualCode == VK_PRIOR)
    {
        Key = "PageUp";
        Code = "PageUp";
    } else if(VirtualCode == VK_NEXT)
    {
        Key = "PageDown";
        Code = "PageDown";
    } else if(VirtualCode == VK_END)
    {
        Key = "End";
        Code = "End";
    } else if(VirtualCode == VK_HOME)
    {
        Key = "Home";
        Code = "Home";
    } else if(VirtualCode == VK_LEFT)
    {
        Key = "ArrowLeft";
        Code = "ArrowLeft";
    } else if(VirtualCode == VK_UP)
    {
        Key = "ArrowUp";
        Code = "ArrowUp";
    } else if(VirtualCode == VK_RIGHT)
    {
        Key = "ArrowRight";
        Code = "ArrowRight";
    } else if(VirtualCode == VK_DOWN)
    {
        Key = "ArrowDown";
        Code = "ArrowDown";
    } else if(VirtualCode == VK_INSERT)
    {
        Key = "Insert";
        Code = "Insert";
    } else if(VirtualCode == VK_DELETE)
    {
        Key = "Delete";
        Code = "Delete";
    } else
    {
        Code = GetDOMCode(VirtualCode);
        
        if(Event == KeyEventCharacter)
        {
            wchar_t CodeWChar = (wchar_t)WindowsVirtualKeyCode;
            std::wstring CodeWstring;
            CodeWstring.push_back(CodeWChar);
            Text = ws2s(CodeWstring);
            Key = Text;
        }else
        {
            std::vector<BYTE> KeyboardState(256,0);
            GetKeyboardState(KeyboardState.data());

            KeyboardState[VK_LCONTROL] = 0;
            KeyboardState[VK_RCONTROL] = 0;
            KeyboardState[VK_CONTROL] = 0;

            std::vector<wchar_t> ResultData(16,0);
            int Size = ToUnicode(VirtualCode, ScanCode, KeyboardState.data(), ResultData.data(), ResultData.size(), 0);



            if(Size ==1)
            {
                std::wstring ResultString(ResultData.begin(),ResultData.begin() + Size);
                Text = ws2s(ResultString);
                Key = Text;
            }else
            {
                Text.clear();
                Key = "Dead";
            }

        }

    }

    /*bool NeedToSetText = true;
    bool NeedToSetKey = true;

    if(WindowsVirtualKeyCode == VK_CONTROL)
    {
        Key = "Control";
        NeedToSetKey = false;
    }else if(WindowsVirtualKeyCode == VK_MENU)
    {
        Key = "Alt";
        NeedToSetKey = false;
    } else if(WindowsVirtualKeyCode == VK_SHIFT)
    {
        Key = "Shift";
        NeedToSetKey = false;
    } else if(WindowsVirtualKeyCode == VK_BACK)
    {
        Key = "Backspace";
        NeedToSetKey = false;
    } else if(WindowsVirtualKeyCode == VK_TAB)
    {
        Key = "Tab";
        NeedToSetKey = false;
    } else if(WindowsVirtualKeyCode == VK_RETURN)
    {
        Key = "Enter";
        Text = "\r";
        NeedToSetText = false;
        NeedToSetKey = false;
    } else if(WindowsVirtualKeyCode == VK_ESCAPE)
    {
        Key = "Escape";
        NeedToSetKey = false;
    } else if(WindowsVirtualKeyCode == VK_PRIOR)
    {
        Key = "PageUp";
        NeedToSetKey = false;
    } else if(WindowsVirtualKeyCode == VK_NEXT)
    {
        Key = "PageDown";
        NeedToSetKey = false;
    } else if(WindowsVirtualKeyCode == VK_END)
    {
        Key = "End";
        NeedToSetKey = false;
    } else if(WindowsVirtualKeyCode == VK_HOME)
    {
        Key = "Home";
        NeedToSetKey = false;
    } else if(WindowsVirtualKeyCode == VK_LEFT)
    {
        Key = "ArrowLeft";
        NeedToSetKey = false;
    } else if(WindowsVirtualKeyCode == VK_UP)
    {
        Key = "ArrowUp";
        NeedToSetKey = false;
    } else if(WindowsVirtualKeyCode == VK_RIGHT)
    {
        Key = "ArrowRight";
        NeedToSetKey = false;
    } else if(WindowsVirtualKeyCode == VK_DOWN)
    {
        Key = "ArrowDown";
        NeedToSetKey = false;
    } else if(WindowsVirtualKeyCode == VK_INSERT)
    {
        Key = "Insert";
        NeedToSetKey = false;
    } else if(WindowsVirtualKeyCode == VK_DELETE)
    {
        Key = "Delete";
        NeedToSetKey = false;
    }

    if(Event == KeyEventCharacter)
    {
        if(NeedToSetText)
        {
            std::wstring TextWideChar;
            TextWideChar.push_back((wchar_t)WindowsVirtualKeyCode);
            Text = ws2s(TextWideChar);
        }
        unsigned int ScanCode = (unsigned int)NativeVirtualKeyCode;
        ScanCode = (ScanCode >> 16) & 0xff;
        unsigned int VirtualKey = MapVirtualKey(ScanCode,MAPVK_VSC_TO_VK);
        Code = GetDOMCode(VirtualKey);

    }else
    {
        if(NeedToSetText)
        {
            unsigned int VirtualKey = (unsigned int)WindowsVirtualKeyCode;
            unsigned int ScanCode = (unsigned int)NativeVirtualKeyCode;
            ScanCode = (ScanCode >> 16) & 0xff;
            std::vector<BYTE> KeyboardState(256,0);
            GetKeyboardState(KeyboardState.data());
            std::vector<wchar_t> ResultData(16,0);
            int Size = ToUnicode(VirtualKey, ScanCode, KeyboardState.data(), ResultData.data(), ResultData.size(), 0);
            std::wstring ResultString(ResultData.begin(),ResultData.begin() + Size);
            Text = ws2s(ResultString);
        }
        Code = GetDOMCode(WindowsVirtualKeyCode);
    }

    if(NeedToSetKey)
    {
        Key = Text;
    }*/

    if(Event != KeyEventCharacter)
    {
        Text.clear();
    }

    Params["text"] = Variant(Text);

    std::string TypeName = "char";
    if(Event == KeyEventDown)
    {
        //Chrome requires rawKeyDown instead of keyDown if text is empty
        //https://github.com/puppeteer/puppeteer/blob/main/packages/puppeteer-core/src/common/Input.ts#L120
        //https://gitlab.com/bablosoft/bastickets/-/issues/1073
        if(Text.empty())
        {
            TypeName = "rawKeyDown";
        }else
        {
            TypeName = "keyDown";
        }
    } else if(Event == KeyEventUp)
    {
        TypeName = "keyUp";
    }

    Params["type"] = Variant(TypeName);

    //Unique DOM defined string value describing the meaning of the key in the context of active modifiers, keyboard layout, etc(e.g., 'AltGr') (default: "").
    Params["key"] = Variant(Key);

    //Unique DOM defined string value for each physical key(e.g., 'KeyA') (default: "").
    Params["code"] = Variant(Code);

    return Params;
}

std::map<std::string, Variant> KeyboardEmulation::PrepareKeyboardEvent(KeyEvent Event, const std::string& Char, int KeyboardPresses)
{
    std::map<std::string, Variant> Params;
    
    Params["modifiers"] = Variant(KeyboardPresses);
    
    //Text that would have been generated by the keyboard if no modifiers were pressed(except for shift).Useful for shortcut(accelerator) key handling(default: "").
    Params["unmodifiedText"] = Variant(std::string());

    //Unique key identifier(e.g., 'U+0041') (default: "").
    Params["keyIdentifier"] = Variant(std::string());

    wchar_t Letter = 0;
    unsigned char Key = 0;
    std::string FirstKey;
    std::string KeyCode;

    if(Char == CharacterCtrl)
    {
        Letter = Key = VK_CONTROL;
        KeyCode = "Control";
        Params["location"] = Variant(1);
    }else if(Char == CharacterAlt)
    {
        Letter = Key = VK_MENU;
        KeyCode = "Alt";
        Params["location"] = Variant(1);
    } else if(Char == CharacterShift)
    {
        Letter = Key = VK_SHIFT;
        KeyCode = "Shift";
        Params["location"] = Variant(1);
    } else if(Char == CharacterBackspace)
    {
        Letter = Key = VK_BACK;
        KeyCode = "Backspace";
    } else if(Char == CharacterTab)
    {
        Letter = Key = VK_TAB;
        KeyCode = "Tab";
    } else if(Char == CharacterEnter)
    {
        Letter = Key = VK_RETURN;
        KeyCode = "Enter";
        FirstKey = "\r";
    } else if(Char == CharacterEscape)
    {
        Letter = Key = VK_ESCAPE;
        KeyCode = "Escape";
    } else if(Char == CharacterPageUp)
    {
        Letter = Key = VK_PRIOR;
        KeyCode = "PageUp";
    } else if(Char == CharacterPageDown)
    {
        Letter = Key = VK_NEXT;
        KeyCode = "PageDown";
    } else if(Char == CharacterEnd)
    {
        Letter = Key = VK_END;
        KeyCode = "End";
    } else if(Char == CharacterHome)
    {
        Letter = Key = VK_HOME;
        KeyCode = "Home";
    } else if(Char == CharacterLeft)
    {
        Letter = Key = VK_LEFT;
        KeyCode = "ArrowLeft";
    } else if(Char == CharacterUp)
    {
        Letter = Key = VK_UP;
        KeyCode = "ArrowUp";
    } else if(Char == CharacterRight)
    {
        Letter = Key = VK_RIGHT;
        KeyCode = "ArrowRight";
    } else if(Char == CharacterDown)
    {
        Letter = Key = VK_DOWN;
        KeyCode = "ArrowDown";
    } else if(Char == CharacterInsert)
    {
        Letter = Key = VK_INSERT;
        KeyCode = "Insert";
    } else if(Char == CharacterDelete)
    {
        Letter = Key = VK_DELETE;
        KeyCode = "Delete";
    } else
    {
        std::string Copy = Char;
        std::wstring WideChar = s2ws(Copy);
        Letter = WideChar[0];
        Key = 0;
        FirstKey = ws2s(WideChar.substr(0, 1));
        KeyCode = FirstKey;

        std::vector<HKL> Locales;
        int MaxLocales = 300;
        Locales.resize(MaxLocales);
        MaxLocales = GetKeyboardLayoutList(MaxLocales, Locales.data());

        int index = 0;
        char state = -1;
        bool IsKeyParsed = false;

        while((!IsKeyParsed || state == -1) && index < MaxLocales)
        {
            short c = VkKeyScanEx(Letter, Locales[index]);
            Key = c & 0xFF;
            IsKeyParsed = true;
            state = c >> 8;
            index++;
        }
    }

    if(Event == KeyEventCharacter)
    {
        Params["windowsVirtualKeyCode"] = Variant((int)Letter);
        Params["nativeVirtualKeyCode"] = Variant(GetNativeCode(Key));
    } else
    {
        Params["windowsVirtualKeyCode"] = Variant((int)Key);
        Params["nativeVirtualKeyCode"] = Variant(GetNativeCode(Key));
    }

    //Text as generated by processing a virtual key code with a keyboard layout. Not needed for for keyUp and rawKeyDown events (default: "")
    bool IsTextEmpty = false;
    if(Event == KeyEventCharacter)
    {
        IsTextEmpty = false;
        Params["text"] = Variant(FirstKey);
    }
    else
    {
        IsTextEmpty = true;
        Params["text"] = Variant(std::string());
    }

    std::string TypeName = "char";
    if(Event == KeyEventDown)
    {
        //Chrome requires rawKeyDown instead of keyDown if text is empty
        //https://github.com/puppeteer/puppeteer/blob/main/packages/puppeteer-core/src/common/Input.ts#L120
        //https://gitlab.com/bablosoft/bastickets/-/issues/1073
        if(IsTextEmpty)
        {
            TypeName = "rawKeyDown";
        }else
        {
            TypeName = "keyDown";
        }
    } else if(Event == KeyEventUp)
    {
        TypeName = "keyUp";
    }

    Params["type"] = Variant(TypeName);

    //Unique DOM defined string value describing the meaning of the key in the context of active modifiers, keyboard layout, etc(e.g., 'AltGr') (default: "").
    Params["key"] = Variant(KeyCode);

    //Unique DOM defined string value for each physical key(e.g., 'KeyA') (default: "").
    Params["code"] = Variant(GetDOMCode(Key));

    return Params;
}
