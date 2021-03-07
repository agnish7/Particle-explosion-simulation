/* Screen.h */

#ifndef SCREEN_H_
#define SCREEN_H_

#include <SDL.h>

namespace caveofprogramming {
    class Screen {
    public:
        const static int SCREEN_WIDTH = 800;
        const static int SCREEN_HEIGHT = 600;
    private:
        SDL_Window * m_window;
        SDL_Renderer * m_renderer;
        SDL_Texture * m_texture;
        Uint32 * m_buffer;
        Uint32 * m_buffer2;
    public:
        Screen();
        bool init();
        void close();
        bool processEvents();
        void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
        void update();
        void drawCircle(int x, int y, int r, int t, Uint8 red, Uint8 green, Uint8 blue);
        void drawLine(int m, int c, Uint8 red, Uint8 green, Uint8 blue);
        // void clear();
        void boxBlur();
        void gaussianBlur();
    };
} /* namespace caveofprogramming */

#endif /* SCREEN_H_ */