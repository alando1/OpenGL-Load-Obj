#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "Vec3.h"
#include "GlutFuncs.h"
#include <chrono>

float ToDegrees(float rad);
float ToRadians(float deg);
void rotateCoords();
void updateTime();
void updateFrames();

bool pause = false;
float dt = 1.0f, FPS = 0, frameCount = 0, fpsUpdateTime = 0;
void* font = GLUT_BITMAP_HELVETICA_18;
chrono::steady_clock::time_point currentTime, lastTime;
float spin = 1.0f;
float deltaSpin = 1.0f;
float offset = 0.0f;

//HORIZONTAL TEST
float X [6] = { 0.0f,  100.0f,  70.7f,  0.0f,  -100.0f,  -70.7f };
float Y [6] = { 0.0f,  0.0f,   -70.7f,  0.0f,    0.0f,	 -70.7f };

//UPSIDE DOWN TEST
// float X [6] = { 0.0f, -100.0f, -70.7f, 50.0f, 150.0f, 120.7f };
// float Y [6] = { 0.0f,    0.0f,  70.7f,  0.0f,   0.0f,  70.7f };

//VERTICAL TEST
// float X [6] = { 0.0f,     0.0f, -70.7f,  0.0f,     0.0f,  -70.7f };
// float Y [6] = { 0.0f,  -100.0f, -70.7f,  50.0f,	 150.0f,  120.7f };

void myInit()
{
	glPointSize(8.0f);
	glMatrixMode(GL_PROJECTION);
}

void myDisplay()
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(-200.0f, 200.0f, -200.0f, 200.0f);
	updateTime();

	// Kpattern coordinates
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	glPointSize(8.0f);
	for (int i = 0; i < 6; i++)
		glVertex2f(X[i], Y[i]);
	glEnd();

	// first half
	Vec2 v1 = Vec2(X[1] - X[0], Y[1] - Y[0]);
	Vec2 v2 = Vec2(X[2] - X[0], Y[2] - Y[0]);

	// determine angle between two vectors and bisecting angle
	float theta = acos((v1.dot(v2))/(v1.length() * v2.length()));
	theta /= 2.0f;	

	// lengths of triangle sides needed to find center point of curve
	float opp = v1.length() * tan(theta);
	float hyp = sqrt( pow(v1.length(), 2.0f) + pow(opp, 2.0f) );

	// normalize vector 
	Vec2 norm = Vec2(v1);
	norm.normalize();
	Vec2 prime = Vec2(0.0f, 0.0f);

	// apply rotation matrix in theta radians and scale to norm
	prime.x = norm.x * cos(-theta) - norm.y * sin(-theta);
	prime.y = norm.x * sin(-theta) + norm.y * cos(-theta);

	// center point is prime vector scaled by hyp translated from p0 position
	Vec2 center = Vec2(hyp * prime.x + X[0], hyp * prime.y + Y[0]);

	float diffRad = ToRadians((180.0f - 90.0f - abs(ToDegrees(theta))) * 2.0f);

	Vec2 offsetVec = Vec2(X[2] - center.x, Y[2] - center.y);
	Vec2 relVec = Vec2(X[1] - center.x, Y[1] - center.y);

	float offsetRad = acos( offsetVec.dot(relVec) / (offsetVec.length() * relVec.length()) );
	offset = ToDegrees(offsetRad);
	glLineWidth(3.0f);

	// draw center point
	glBegin(GL_POINTS);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(center.x, center.y);
	glEnd();

	// draw first leg
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	glVertex2f(X[0], Y[0]);
	glVertex2f(X[1], Y[1]);

	// compute the delta angle for curve iteration based off the number of segments
	int numOfSegments = 12;
	float delta = ToRadians((360.0f - ToDegrees(diffRad)) / (float)numOfSegments);
	Vec2 tmp = Vec2(0.0f, 0.0f);

	// compute points along the curve from p1 to p2
	for (float i = 2 * PI - diffRad; i > 0.0f; i -= delta)
	{
		glColor3f(i/3.926f, i/3.926f, 1.0f);
		tmp = Vec2(center.x + opp * cos(i - offsetRad), center.y + opp * sin(i - offsetRad));
		//tmp = Vec2(opp * cos(i), opp * sin(i));
		glVertex2f(tmp.x, tmp.y);
	}

	// Second half
	glVertex2f(X[2], Y[2]);
	glVertex2f(X[0], Y[0]);
	glVertex2f(X[3], Y[3]);
   	glVertex2f(X[4], Y[4]);
	glEnd();
	Vec2 v3 = Vec2(X[4] - X[3], Y[4] - Y[3]);
	Vec2 v4 = Vec2(X[5] - X[3], Y[5] - Y[3]);

	theta = acos(v3.dot(v4) / (v3.length() * v4.length()) ) / 2.0f;

	opp = v3.length() * tan(theta);
	hyp = sqrt(pow(v3.length(), 2.0f) + pow(opp, 2.0f));

	v3.normalize();
	prime.x = v3.x * cos(theta) - v3.y * sin(theta);
	prime.y = v3.x * sin(theta) + v3.y * cos(theta);

	center.x = hyp * prime.x + X[3];
	center.y = hyp * prime.y + Y[3];

	glBegin(GL_POINTS);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(center.x, center.y);
	glEnd();

	diffRad = ToRadians((180.0f - 90.0f - abs(ToDegrees(theta))) * 2.0f);
	Vec2 xVec = Vec2(1.0f, 0.0f);
	relVec = Vec2(X[4] - center.x, Y[4] - center.y);
	offsetRad = acos( xVec.dot(relVec) / (xVec.length() * relVec.length()) );
	delta = ToRadians(((360.0f - ToDegrees(diffRad)) / (float)numOfSegments));

	glBegin(GL_LINE_STRIP);
	for (float i = 0; i < 2 * PI - diffRad; i += delta)
	{
		glColor3f(i/3.926f, i/3.926f, 1.0f);
		tmp = Vec2(center.x + opp * cos(i + offsetRad), center.y + opp * sin(i + offsetRad));
		//tmp = Vec2(opp * cos(i), opp * sin(i));
		glVertex2f(tmp.x, tmp.y);
	}

	glVertex2f(X[5], Y[5]);
	glVertex2f(X[3], Y[3]);

	glEnd();
	glFlush();
   	printScreenText();
   	updateFrames();
    glutSwapBuffers(); //swap the buffers
}


