#include <WindowManager.h>
#include <stdexcept>
#include <cstring>
#include <iostream>

WindowManager::WindowManager(int width, int height) : width(width), height(height), fpsCounter(1.0)
{
    if (!(display = XOpenDisplay(NULL)))
        throw std::runtime_error("Cannot connect to X server");

    imgBuffer = (int *)malloc(width * height * sizeof(int));
    memset(imgBuffer, 0, width * height * sizeof(int));

    screen = DefaultScreen(display);

    unsigned long black = BlackPixel(display, screen);

    window = XCreateSimpleWindow(display,
                                 RootWindow(display, screen),
                                 0, 0,
                                 width, height,
                                 0,
                                 black,
                                 black);

    XSelectInput(display, window, StructureNotifyMask | KeyPressMask | KeyReleaseMask);
    XMapWindow(display, window);
    gc = XCreateGC(display, window, 0, NULL);

    for (;;)
    {
        XEvent e;
        XNextEvent(display, &e);
        if (e.type == MapNotify)
            break;
    }

    inputManager = new InputManager(display);

    Visual *visual = DefaultVisual(display, screen);
    img = XCreateImage(display,
                       visual,
                       DefaultDepth(display, screen),
                       ZPixmap,
                       0,
                       (char *)imgBuffer,
                       width, height,
                       32,
                       0);
    if (!img)
        throw std::runtime_error("Cannot create image");
}

WindowManager::~WindowManager()
{
    XDestroyImage(img);
    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    delete inputManager;
}

InputManager &WindowManager::getInputManager() { return *inputManager; }
int WindowManager::getWidth() const { return width; }
int WindowManager::getHeight() const { return height; }

void WindowManager::drawVertLine(int x, int yStart, int yEnd, int lineHeight, Texture &texture, int texX, bool darken)
{
    double step = double(texture.getHeight()) / lineHeight;
    double texY = (yStart - height / 2 + lineHeight / 2) * step;
    for (int y = yStart; y <= yEnd; y++)
    {
        unsigned int color = texture.get(texX, int(texY));
        texY += step;
        if (darken)
            color = (color >> 1) & 8355711;
        imgBuffer[x + y * width] = color;
    }
}

void WindowManager::drawPixel(int x, int y, unsigned int color)
{
    imgBuffer[x + y * width] = color;
}

void WindowManager::flush()
{
    XPutImage(display, window, gc, img, 0, 0, 0, 0, width, height);

    std::string fpsStr = std::to_string(int(fpsCounter.get())) + " FPS";
    std::cout << "\r" << fpsStr << std::flush;
}

void WindowManager::updateFPS(double fps)
{
    fpsCounter.update(fps);
}