#include <iostream>
#include <SDL.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "Screen.h"
#include "Swarm.hpp"

using namespace std;
using namespace caveofprogramming;

int main()
{
	// There are 4 values representing each pixel namely red, green, blue and alpha.
	// All 4 are packed into a single Uint32 integer.

	// Gameloop. This is a loop which will run for as long as the game is running.
	// In the meantime the colors and positions of particles and objects are updated.
	// We also constantly check for events and messages.
	// You process them as and when they happen.

	srand(time(NULL));

	caveofprogramming::Screen screen;

	if(screen.init() == false) {
		std::cout << "Error initialising SDL." << endl;
	} else {
		std::cout << "SDL Init suceeded." << std::endl;
	}

	Swarm swarm;

	int max = 0;

	while(true) {
		// SDL provides ways to check multiple things that can happen.
		double elapsed = SDL_GetTicks();
		//screen.clear();
		swarm.update(elapsed);

		unsigned char green =  (1 + sin(elapsed  * 0.0004)) * 128;
		unsigned char red = (1 + cos(elapsed  * 0.0002)) * 128;
		unsigned char blue = (1 + cos(elapsed  * 0.0001)) * 128;

		const Particle * const pParticles = swarm.getParticles();

		for(int i = 0; i < Swarm::NPARTICLES; i++) {
			

			Particle particle = pParticles[i];
			// You'll have to map the particle space to the actual screen space.

			int x = (particle.get_x() + 1) * 0.5 * Screen::SCREEN_WIDTH;
			int y = particle.get_y() * 0.5 * Screen::SCREEN_WIDTH +  0.5 * Screen::SCREEN_HEIGHT;

			screen.setPixel(x, y, red, green, blue);
		}

		screen.gaussianBlur();

		/*
		if(green > max)
			max = green;

		for(int y = 0; y < Screen::SCREEN_HEIGHT; y++) {
			for(int x = 0; x < Screen::SCREEN_WIDTH; x++) {
				screen.setPixel(x, y, red, green, blue);
			}
		}
		*/

		screen.update();

		if(screen.processEvents() == false) {
			break;
		}
	}

	cout << "Max: " << max << endl;

	screen.close();

	return 0;
}