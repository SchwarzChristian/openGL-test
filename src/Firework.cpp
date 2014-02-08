/*
 * Firework.cpp
 *
 *  Created on: 08.02.2014
 *      Author: user
 */

#include "Firework.h"
#include "settings.h"

void Firework::move(float timegap) {
	this->speed.y -= grav * timegap;
	speed += wind * (1 - airFriction * timegap);
	speed *= 1 - airFriction * timegap;
	if (livetime < 1.0) {
		Color c(rnd(), rnd(), rnd());
		livetime = -1.0;
		for (int i = 0; i < 300; i++) {
			Point dir(rnd(-30.0, 30.0), rnd(-30.0, 30.0), rnd(-30.0, 30.0));
			new Spark(pos, dir, rnd(0.25, 1.5), c);
		}
	}
}

