/*
 * Firework.h
 *
 *  Created on: 06.11.2013
 *      Author: user
 */

#ifndef FIREWORK_H_
#define FIREWORK_H_

#include "Spark.h"

class Firework : Particle {
public:
	Firework(Point pos, Point speed, float time = 5.0) : Particle(true, pos, speed, time + 1.0) {};
	void move(float timegap);
	void draw() { drawSphere(pos, 0.2, clRed); };
};

#endif /* FIREWORK_H_ */
