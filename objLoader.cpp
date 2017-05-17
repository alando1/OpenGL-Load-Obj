//header
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include "Obj.h"
#include "SuppliedGlutFuncs.h"

using namespace std;
//globals
Obj* myObj = NULL;
GLuint elephant;
float theta = 1;
char ch='1';

bool keyStates[256], keyTaps[256], loadHiRes;

/*--Camera Position and Look Vectors--*/
Vec3 camLook(0.0f,0.0f,-1.0f);
Vec3 camPos(0, 5, -10);
int centerX, centerY;
float add = 0.0f;
bool pause = false;

/*------View-Angles-------*/
float heading = 0.0f;
float pitch = 0.0f;
float perspective = 45.0f;
float aspectRatio = 1024.0f/768.0f; 
float dt = 1.0f;
float camSpeed = 0.5f;

void handleFunc(float dt)
{
  float vecx, vecz;

  if(keyTaps['.'])
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

    camPos.x+=camLook.x*(camSpeed*2.0f*dt);
    //camPos.y+=camLook.y;  //comment out to walk forward on plane
    camPos.z+=camLook.z*(camSpeed*2.0f*dt);
  }
  if(keyStates['r'] || keyStates['R'])
  {
    camPos.x+=camLook.x*(camSpeed*2.0f*dt);
    //camPos.y+=camLook.y;  //comment out to walk forward on plane
    camPos.z+=camLook.z*(camSpeed*2.0f*dt);
  }
  /*--------Move-Backwards------*/
  if(keyStates['s'] || keyStates['S'])
  {
    camPos.x-=camLook.x*(camSpeed*2.0f*dt);
    //y-=ly*speed;  //comment out to walk backwards on plane
    camPos.z-=camLook.z*(camSpeed*2.0f*dt);
  }
  /*--------Move-Left----------*/
  if(keyStates['a'] || keyStates['A'])
  {
    vecx = camLook.z;
    vecz = -camLook.x;
    camPos.x+= vecx*(camSpeed*2.0f*dt);
    camPos.z+= vecz*(camSpeed*2.0f*dt);
  }
  /*--------Move-Right----------*/
  if(keyStates['d'] || keyStates['D'])
  {

    vecx = camLook.z;
    vecz = -camLook.x;
    camPos.x-= vecx*(camSpeed*2.0f*dt);
    camPos.z-= vecz*(camSpeed*2.0f*dt);
  }

  if(keyStates['p'] || keyStates['P'])
  {
    add+= 0.2f;
  }
  if(keyStates['o'] || keyStates['O'])
  {
    add-= 0.2f;
  }

  if(keyStates['x'] || keyStates['X'])
    camPos.y += (camSpeed*1.30f*dt);

  if(keyStates['Z'] || keyStates['z'])
    camPos.y -= (camSpeed*1.30f*dt);
}

//other functions and main
void cleanUp();

void draw()
{
 	glPushMatrix();
	 	glTranslatef(0,-40.00,-105);
		glScalef(85.0f+add,85.0f+add,85.0f+add);

		if(pause == true)
			theta = 0;

		glRotatef(theta,0,1,0);
		myObj->draw();
	glPopMatrix();

	theta += 0.1f;
	if(theta > 360)
		theta -= 360;
}

void display(void)
{  

   	glClearColor (0.0,0.0,0.0,1.0); 
   	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    handleFunc(dt);
    glLoadIdentity();
    gluLookAt(camPos.x,camPos.y,camPos.z,camPos.x+camLook.x,camPos.y+camLook.y,camPos.z+camLook.z,0,1,0);
   	draw();
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

	myObj = new Obj("./data/stormtrooper/Stormtrooper.obj", "trooper"); 

	glutMainLoop();
	return 0;
}

void cleanUp()
{
  if(myObj)
  delete myObj;
  
  myObj = NULL;
}
