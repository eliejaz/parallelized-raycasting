#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <X11/Xlib.h>

/**
 * @brief The InputManager class handles user input for a display.
 */
class InputManager
{
public:
    /**
     * @brief Constructs an InputManager object.
     * @param display The display to handle input for.
     */
    InputManager(Display *display);

    /**
     * @brief Retrieves user input and updates the state of the keys.
     */
    void update();

    /**
     * @brief Checks if the up key (arrow) is pressed.
     * @return True if the up key is pressed, false otherwise.
     */
    bool up() const;

    /**
     * @brief Checks if the down key (arrow) is pressed.
     * @return True if the down key is pressed, false otherwise.
     */
    bool down() const;

    /**
     * @brief Checks if the right key (arrow) is pressed.
     * @return True if the right key is pressed, false otherwise.
     */
    bool right() const;

    /**
     * @brief Checks if the left key (arrow) is pressed.
     * @return True if the left key is pressed, false otherwise.
     */
    bool left() const;

    /**
     * @brief Checks if the escape key is pressed.
     * @return True if the escape key is pressed, false otherwise.
     */
    bool esc() const;

private:
    Display *display;  // The display to handle input for.
    unsigned int keys; // The current state of the keys (which keys are pressed or not pressed).

    /**
     * @brief Converts a KeySym to one of the below bit masks.
     * @param key The KeySym to convert.
     * @return The converted bit mask.
     */
    unsigned int convertKey(KeySym key);

    static unsigned int const KEY_UP = (1 << 0);    // Bit mask for the up key (arrow).
    static unsigned int const KEY_DOWN = (1 << 1);  // Bit mask for the down key (arrow).
    static unsigned int const KEY_RIGHT = (1 << 2); // Bit mask for the right key (arrow).
    static unsigned int const KEY_LEFT = (1 << 3);  // Bit mask for the left key (arrow).
    static unsigned int const KEY_ESC = (1 << 4);   // Bit mask for the escape key.
};

#endif