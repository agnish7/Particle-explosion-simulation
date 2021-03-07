/*
 * Particle.h
 * 
 * Created on: 6 Mar 2021
 *     Author: agnishd
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <stdlib.h>

namespace caveofprogramming {

class Particle {
    private:
    double m_x;
    double m_y;
    double m_speed;
    double m_direction;
    void init();

    public:
    Particle();
    virtual ~Particle();
    double get_x() {return m_x;}
    double get_y() {return m_y;}
    void update(int interval); // Update the position of the particle.
    
};

} /* namespace caveofprogramming */

#endif /* PARTICLE_H_ */