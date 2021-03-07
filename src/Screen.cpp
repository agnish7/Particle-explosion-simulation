/* Screen.cpp */

#include "Screen.h"
#include <math.h>

namespace caveofprogramming {

Screen::Screen() : 
m_window(NULL),
m_renderer(NULL),
m_texture(NULL),
m_buffer(NULL),
m_buffer2(NULL) {

    // We cant return a value from the constructor to say whether the previous stuff suceeded or failed.
    // Constructors can throw exceptions though.


}



bool Screen::init() {
    const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		return false;
	}

	m_window = SDL_CreateWindow(
		"Paticle fire explosion", 
		SDL_WINDOWPOS_UNDEFINED, // xpos
		SDL_WINDOWPOS_UNDEFINED, // ypos
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN
	);

	if(m_window == NULL) {
		SDL_Quit();
		return false;
	}

	m_renderer = SDL_CreateRenderer(
		m_window,
		-1, //
		SDL_RENDERER_PRESENTVSYNC // Rendered needs to be synchronised with the vertical refresh rate of the screen.
	);

	if(m_renderer == NULL) {
		SDL_DestroyWindow(m_window);
		SDL_Quit();
        return false;
	}

	m_texture = SDL_CreateTexture(
		m_renderer,
		SDL_PIXELFORMAT_RGBA8888, // A number denoting no. of bytes per pixel.
		SDL_TEXTUREACCESS_STATIC,
		SCREEN_WIDTH,
		SCREEN_HEIGHT
	);

	if(m_texture == NULL) {
        SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
        return false;
	}

	m_buffer = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
	m_buffer2 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];

	// Generally you'd want to check for any exceptions that would be raised if memory
	// can't be allocated.

	// memset is a useful command that updates
	memset(m_buffer, 0, SCREEN_WIDTH*SCREEN_HEIGHT*sizeof(Uint32));
	memset(m_buffer2, 0, SCREEN_WIDTH*SCREEN_HEIGHT*sizeof(Uint32));

    SDL_UpdateTexture(
		m_texture,
		NULL,
		m_buffer,
		SCREEN_WIDTH * sizeof(Uint32) // Pitch or the number of bytes between rows of data.
	);
	SDL_RenderClear(m_renderer);
	SDL_RenderCopy(m_renderer, m_texture, NULL, NULL); // 1st 
    // null is to specify if we want the entire renderer and the second for the entire texture.
	SDL_RenderPresent(m_renderer);

    // If all works finr return true;
    return true;
}

void Screen::close() {
	delete[] m_buffer;
	delete[] m_buffer2;
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyTexture(m_texture);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

bool Screen::processEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				return false;
			}
		}
    return true;;
}

void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {
    Uint32 color = 0;

    color += red;
    color <<= 8;
    color += green;
    color <<= 8;
    color += blue;
    color <<= 8;
    color += 0xFF; // Keep the transparency value fixed to opaque.

    y = y % SCREEN_HEIGHT;
    x = x % SCREEN_WIDTH;

    if(y < 1) {
        y += 600;
    }
    if(x < 1){
        x += 800;
    }

    m_buffer[(y * SCREEN_WIDTH) + x] = color;
}

void Screen::update() {
    SDL_UpdateTexture(
		m_texture,
		NULL,
		m_buffer,
		SCREEN_WIDTH * sizeof(Uint32) // Pitch or the number of bytes between rows of data.
	);
	SDL_RenderClear(m_renderer);
	SDL_RenderCopy(m_renderer, m_texture, NULL, NULL); // 1st 
    // null is to specify if we want the entire renderer and the second for the entire texture.
	SDL_RenderPresent(m_renderer);
}

void Screen::drawCircle(int x, int y, int r, int t, Uint8 red, Uint8 green, Uint8 blue) {
    for(int rad = r; rad < r + t; rad++) {
        for(int i = x - rad; i < x + rad; i++) {
            int j1 = y + sqrt(     (rad*rad)   -   ((i - x) * (i - x))   );
            int j2 = y - sqrt(     (rad*rad)   -   ((i - x) * (i - x))   );
            setPixel(i, j1, red, green, blue);
            setPixel(i, j2, red, green, blue);
        }
    }
}

void Screen::drawLine(int m, int c, Uint8 red, Uint8 green, Uint8 blue) {
    for(int i = 0; i < SCREEN_HEIGHT; i++) {
        setPixel(i, m*i + c, red, green, blue);
    }
}

/*
void Screen::clear() {
    memset(m_buffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
	memset(m_buffer2, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
}
*/

void Screen::boxBlur() {

	Uint32* temp = m_buffer;
	m_buffer = m_buffer2;
	m_buffer2 = temp;

	for(int y = 0; y < SCREEN_HEIGHT; y++) {
		for(int x = 0; x < SCREEN_WIDTH; x++) {
			
			int redTotal = 0;
			int greenTotal = 0;
			int blueTotal = 0;

			for(int row = -1; row <= 1; row++) {
				for(int col = -1; col <= 1; col++) {
					int currentX = x + col;
					int currentY = y + row;

					if(currentX >= 0 && currentX < SCREEN_WIDTH && currentY >= 0 && currentY < SCREEN_HEIGHT) {
						Uint32 color = m_buffer2[currentY * SCREEN_WIDTH + currentX];
						Uint8 red  = color >> 24;
						Uint8 green = color >> 16;
						Uint8 blue = color >> 8;

						redTotal += red;
						greenTotal += green;
						blueTotal += blue;
					}
				}
			}

			Uint8 red = redTotal/9;
			Uint8 blue = blueTotal/9;
			Uint8 green = greenTotal/9;

			setPixel(x, y, red, green, blue);
		}
	}
}

void Screen::gaussianBlur() {
	int gaussian_kernel[5][5] = {{1, 4,  6, 4, 1},
                   		   {4, 16, 24, 16, 4},
                           {6, 24, 36, 24, 6},
                           {4, 16, 24, 16, 4},
                           {1, 4,  6, 4, 1}};

	Uint32* temp = m_buffer;
	m_buffer = m_buffer2;
	m_buffer2 = temp;

	for(int y = 0; y < SCREEN_HEIGHT; y++) {
		for(int x = 0; x < SCREEN_WIDTH; x++) {
			
			int redTotal = 0;
			int greenTotal = 0;
			int blueTotal = 0;

			for(int row = -2; row <= 2; row++) {
				for(int col = -2; col <= 2; col++) {
					int currentX = x + col;
					int currentY = y + row;

					int gaussianX = 2 + col;
					int gaussianY = 2 + row;

					if(currentX >= 0 && currentX < SCREEN_WIDTH && currentY >= 0 && currentY < SCREEN_HEIGHT) {
						Uint32 color = m_buffer2[currentY * SCREEN_WIDTH + currentX];
						Uint8 red  = color >> 24;
						Uint8 green = color >> 16;
						Uint8 blue = color >> 8;

						redTotal += gaussian_kernel[gaussianX][gaussianY] * (int)red;
						greenTotal += gaussian_kernel[gaussianX][gaussianY] * (int)green;
						blueTotal += gaussian_kernel[gaussianX][gaussianY] * (int)blue;
					}
				}
			}

			Uint8 red = redTotal/256;
			Uint8 blue = blueTotal/256;
			Uint8 green = greenTotal/256;

			setPixel(x, y, red, green, blue);
		}
	}
	
}
} /* caveofprogramming */