/* Particle.cpp */
#include "Particle.h"
#include <stdlib.h>
#include <math.h>

namespace caveofprogramming {

Particle::Particle(){
    init();
}

Particle::~Particle() {
    // TODO Auto-generated destructor stub.
}

void Particle::init() {
    m_x = 0.0;
    m_y = 0.0;
    m_direction = 2 * M_PI * (double(double(rand())/double(RAND_MAX)));
    m_speed = 0.02 * (double(double(rand())/double(RAND_MAX)));

    m_speed = m_speed * m_speed;
}

void Particle::update(int interval) {

    m_direction += interval * 0.0005;

    double xspeed = m_speed * cos(m_direction);
    double yspeed = m_speed * sin(m_direction);

    m_x += xspeed * interval;
    m_y += yspeed * interval;

    if(m_x < -1 || m_y < -1 || m_y > 1) {
        init();
    }

    if(rand() < RAND_MAX/100) {
        init();
    }
}
} /* namespace caveofprogramming */