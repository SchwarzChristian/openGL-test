/*
 * Spark.cpp
 *
 *  Created on: 08.02.2014
 *      Author: user
 */

#include "Spark.h"
#include "settings.h"

Spark::Spark(Point pos, Point speed, float time, Color c) : Particle(true, pos, wind + speed, time) {
	_initLt = time;
	this->_c = c;
	this->_da = _initLt;
}

void Spark::move(float timegap) {
	this->speed.y -= grav * timegap;
	speed += wind * (1 - airFriction * timegap);
	speed *= 1 - airFriction * timegap;
	_c.mod(0.0, 0.0, 0.0, -timegap / _initLt);
	_da -= timegap;
//	dAmb += _da / _initLt / 5000.f;
}

void Spark::draw() {
	Point p = pos;
	p.mod(speed.x / rainLen, speed.y / rainLen, speed.z / rainLen);
	float em[] = { _c.r, _c.g, _c.b };
	glMaterialfv(GL_FRONT, GL_EMISSION, em);
	glBegin(GL_LINES);
		glVertex3f(pos.x, pos.y, pos.z);
		glVertex3f(p.x, p.y, p.z);
	glEnd();
	glMaterialfv(GL_FRONT, GL_EMISSION, fpNull);
}

