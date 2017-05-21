#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include "Obj.h"
#include <chrono>
#include "SuppliedGlutFuncs.h"

using namespace std;

/*--Initial Camera Position and Look Vectors--*/
Vec3 camLook(0.0f,0.0f,-1.0f);
Vec3 camPos(0, 5, -10);
Vec3 prevPos = camPos;
int centerX, centerY, hud = 0;
float add = 1.0f, scale = 1.0f, theta = 1;
float heading = 0.0f, pitch = 0.0f, perspective = 45.0f, aspectRatio = 1024.0f/768.0f; 
float camSpeed = 0.1f, dt = 1.0f, FPS = 0, frameCount = 0, fpsUpdateTime = 0;
void* font = GLUT_BITMAP_HELVETICA_18;

chrono::steady_clock::time_point currentTime, lastTime;
bool keyStates[256], keyTaps[256], loadHiRes, pause = false;
Obj* myObj = NULL;

void handleFunc(float dt)
{
  float vecx, vecz;

  if(keyTaps['h'] || keyTaps['H'])
  	hud++;

  if(keyTaps['p'] || keyTaps['P'])
    pause = !pause;

  /*--------change perspective---------*/
  if(keyStates['-'])
  {
    perspective -= 0.01f;
    if(perspective <= 1.0f)
      perspective = 180.0f;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(perspective, aspectRatio, 0.1f, 10000.0f);
    glMatrixMode(GL_MODELVIEW);
  }
  if(keyStates['\\'])
  {
    perspective= 45.0f;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(perspective, aspectRatio, 0.1f, 10000.0f);
    glMatrixMode(GL_MODELVIEW);
  }
  if(keyStates['='])
  {
    perspective += 0.01f;
    if(perspective >= 180.0f)
      perspective = 0.0f;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(perspective, aspectRatio, 0.1f, 10000.0f);
    glMatrixMode(GL_MODELVIEW);
  }

  /*---------Fly-Foward---------*/
  if(keyStates['q'] || keyStates['Q'])
  {
    camPos+=camLook*(1.35f*dt);
  }

  /*--------Move-Foward---------*/
  if(keyStates['w'] || keyStates['W'])
  {

    camPos.x+=camLook.x*(camSpeed*1.3f*dt);
    //camPos.y+=camLook.y;  //comment out to walk forward on plane
    camPos.z+=camLook.z*(camSpeed*1.3f*dt);
  }
  if(keyStates['r'] || keyStates['R'])
  {
    camPos.x+=camLook.x*(camSpeed*1.3f*dt);
    //camPos.y+=camLook.y;  //comment out to walk forward on plane
    camPos.z+=camLook.z*(camSpeed*1.3f*dt);
  }
  /*--------Move-Backwards------*/
  if(keyStates['s'] || keyStates['S'])
  {
    camPos.x-=camLook.x*(camSpeed*1.3f*dt);
    //y-=ly*speed;  //comment out to walk backwards on plane
    camPos.z-=camLook.z*(camSpeed*1.3f*dt);
  }
  /*--------Move-Left----------*/
  if(keyStates['a'] || keyStates['A'])
  {
    vecx = camLook.z;
    vecz = -camLook.x;
    camPos.x+= vecx*(camSpeed*1.3f*dt);
    camPos.z+= vecz*(camSpeed*1.3f*dt);
  }
  /*--------Move-Right----------*/
  if(keyStates['d'] || keyStates['D'])
  {

    vecx = camLook.z;
    vecz = -camLook.x;
    camPos.x-= vecx*(camSpeed*1.3f*dt);
    camPos.z-= vecz*(camSpeed*1.3f*dt);
  }

  if(keyStates['+'] || keyStates['='])
    scale += add;
  if(keyStates['-'] || keyStates['_'])
    scale -= add;

  if(keyStates['x'] || keyStates['X'])
    camPos.y += (camSpeed*1.3f*dt);

  if(keyStates['Z'] || keyStates['z'])
    camPos.y -= (camSpeed*1.3f*dt);
}

//other functions and main
void cleanUp();

void draw()
{
 	glPushMatrix();
	 	glTranslatef(0,0.0f,-50.0f);
		glScalef(scale, scale, scale);

		if(pause == false)
			glRotatef(theta,0,1,0);

		myObj->draw();
	glPopMatrix();
}

void updateTime()
{
	theta += (360.0f * dt / 1000.0f);
	currentTime = chrono::steady_clock::now();
	dt = chrono::duration_cast<chrono::duration<float, milli>>(currentTime-lastTime).count();
    lastTime = currentTime;
    fpsUpdateTime += dt;
    frameCount++;
}

void updateFrames()
{
	if (fpsUpdateTime > 100.0f)
	{
		FPS = static_cast<float>(frameCount) / fpsUpdateTime * 1000.0f;
		frameCount = 0;
		fpsUpdateTime = 0.0f;
	}

	//Update prevPos
	prevPos = camPos;
}

void display()
{  

	updateTime();

   	glClearColor (0.0,0.0,0.0,1.0); 
   	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    handleFunc(dt);
    glLoadIdentity();
    gluLookAt(camPos.x,camPos.y,camPos.z,camPos.x+camLook.x,camPos.y+camLook.y,camPos.z+camLook.z,0,1,0);
   	draw();

   	printScreenText();
   	updateFrames();
    glutSwapBuffers(); //swap the buffers
    memset(keyTaps,0,256*sizeof(bool)); //resets input array for next frame

}

int main(int argc, char** argv)
{
  if(argc == 1)
  {
    loadHiRes = false;
    cout <<"> Loading Lo-Res textures ..." << endl;
  }
  else
  {
    if(argv[1][0] == '0')
    {
      cout <<"> Loading Lo-Res textures ..." << endl;
      loadHiRes = false;
    }
    else if(argv[1][0] == '1')
    {
      cout <<"> Loading Hi-Res textures ..." << endl;
      loadHiRes = true;
    }
    else 
    {
      cout <<"> Invalid arguements, loading Lo-Res textures ..." << endl;
      loadHiRes = false;
    }
  }
  initGL(argc, argv);
	
  atexit(cleanUp);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

	myObj = new Obj("./data/City/town.obj", "test"); 
    //myObj = new Obj("./data/stormtrooper/Stormtrooper.obj", "test");
	lastTime = chrono::steady_clock::now();
	glutMainLoop();
	return 0;
}

void cleanUp()
{
  if(myObj)
  delete myObj;
  
  myObj = NULL;
}
