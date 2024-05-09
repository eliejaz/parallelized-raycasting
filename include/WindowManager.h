#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vector>
#include <string>

#include <Vector.h>
#include <InputManager.h>
#include <Average.h>
#include <Texture.h>

/**
 * @brief Manages the window and graphics operations.
 *
 * The WindowManager class provides functionality for creating and managing a window,
 * handling user input, drawing lines and pixels, and updating the frames per second (FPS).
 * It encapsulates the X11 window system and provides an interface for interacting with it.
 */
class WindowManager
{
public:
    /**
     * @brief Constructs a WindowManager object with the specified width and height.
     * @param width The width of the window.
     * @param height The height of the window.
     */
    WindowManager(int width, int height);

    /**
     * @brief Destructor for the WindowManager object.
     */
    ~WindowManager();

    /**
     * @brief Retrieves the InputManager associated with the window.
     * @return A reference to the InputManager object.
     */
    InputManager &getInputManager();

    /**
     * @brief Gets the width of the window.
     * @return The width of the window.
     */
    int getWidth() const;

    /**
     * @brief Gets the height of the window.
     * @return The height of the window.
     */
    int getHeight() const;

    /**
     * @brief Draws a vertical line on the window.
     * @param x The x-coordinate of the line on the window.
     * @param yStart The starting y-coordinate of the line on the window.
     * @param yEnd The ending y-coordinate of the line on the window.
     * @param lineHeight The height of the line.
     * @param texture The texture to use for drawing the line.
     * @param texX The x-coordinate of the texture to start drawing from.
     * @param darken Whether to darken the line or not.
     */
    void drawVertLine(int x, int yStart, int yEnd, int lineHeight, Texture &texture, int texX, bool darken);

    /**
     * @brief Draws a pixel on the window.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param color The color of the pixel.
     */
    void drawPixel(int x, int y, unsigned int color);

    /**
     * @brief Flushes the window buffer to the screen.
     */
    void flush();

    /**
     * @brief Updates the average frames per second (FPS) counter with the current FPS.
     * @param fps The current frames per second.
     */
    void updateFPS(double fps);

private:
    int width, height; // The width and height of the window.

    int *imgBuffer; // The buffer for the window image.
    XImage *img;    // The X11 image for the window.

    int screen;       // The screen number of the window.
    Display *display; // The display of the window.
    Window window;    // The window.
    GC gc;            // The graphics context of the window.

    InputManager *inputManager; // The input manager for the window.
    Average fpsCounter;         // The average frames per second (FPS) counter for the window.
};

#endif