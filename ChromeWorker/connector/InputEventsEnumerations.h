#ifndef INPUTEVENTSENUMERATIONS_H
#define INPUTEVENTSENUMERATIONS_H

#include <string>

//Input enums
enum MouseButton
{
    MouseButtonNone = 0,
    MouseButtonLeft = 1,
    MouseButtonRight = 2,
    MouseButtonMiddle = 4,
};

enum MouseEvent
{
    MouseEventDown,
    MouseEventUp,
    MouseEventMove,
};

enum DragEvent
{
    DragEventEnter,
    DragEventOver,
    DragEventDrop,
    DragEventCancel,
};

enum TouchEvent
{
    TouchEventDown,
    TouchEventUp,
    TouchEventMove,
};

enum KeyEvent
{
    KeyEventDown,
    KeyEventUp,
    KeyEventCharacter,
};

enum KeyboardModifiers
{
    KeyboardModifiersNone = 0,
    KeyboardModifiersAlt = 1,
    KeyboardModifiersCtrl = 2,
    KeyboardModifiersShift = 8,
};

extern const std::string CharacterCtrl;
extern const std::string CharacterAlt;
extern const std::string CharacterShift;
extern const std::string CharacterBackspace;
extern const std::string CharacterTab;
extern const std::string CharacterEnter;
extern const std::string CharacterEscape;
extern const std::string CharacterPageUp;
extern const std::string CharacterPageDown;
extern const std::string CharacterEnd;
extern const std::string CharacterHome;
extern const std::string CharacterLeft;
extern const std::string CharacterUp;
extern const std::string CharacterRight;
extern const std::string CharacterDown;
extern const std::string CharacterInsert;
extern const std::string CharacterDelete;


#endif // DEVTOOLSACTIONSETPROXY_H
