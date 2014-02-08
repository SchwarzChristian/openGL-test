/*
 * drawing.cpp
 *
 *  Created on: 08.02.2014
 *      Author: user
 */

#include "drawing.h"
#include "API.h"

extern Texture
	roof,
	wall,
	ground,
	wave,
	smoke,
	door
;

extern int
	stored,
	maxStorage
;

void drawHouse(Point const& haus) {
	static GLuint id = 0;

	glPushMatrix();
	glTranslatef(haus.x, haus.y, haus.z);

	clRed.use();

	if (id > 0) glCallList(id);
	else {
		id = glGenLists(1);
		glNewList(id, GL_COMPILE);
			clWhite.use();
			wall.use();
			glBegin(GL_QUADS);
				// front
				glNormal3f(0, -1, 0);
				glTexCoord2f( 0,  0); glVertex3f(-2, -2,  3);
				glTexCoord2f( 0, 15); glVertex3f(-2, -2, -3);
				glTexCoord2f(10, 15); glVertex3f( 2, -2, -3);
				glTexCoord2f(10,  0); glVertex3f( 2, -2,  3);

				// back
				glNormal3f(0, 1, 0);
				glTexCoord2f(10,  0); glVertex3f( 2,  2,  3);
				glTexCoord2f(10, 15); glVertex3f( 2,  2, -3);
				glTexCoord2f( 0, 15); glVertex3f(-2,  2, -3);
				glTexCoord2f( 0,  0); glVertex3f(-2,  2,  3);

				// left
				glNormal3f(-1, 0, 0);
				glTexCoord2f( 0,  0); glVertex3f(-2,  2,  3);
				glTexCoord2f( 0, 15); glVertex3f(-2,  2, -3);
				glTexCoord2f(10, 15); glVertex3f(-2, -2, -3);
				glTexCoord2f(10,  0); glVertex3f(-2, -2,  3);

				// right
				glNormal3f(1, 0, 0);
				glTexCoord2f(10,  0); glVertex3f( 2, -2,  3);
				glTexCoord2f(10, 15); glVertex3f( 2, -2, -3);
				glTexCoord2f( 0, 15); glVertex3f( 2,  2, -3);
				glTexCoord2f( 0,  0); glVertex3f( 2,  2,  3);
			glEnd();
			glBegin(GL_TRIANGLES);
				glNormal3f(-1, 0, 0);
				glTexCoord2f( 0,  0.5); glVertex3f(-2,  2, 3);
				glTexCoord2f(10,  0.5); glVertex3f(-2, -2, 3);
				glTexCoord2f( 5,  5.5); glVertex3f(-2,  0, 5);

				glNormal3f(1, 0, 0);
				glTexCoord2f(10,  0.5); glVertex3f(2, -2, 3);
				glTexCoord2f( 0,  0.5); glVertex3f(2,  2, 3);
				glTexCoord2f( 5,  5.5); glVertex3f(2,  0, 5);
			glEnd();
			roof.use();
			glBegin(GL_QUADS);
				glNormal3f(0, 1, 1);
				glTexCoord2f( 0, 10); glVertex3f( 2,  0,  5);
				glTexCoord2f( 0,  0); glVertex3f( 2,  2,  3);
				glTexCoord2f(15,  0); glVertex3f(-2,  2,  3);
				glTexCoord2f(15, 10); glVertex3f(-2,  0,  5);

				glNormal3f(0, -1, 1);
				glTexCoord2f( 0, 10); glVertex3f(-2,  0,  5);
				glTexCoord2f( 0,  0); glVertex3f(-2, -2,  3);
				glTexCoord2f(15,  0); glVertex3f( 2, -2,  3);
				glTexCoord2f(15, 10); glVertex3f( 2,  0,  5);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
		glEndList();
	}
	glPopMatrix();
}

void drawFWsphere() {
	const Point p(0,0,1);
	drawSphere(p, 0.5, clRed);
}

void drawHUD() {
    GLfloat ambientColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };



    glMatrixMode(GL_PROJECTION);		// switch to pojection matrix
	glLoadIdentity();					// clear new
	gluOrtho2D(0.0, 1.0, 0.0, 1.0);		// set 2D

	glMatrixMode(GL_MODELVIEW);			// back to modelview
	glLoadIdentity();

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	float h = 0.8 * stored / float(maxStorage);
	clRed.use();
	glBegin(GL_LINES);
		glVertex2f(0.49, 0.5);
		glVertex2f(0.51, 0.5);
		glVertex2f(0.5, 0.49);
		glVertex2f(0.5, 0.51);
	glEnd();
	glBegin(GL_QUADS);
		glVertex2f(0.05, 0.1);
		glVertex2f(0.1,  0.1);
		glVertex2f(0.1,  0.1 + h);
		glVertex2f(0.05, 0.1 + h);
	glEnd();
	clBlack.use();
	glBegin(GL_LINE_LOOP);
		glVertex2f(0.05, 0.1);
		glVertex2f(0.1,  0.1);
		glVertex2f(0.1,  0.9);
		glVertex2f(0.05, 0.9);
	glEnd();
//	glTranslatef(0.1, 0.01, 0);
//	drawString("Hello World!");
}

void drawDoor() {
	static GLuint id = 0;
	if (id > 0) glCallList(id);
	else {
		if (!(id = glGenLists(2))) return;
		glNewList(id + 1, GL_COMPILE_AND_EXECUTE); // front sides

		glBegin(GL_QUADS);
			glNormal3f(0, -1, 0);
			glVertex3f(-0.1, 0.13, 0);
			glVertex3f( 0.1, 0.13, 0);
			glVertex3f( 0.1, 0.13, 1);
			glVertex3f(-0.1, 0.13, 1);

			glVertex3f( 0.9, 0.13, 0);
			glVertex3f( 1.0, 0.13, 0);
			glVertex3f( 1.0, 0.13, 1);
			glVertex3f( 0.9, 0.13, 1);

			glVertex3f( 0.1, 0.13, 0);
			glVertex3f( 0.9, 0.13, 0);
			glVertex3f( 0.9, 0.13, 0.1);
			glVertex3f( 0.1, 0.13, 0.1);

			glVertex3f( 0.1, 0.13, 0.9);
			glVertex3f( 0.9, 0.13, 0.9);
			glVertex3f( 0.9, 0.13, 1);
			glVertex3f( 0.1, 0.13, 1);

			glVertex3f( 0.1, 0.1, 0.9);
			glVertex3f( 0.9, 0.1, 0.9);
			glVertex3f( 0.9, 0.1, 1);
			glVertex3f( 0.1, 0.1, 1);
		glEnd();

		glEndList();
	}
}

