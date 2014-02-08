/*
 * Rain.h
 *
 *  Created on: 06.11.2013
 *      Author: user
 */

#ifndef RAIN_H_
#define RAIN_H_

#include "API.h"

class Rain : Particle {
	float size;
	Color cl;
public:
	Rain(Point pos, Point speed);
	void move(float timegap);
	void draw();
	~Rain() {};
};


#endif /* RAIN_H_ */
