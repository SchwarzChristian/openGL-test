/*
 * Spark.h
 *
 *  Created on: 06.11.2013
 *      Author: user
 */

#ifndef SPARK_H_
#define SPARK_H_

#include "API.h"
/*
float
	dAmb = 0.f
;
*/
class Spark : Particle {
	float _initLt;
	Color _c;
	float _da;
public:
	Spark(Point pos, Point speed, float time = 1.0, Color c = clYellow);
	void move(float timegap);
	void draw();
};


#endif /* SPARK_H_ */
