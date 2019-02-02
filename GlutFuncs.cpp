#include "GlutFuncs.h"

void reshape(int w, int h);
extern void myDisplay();
extern bool pause;
extern void* font;
extern float FPS, spin;//, offset;
extern float X[], Y[];

float aspectRatio;

void initGL(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("DogBone Test");
	//glutFullScreen();
	aspectRatio = (GLUT_WINDOW_WIDTH / GLUT_WINDOW_HEIGHT);
	
	// register callbacks
	glutReshapeFunc(reshape);
    glutDisplayFunc(myDisplay);
	glutIdleFunc(myDisplay);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(pressNormalKeys);
	glutSpecialFunc(pressSpecialKeys);
	glutKeyboardUpFunc(releaseNormalKeys);
	glutSpecialUpFunc(releaseSpecialKeys);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

}

void pressNormalKeys(unsigned char key, int xx, int yy) 
{
	if(key == 27)
	{	cout<< "> ESC:\tExiting Program." << endl;
		exit(EXIT_FAILURE);
	}

	if(key == 32)
	{
		cout << "> SPB: Pause pressed." << endl;
		pause = !pause;
	}	
}

void releaseNormalKeys(unsigned char key, int xx, int yy)
{
}

void pressSpecialKeys(int key, int xx, int yy) 
{
} 

void releaseSpecialKeys(int key, int x, int y) 
{
} 

void renderBitmapString(float d, float e, float f, void *font, char *string)
{
    char *c;
    glRasterPos3f(d,e,f);
 
    for (c = string; *c != '\0'; c++)
        glutBitmapCharacter(font, *c);
}

void renderText2D(float a, float b, void* font, char* string)
{
    glPushMatrix();
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
            glPushMatrix();
                glLoadIdentity();
                glOrtho(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0, 1.0, 5000.0);
                renderBitmapString(a, b, -1.0f, font, string);
            glPopMatrix();
        glMatrixMode(GL_MODELVIEW);    
    glPopMatrix();
}

void printScreenText()
{
	// if(hud%2 == 0)
	// {
		char msg[80];
	    glColor3f(0, 0.6f, 0);

	//     sprintf(msg, "Position: ( %.03f, %.03f, %.03f )", camPos.x, camPos.y, camPos.z);
	//     renderText2D(5.0f, 20.0f, font, msg);

	//     sprintf(msg, "look Vec: < %.03f, %.03f, %.03f >", camLook.x, camLook.y, camLook.z);
	//     renderText2D(5.0f, 40.0f, font, msg);

	    sprintf(msg, "P0: %.3f, %.3f", X[0], Y[0]);
	    renderText2D(5.0f, 20.0f, font, msg);

	    sprintf(msg, "P1: %.3f, %.3f", X[1], Y[1]);
	    renderText2D(5.0f, 40.0f, font, msg);

	    sprintf(msg, "spin: %.3f", spin);
	    renderText2D(5.0f, 60.0f, font, msg);

	    sprintf(msg, "FPS: %.3f", FPS);
	    renderText2D(5.0f, 80.0f, font, msg);

	    // sprintf(msg, "offset: %.3f", offset);
	    // renderText2D(5.0f, 100.0f, font, msg);

	    sprintf(msg, "Press SB to Pause.");
	    renderText2D(5.0f, 120.0f, font, msg);

	// 	sprintf(msg, "+ Increase");
	//     renderText2D(5.0f, glutGet(GLUT_WINDOW_HEIGHT)-105.0f, font, msg);
	// 	sprintf(msg, "- Decrease");
	//     renderText2D(5.0f, glutGet(GLUT_WINDOW_HEIGHT)-85.0f, font, msg);
	// 	sprintf(msg, "W-Forward");
	//     renderText2D(5.0f, glutGet(GLUT_WINDOW_HEIGHT)-65.0f, font, msg);
	//    	sprintf(msg, "A-Left");
	//     renderText2D(5.0f, glutGet(GLUT_WINDOW_HEIGHT)-45.0f, font, msg);
	// 	sprintf(msg, "D-Right");
	//     renderText2D(5.0f, glutGet(GLUT_WINDOW_HEIGHT)-25.0f, font, msg);
	// 	sprintf(msg, "S-Back");
	//     renderText2D(5.0f, glutGet(GLUT_WINDOW_HEIGHT)-5.0f, font, msg);
	// }

}

void reshape(int w, int h) 
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0f / h;
	aspectRatio = ratio;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	//gluPerspective(perspective, aspectRatio, 0.1f, 10000.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}
