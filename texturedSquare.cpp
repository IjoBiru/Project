///////////////////////////////////////////////////////////////////////////////////////////////////////
// texturedSquare.cpp
//
// Alya Hani Nurahma	  (5222600032)
// Haniefam Muslima Yahya (5222600060) 
//
// Texture Credits: See ExperimenterSource/Textures/TEXTURE_CREDITS.txt
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h> 
//#include <GL/glut.h>

#include "getBMP.h"

#define ONE_BY_ROOT_THREE 0.57735

GLfloat angle = 0.0f;

static int step = 0;

// Globals.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate torpedo.
static int isAnimate = 0; // In animation mode?
static int animationPeriod = 200; // Speed of animation.
static unsigned int texture[2]; // Array of texture ids.

static float angle = 0.0; // Angle to rotate textured square.
static int id = 0; // Currently displayed texture id.

GLfloat mat_amb_diff[] = { 0.7f, 0.7f, 0.7f, 1.0f };  // Ambien dan Diffuse
GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Spekular
GLfloat mat_shininess[] = { 50.0f };                    // Nilai shininess


float Color = 1.0f;

// Box vertex normal vectors = normalized unit vector pointing from origin to vertex.
/*static float normals[] =
{
	ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE,
	ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE,
	ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE,
	ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE,
	-ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE,
	-ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE,
	-ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE,
	-ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE
};*/

/*void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
}*/


// Load external textures.
void loadTextures()
{
	// Local storage for image data.
	imageFile* image[1];

	// Load the image.
	image[0] = getBMP("../../Textures/headset.bmp");

	// Create texture object texture[0]. 
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Specify image data for currently active texture object.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->width, image[0]->height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);

	// Set texture parameters for wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture parameters for filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}


// Initialization routine.
void setup(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	GLfloat light_position[] = { 0.0f, 0.0f, 1.0f, 0.0f }; // Cahaya datang dari arah z positif
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Warna cahaya putih

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	// Create texture ids.
	glGenTextures(2, texture);

	// Load external texture. 
	loadTextures();

	// Turn on OpenGL texturing.
	glEnable(GL_TEXTURE_2D);


}

void drawBox(void)
{
	glBegin(GL_QUADS);

	// Sisi depan
	glNormal3f(0.0f, 0.0f, 1.0f); // Normal untuk sisi depan
	glTexCoord2f(0.0, 0.0); glVertex3f(-10.0, -10.0, 5.0);   // Kiri Bawah
	glTexCoord2f(1.0, 0.0); glVertex3f(10.0, -10.0, 5.0);    // Kanan Bawah
	glTexCoord2f(1.0, 1.0); glVertex3f(10.0, 10.0, 5.0);     // Kanan Atas
	glTexCoord2f(0.0, 1.0); glVertex3f(-10.0, 10.0, 5.0);    // Kiri Atas

	// Sisi belakang
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(-10.0, -10.0, -5.0);  // Kiri Bawah
	glTexCoord2f(1.0, 0.0); glVertex3f(10.0, -10.0, -5.0);   // Kanan Bawah
	glTexCoord2f(1.0, 1.0); glVertex3f(10.0, 10.0, -5.0);    // Kanan Atas
	glTexCoord2f(0.0, 1.0); glVertex3f(-10.0, 10.0, -5.0);   // Kiri Atas

	// Sisi kiri
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(-10.0, -10.0, 5.0);   // Depan Bawah
	glTexCoord2f(0.0, 0.0); glVertex3f(-10.0, -10.0, -5.0);  // Belakang Bawah
	glTexCoord2f(0.0, 0.0); glVertex3f(-10.0, 10.0, -5.0);   // Belakang Atas
	glTexCoord2f(0.0, 0.0); glVertex3f(-10.0, 10.0, 5.0);    // Depan Atas

	// Sisi kanan
	glNormal3f(1.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(10.0, -10.0, 5.0);    // Depan Bawah
	glTexCoord2f(0.0, 0.0); glVertex3f(10.0, -10.0, -5.0);   // Belakang Bawah
	glTexCoord2f(0.0, 0.0); glVertex3f(10.0, 10.0, -5.0);    // Belakang Atas
	glTexCoord2f(0.0, 0.0); glVertex3f(10.0, 10.0, 5.0);     // Depan Atas

	// Sisi atas
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(-10.0, 10.0, 5.0);    // Kiri Depan
	glTexCoord2f(0.0, 0.0); glVertex3f(10.0, 10.0, 5.0);     // Kanan Depan
	glTexCoord2f(0.0, 0.0); glVertex3f(10.0, 10.0, -5.0);    // Kanan Belakang
	glTexCoord2f(0.0, 0.0); glVertex3f(-10.0, 10.0, -5.0);   // Kiri Belakang

	// Sisi bawah
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(-10.0, -10.0, 5.0);   // Kiri Depan
	glTexCoord2f(0.0, 0.0); glVertex3f(10.0, -10.0, 5.0);    // Kanan Depan
	glTexCoord2f(0.0, 0.0); glVertex3f(10.0, -10.0, -5.0);   // Kanan Belakang
	glTexCoord2f(0.0, 0.0); glVertex3f(-10.0, -10.0, -5.0);  // Kiri Belakang


	glEnd();

}

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE); // Normalisasi vektor normal
	glEnable(GL_DEPTH_TEST); 

	gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//glRotatef(angle, 0.0, 1.0, 0.0);

	glRotatef(Zangle, 0.0, 0.0, 1.0);
	glRotatef(Yangle, 0.0, 1.0, 0.0);
	glRotatef(Xangle, 1.0, 0.0, 0.0);

	// Activate texture object.
	glBindTexture(GL_TEXTURE_2D, texture[id]);

	glPushMatrix();
	drawBox();
	glPopMatrix();


	glutSwapBuffers();
}

void animate(int value)
{
	if (false)
	{
		angle += 10;
		if (angle > 360.0) angle -= 360.0;

		glutPostRedisplay();
		glutTimerFunc(animationPeriod, animate, 1);
	}
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();


}

// Keyboard input processing routine.
// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ':
		id = (id + 1) % 2; // Mengganti antara dua tekstur yang tersedia.
		glutPostRedisplay(); // Memperbarui tampilan untuk menerapkan perubahan tekstur.
		break;
	case 'x':
		Xangle += 5.0;
		if (Xangle > 360.0) Xangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'X':
		Xangle -= 5.0;
		if (Xangle < 0.0) Xangle += 360.0;
		glutPostRedisplay();
		break;
	case 'y':
		Yangle += 5.0;
		if (Yangle > 360.0) Yangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Y':
		Yangle -= 5.0;
		if (Yangle < 0.0) Yangle += 360.0;
		glutPostRedisplay();
		break;
	case 'z':
		Zangle += 5.0;
		if (Zangle > 360.0) Zangle -= 360.0;
		glutPostRedisplay();
		break;
	case 'Z':
		Zangle -= 5.0;
		if (Zangle < 0.0) Zangle += 360.0;
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void specialKeyInput(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) if (step < 180) step++;;
	if (key == GLUT_KEY_DOWN) if (step > 0) step--;;
	glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Press the x, X, y, Y, z, Z keys to rotate the box." << std::endl
		<< "Press space to toggle between textures." << std::endl;
	std::cout << "Press up/down arrow keys to open/close the box." << std::endl;

}

// Main routine.
int main(int argc, char** argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("box headset");
	//initRendering();

	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);

	//glewExperimental = GL_TRUE;
	//glewInit();

	setup();

	glutMainLoop();
}