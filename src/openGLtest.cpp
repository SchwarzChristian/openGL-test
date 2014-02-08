#include <iostream>

//#include <GL/glut.h>

#include <math.h>
#include <iomanip>

#include "API.h"
#include "settings.h"
#include "Rain.h"
#include "Firework.h"
#include "drawing.h"

#define TEX_SIZE 1.0
#define SEL_BUF_SIZE 512

using namespace std;

const GLfloat cp[] = {
		-2.5, -2.5,  0.0,     0.0, -2.5,  2.0,     0.0, -2.5,  0.0,     2.5, -2.5, 2.0,
		-2.5,  0.0,  2.0,     0.0,  0.0,  0.0,     0.0,  0.0,  2.0,     2.5,  0.0, 0.0,
		-2.5,  0.0,  0.0,     0.0,  0.0,  2.0,     0.0,  0.0,  0.0,     2.5,  0.0, 2.0,
		-2.5,  2.5,  2.0,     0.0,  2.5,  0.0,     0.0,  2.5,  2.0,     2.5,  2.5, 0.0
};

Color bg(clSkyBlue);
Point angle, speed, sun, cursor;
Point wheel(5.0, 0, 0);
Map map;
Font font;

Texture
	roof,
	wall,
	ground,
	wave,
	smoke,
	door
;

int
	stored = 0,
	maxStorage = 100
;

bool
	light    = true,
	animate  = true,
	breaking = false,
	raining  = false,
	freecam  = false,
	poly_smo = true
;

void handleMouse(int x, int y) {
	static bool set = true;
	if (set) {
		map.cam.rotate((y - 200) / -sensity, 0.f,(x - 200) / -sensity);
		glutWarpPointer(200, 200);
		set = false;
	} else set = true;
}

void handleMouseButton(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP && stored < maxStorage) {
			map.mod(round(cursor.x), round(cursor.y), -0.1);
			stored++;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_UP && stored > 0) {
			map.mod(round(cursor.x), round(cursor.y), 0.1);
			stored--;
		}
		break;
	}
}

//Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27:
			shutdown("exit by user");
			break;

		// movement
		case key_f:
			speed.y =  moveSpeed;
			break;
		case key_b:
			speed.y = -moveSpeed;
			break;
		case key_r:
			speed.x =  moveSpeed;
			break;
		case key_l:
			speed.x = -moveSpeed;
			break;
		case key_jump:
			speed.z = 1.0;
			break;

		// interaction
		case key_select:
			GLuint selBuf[SEL_BUF_SIZE];
			GLuint hits;

			// prepare selection buffer
			glSelectBuffer(SEL_BUF_SIZE, selBuf);
			glRenderMode(GL_SELECT);

			// prepare selection box
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-0.1, 0.1, -0.1, 0.1, 0.1, 3.0);

			// prepare scene
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			map.cam.set();

			// prepare naming
			glInitNames();
			glPushName(1);

			// drawing selectable objects
			drawFWsphere();

			// do the work
			glFlush();

			// get hit count and back to render-mode
			hits = glRenderMode(GL_RENDER);

			// process hits
			if (hits > 0)
				cout << "hit" << endl;

			break;

		// debugging
		case key_light:
			light = !light;
			if (light) glShadeModel(GL_SMOOTH);
			else glShadeModel(GL_FLAT);
			break;
		case key_animate:
			animate = !animate;
			break;
		case key_break:
			breaking = !breaking;
			break;
		case key_rain:
			raining = !raining;
			break;
		case key_ctrl:
			freecam = !freecam;
			if (freecam) speed.z = 0;
			break;
	}
}

void handleKeyUp(unsigned char key, int x, int y) {
	switch (key) {
	case key_f:
	case key_b:
		speed.y = 0.0f;
		break;
	case key_l:
	case key_r:
		speed.x = 0.0f;
		break;
	}
}