int main(int argc, char *argv[])
{
	initGL(argc, argv);
	myInit();
	lastTime = chrono::steady_clock::now();
	glutMainLoop();
	return 0;
} 

float ToDegrees(float rad)
{
	return (rad * 180.0f / PI);
}

float ToRadians(float deg)
{
	return (deg * PI / 180.0f);
}

void updateTime()
{
	

	currentTime = chrono::steady_clock::now();
	dt = chrono::duration_cast<chrono::duration<float, milli>>(currentTime-lastTime).count();
    lastTime = currentTime;
    fpsUpdateTime += dt;
    frameCount++;

    if(!pause)
    {
    	deltaSpin = (360.0f * dt / 10000.0f);
		spin += deltaSpin;	// spins 2pi per 10 seconds

		if(spin >= 360.0f)
			spin -= 360.0f;

	    rotateCoords();
    }
}

void updateFrames()
{
	if (fpsUpdateTime > 100.0f)
	{
		FPS = static_cast<float>(frameCount) / fpsUpdateTime * 1000.0f;
		frameCount = 0;
		fpsUpdateTime = 0.0f;
	}
}

void rotateCoords()
{
	float phi = ToRadians(deltaSpin);
	float tmpx[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
	float tmpy[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

	Vec2 prime = Vec2(0.0f, 0.0f);
	Vec2 origin = Vec2((X[0] - X[3]) / 2.0f, (Y[0] - Y[3]) / 2.0f);
	
	for(int i = 0; i < 6; i++)
	{
		Vec2 vec = Vec2(X[i] - origin.x, Y[i] - origin.y);
		prime.x = vec.x * cos(phi) - vec.y * sin(phi);
		prime.y = vec.x * sin(phi) + vec.y * cos(phi);
		tmpx[i] = prime.x + origin.x;
		tmpy[i] = prime.y + origin.y;
	}

	// glBegin(GL_POINTS);
	// glPointSize(12.0f);
	// glColor3f(1.0f, 1.0f, 0.0f);

	// for ( int i = 0; i < 6; i++)
	// {
	// 	glVertex2f(tmpx[i], tmpy[i]);
	// }
	// glEnd();

	// Update array coordinates
	for (int i = 0; i < 6; i++)
	{
		X[i] = tmpx[i];
		Y[i] = tmpy[i];
	}
	
}
