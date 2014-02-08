/*
 * settings.h
 *
 *  Created on: 06.11.2013
 *      Author: user
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

//physics
const float
	airFriction = 0.1,
	grav = 6,
	moveSpeed = 0.2,
	rotSpeed = 2.0,
	sunSpeed = 1.4,
	wheelSpeed = 1.0,
	wheelRot = 10.0,
	rainSpeed = 100.0,
	rainLen = 30.0,
	sensity = 10,
	fpNull[] = {0.f, 0.f, 0.f, 0.f};
;

const Point
	wind(0.1, 0.0, -0.1)
;

//keymapping
const unsigned char
	// movement
	key_f  		  = 'w',
	key_b  		  = 's',
	key_l  		  = 'a',
	key_r  		  = 'd',
	key_jump      = ' ',

	// interact
	key_select    = 'e',

	// debugging
	key_light     = '1',
	key_animate   = 'q',
	key_break     = '#',
	key_rain      = 'r',
	key_firework  = 'f',
	key_ctrl      = '<'
;

#endif /* SETTINGS_H_ */
