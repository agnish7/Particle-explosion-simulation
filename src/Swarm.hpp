#ifndef SWARM_H
#define SWARM_H
#pragma once

#include "Particle.h"

namespace caveofprogramming {
	class Swarm  
	{
		private:
			// The use of const here ensures that once m_pParticles is set to point to the
			// array of particles, it can't point to anything else.
			Particle * m_pParticles;
			int lastTime;
		public:
			const static int NPARTICLES = 2000;
			Swarm();
			virtual ~Swarm();

			Particle* getParticles() {return m_pParticles; };
			void update(int elapsed);

	};
} /* namespace caveofprogramming */

#endif