void update(int c) {
	static int fwNext = 1;
	static int phase = 0;
	static Color cl = clRed;
	static Point p1, p2;
	const float timegap = 1 / 60.0;

	glutTimerFunc(1000 / 60.0, update, c);

	if (animate) {

		if (++c > 10000) c = 0;

		if (raining) for (int i = 0; i < 10; i++) {
			p2.set(0, 0, -rnd(rainSpeed * 0.9, rainSpeed * 1.1));
			p1.set(rnd(-100, 100), + rnd(-100, 100), rnd( 50, 75));
			new Rain(p1, p2);
		}

		p1.set(0.0, 0.0, 1.0);
		if (c == fwNext) {
			p2.set(rnd(-50.0, 50.0), rnd(-50.0, 50.0), rnd(100.0, 125.0));
			new Firework(p1, p2, rnd(0.5, 1.0));
			fwNext += floor(rnd(10.0, 15.0));
			if (fwNext >= 10000) fwNext -= 10000;
		}

		// fountain
		switch (phase) {
		case 0:
			cl.mod(-timegap, timegap, -timegap);
			if (cl.r <= 0.0) phase = rnd(3);
			break;
		case 1:
			cl.mod(-timegap, -timegap, timegap);
			if (cl.g <= 0.0) phase = rnd(3);
			break;
		case 2:
			cl.mod(timegap, -timegap, -timegap);
			if (cl.b <= 0.0) phase = rnd(3);
			break;
		}
		for (int i = 0; i < 20; i++) {
			p2.set(rnd(-25.0, 25.0), rnd(-25.0, 25.0), rnd(75.0, 100.0));
			new Spark(p1, p2, rnd(0.35, 0.75), cl);
		}

		//sun rotation
		if ((sun.z += sunSpeed * timegap) > 360) sun.z -= 360;

		//wheel
		if ((wheel.y += wheelRot) > 360) wheel.y -= 360;
		if ((wheel.z += wheelSpeed) > 360) wheel.z -= 360;
	}

//	dAmb = 0.f;
	moveParticles(timegap);

	//cam control
	map.cam.rotate(angle.x, angle.z, angle.y);
	if (freecam) {
		map.cam.moveRel(speed.x, speed.z, -speed.y);
	} else {
		p1 = map.cam.getAngle();
		speed.z -= grav * timegap;
		map.cam.move(dcos(p1.z) * speed.x + dsin(-p1.z) * speed.y,
				     dcos(-p1.z) * speed.y + dsin(p1.z) * speed.x, speed.z);
		map.setCam(true, 1.0);
	}

	map.cam.moveRel(0, 0, -2);
	cursor = map.cam.getPos();
	map.cam.moveRel(0, 0, 2);
	cursor.x = round(cursor.x);
	cursor.y = round(cursor.y);
	map.set(cursor);

    glutPostRedisplay();

}

Point haus[16];

void initWorld() {
	Point p(-50, -50, 0);
//	GLfloat texcoord[] = {
//			0.0, 0.0,  0.0, 4.0,
//			4.0, 0.0,  4.0, 4.0
//	};
	GLfloat _map[101][101];
	const GLfloat d = 0.5;
	for (int  i = 0; i < 101; i++)
		for (int j = 0; j < 101; j++) {
			int
				_i = ((i % 2) ? i / 2 + 1 : (-i / 2)) + 50,
				_j = ((j % 2) ? j / 2 + 1 : (-j / 2)) + 50,
				iprev = (_i > 50) ? _i - 1 : (_i + 1),
				jprev = (_j > 50) ? _j - 1 : (_j + 1);
			GLfloat _d = (i + j) / 1000.0 + d;
			if (i == 0)
				if (j == 0) _map[_i][_j] = rnd(-d, d);
				else _map[_i][_j] = _map[_i][jprev] + rnd(-d, _d);
			else
				if (j == 0) _map[_i][_j] = _map[iprev][_j] + rnd(-d, _d);
				else _map[_i][_j] = (_map[_i][jprev] + _map[iprev][_j]) / 2.0 + rnd(-d, _d);
		}
	map.init(_map[0], 101, 101, p, clWhite);
	map.cam.moveTo(10.0, 0.0, 1.0).rotate(90.0, 0.0, 0.0);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			map.set(haus[4*i+j].set(-15 + i * 10, -15 + j * 10, rnd() * 360));

//	glMap2f(GL_MAP2_VERTEX_3, 0.f, 1.f, 3, 4, 0.f, 1.f, 12, 4, cp);
//	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 0.5, 2, 2, 0, 0.5, 4, 2, texcoord);
//	glEnable(GL_MAP2_VERTEX_3);
//	glEnable(GL_MAP2_TEXTURE_COORD_2);
//	glMapGrid2f(30, 0.f, 1.f, 30, 0.f, 1.f);
	roof.load("textures/roof.png");
	wall.load("textures/wall.png");
	wave.load("textures/wave.png");
	smoke.load("textures/smoke.png");
	ground.load("textures/ground.png");
	font.init(0.02, 0.05, 0.1);
}

