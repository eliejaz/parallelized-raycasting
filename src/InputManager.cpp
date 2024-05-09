#include <InputManager.h>

#include <X11/Xutil.h>

InputManager::InputManager(Display *display) : display(display), keys(0)
{
}

void InputManager::update()
{
    XEvent e;
    while (XPending(display))
    {
        XNextEvent(display, &e);
        switch (e.type)
        {
        case KeyPress:
            keys |= convertKey(XLookupKeysym(&e.xkey, 0));
            break;
        case KeyRelease:
            keys &= ~convertKey(XLookupKeysym(&e.xkey, 0));
            break;
        }
    }
}

unsigned int InputManager::convertKey(KeySym key)
{
    switch (key)
    {
    case XK_Up:
        return KEY_UP;
    case XK_Down:
        return KEY_DOWN;
    case XK_Left:
        return KEY_LEFT;
    case XK_Right:
        return KEY_RIGHT;
    case XK_Escape:
        return KEY_ESC;
    }
    return 0;
}

bool InputManager::up() const { return keys & KEY_UP; }
bool InputManager::down() const { return keys & KEY_DOWN; }
bool InputManager::right() const { return keys & KEY_RIGHT; }
bool InputManager::left() const { return keys & KEY_LEFT; }
bool InputManager::esc() const { return keys & KEY_ESC; }