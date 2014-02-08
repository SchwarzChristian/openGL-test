/*
 * Rain.cpp
 *
 *  Created on: 08.02.2014
 *      Author: user
 */

#include "Rain.h"
#include "settings.h"

Rain::Rain(Point pos, Point speed) : Particle(true, pos, wind + speed, 60.0) {
	cl.set(0, 0, 1, 0.5);
	size = rnd(0.1, 0.2);
}

void Rain::move(float timegap) {
	this->speed.z -= grav * timegap;
	if (pos.z < -1) livetime = -1;
	speed += wind * (1 - airFriction * timegap);
	speed *= 1 - airFriction * timegap;
}

void Rain::draw() {
	glBegin(GL_LINES);
		cl.use();
		glVertex3f(pos.x, pos.y, pos.z);
		glVertex3f(pos.x + speed.x / rainLen, pos.y + speed.y / rainLen, pos.z + speed.z / rainLen);
	glEnd();
}