//Draws the 3D scene
void renderScene() {
	glClearColor(bg.r * dsin(sun.z), bg.g * dsin(sun.z), bg.b * dsin(sun.z), 1.0);
	prepareScene();


	// set CamAngle
	map.cam.set();

    Point p1(-100, -100), p2(100, 100), p3(-50, 0, 0);

    //Add ambient light
//    GLfloat ambientColor[] = {dAmb + 0.2f, dAmb + 0.2f, dAmb + 0.2f, 1.0f};

//    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	glPushMatrix();
	glRotatef(sun.z, 0, 1, 0);

	//Add positioned light
	GLfloat lightInt = min(abs(dsin(sun.z)) * 10, 1.f);
	if (sun.z > 180) {
		GLfloat em[] = { 1.f, 1.f, 1.f, 1.f};
		glMaterialfv(GL_FRONT, GL_EMISSION, em);
		lightInt *= 0.1;
		p3.mod(100);
	    drawSphere(p3, 0.5, clWhite);
	} else {
		GLfloat em[] = { 1.f, 1.f, 0.5f, 1.f};
		glMaterialfv(GL_FRONT, GL_EMISSION, em);
	    drawSphere(p3, 1.f, clYellow);
	}
	glMaterialfv(GL_FRONT, GL_EMISSION, fpNull);
    GLfloat lightColor0[] = { lightInt, lightInt, lightInt, 1.f };
    GLfloat lightPos0[] = { p3.x, p3.y, p3.z, 1.f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glPopMatrix();

	for (int i = 0; i < 16; i++)
		drawHouse(haus[i]);

	drawFWsphere();

	glPushMatrix();
	glRotatef(wheel.z, 0, 0, -1);
	glTranslatef(0, 10, 1);
	glRotatef(wheel.x, 1, 0, 0);
	glRotatef(wheel.y, 0, 1, 0);
	drawWheel(0.1, 10, clRed);
    glPopMatrix();

    GLfloat fogColor[] = {bg.r * dsin(sun.z), bg.r * dsin(sun.z), bg.r * dsin(sun.z), bg.a};
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 30.0f);
    glFogf(GL_FOG_END, 50.0f);

    glPushMatrix();
	ground.use();
	map.draw();
	glBindTexture(GL_TEXTURE_2D, 0);
	glTranslatef(10, 0, 1);
//	wave.use();
	clBlue.use();
//	glEvalMesh2(GL_FILL, 0, 30, 0, 30);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPolygonMode(GL_BACK, GL_LINE);
	glRotatef(90, 1, 0, 0);

	glTranslatef(0, 1, 0);
	clBlack.use();
	font.render("HEY THERE");
	glTranslatef(0, -.15, 0);
	font.render("I AM TESTING MY");
	glTranslatef(0, -.15, 0);
	font.render("FONT RENDERING FUNCTION");

	glPopMatrix();

	clRed.use();
	glBegin(GL_LINES);
		glVertex3f(cursor.x, cursor.y, cursor.z);
		glVertex3f(cursor.x, cursor.y, cursor.z + 1.0);
	glEnd();

	drawParticles();

	drawHUD();

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	initEngine(argc, argv);

	glEnable(GL_LIGHT0);

	initWorld();

	//set callback functions
	glutDisplayFunc(renderScene);
	glutKeyboardFunc(handleKeypress);
	glutKeyboardUpFunc(handleKeyUp);
	glutPassiveMotionFunc(handleMouse);
	glutMouseFunc(handleMouseButton);
	glutMotionFunc(handleMouse);
	glEnable(GL_FOG);

	glutWarpPointer(200, 200);
    glutSetCursor(GLUT_CURSOR_NONE);

	update(0);

    startEngine();
	return 0;
}
