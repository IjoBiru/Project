///////////////////////////////////////////////////////////////////////////////////////////////////////
// texturedSquare.cpp
//
//
//
// Alya Hani Nurahma	  (5222600032)
// Haniefam Muslima Yahya (5222600060) 
//
// 
//
// Texture Credits: See ExperimenterSource/Textures/TEXTURE_CREDITS.txt
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <GL/glut.h>

#include "getBMP.h"



// Globals.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate torpedo.
static int isAnimate = 0; // In animation mode?
static int animationPeriod = 1; // Speed of animation.
static unsigned int texture[2]; // Array of texture ids.
static unsigned char chessboard[64][64][4]; // Storage for chessboard image.
static float angle = 0.0; // Angle to rotate textured square.
static int id = 0; // Currently displayed texture id.

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
}

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

// Create 64 x 64 RGBA image of a chessboard.
void createChessboard(void)
{
	int i, j;
	for (i = 0; i < 64; i++)
		for (j = 0; j < 64; j++)
			if ((((i / 8) % 2) && ((j / 8) % 2)) || (!((i / 8) % 2) && !((j / 8) % 2)))
			{
				chessboard[i][j][0] = 0x00;
				chessboard[i][j][1] = 0x00;
				chessboard[i][j][2] = 0x00;
				chessboard[i][j][3] = 0xFF;
			}
			else
			{
				chessboard[i][j][0] = 0xDD;
				chessboard[i][j][1] = 0xDD;
				chessboard[i][j][2] = 0xDD;
				chessboard[i][j][3] = 0xFF;
			}
}

// Routine to load a program-generated image as a texture. 
void loadChessboardTexture()
{
	// Generate internal texture.
	createChessboard();

	// Create texture object texture[1]. 
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	// Specify image data for currently active texture object.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, chessboard);

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
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	// Create texture ids.
	glGenTextures(2, texture);

	// Load external texture. 
	loadTextures();

	// Load internal texture.
	loadChessboardTexture();

	// Specify how texture values combine with current surface color values.
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	
	// Turn on OpenGL texturing.
	glEnable(GL_TEXTURE_2D);

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST); // Enable depth testing.

	//Turn on lighting
	glEnable(GL_LIGHTING);

	// Light property vectors.
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float lightPos[] = { 0.0, 1.5, 3.0, 1.0 };
	float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };

	// Light properties.
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glEnable(GL_LIGHT0); // Enable particular light source.
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // Enable two-sided lighting.
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.

	// Cull back faces.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void drawBox(void)
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glPushMatrix();
	glScalef(13.3f, 13.3f, 6.6f);  // Atur skala lemari meja (panjang, lebar, tinggi)
	glTranslatef(0.0f, 0.0f, 0.0f);  // Geser ke bawah
	glutSolidCube(1.5);  // Gambar kubus solid
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture[id]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);

	// Sisi depan
	glTexCoord2f(0.0, 0.0); glVertex3f(-10.0, -10.0, 5.0);   // Kiri Bawah
	glTexCoord2f(1.0, 0.0); glVertex3f(10.0, -10.0, 5.0);    // Kanan Bawah
	glTexCoord2f(1.0, 1.0); glVertex3f(10.0, 10.0, 5.0);     // Kanan Atas
	glTexCoord2f(0.0, 1.0); glVertex3f(-10.0, 10.0, 5.0);    // Kiri Atas

	glDisable(GL_TEXTURE_2D);

	glEnd();
	
}

// Drawing routine.
void drawScene(void)
{
	
	glEnable(GL_LIGHTING); // Aktifkan pencahayaan
	glEnable(GL_LIGHT0); // Aktifkan sumber cahaya

	// Setup sifat material
	float lightPos[] = { 0.0, 2.0, 2.5, 1.0 };
	float matAmbAndDif[] = { 0.0, 0.0, 0.0, 0.0 }; // Atur sifat ambien dan difus
	float matSpec[] = { 10.0, 10.0, 10.0, 10.0 }; // Atur sifat specular
	float matShine[] = { 100.0 }; // Atur shininess

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

	glRotatef(Zangle, 0.0, 0.0, 1.0);
	glRotatef(Yangle, 0.0, 1.0, 0.0);
	glRotatef(Xangle, 1.0, 0.0, 0.0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glEnable(GL_TEXTURE_2D);

	// Activate texture object.
	glBindTexture(GL_TEXTURE_2D, texture[id]);

	glPushMatrix();
	drawBox();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glutSwapBuffers();
}

void animate(int value)
{
	if (isAnimate)
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
	glLoadIdentity();
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


// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Press the x, X, y, Y, z, Z keys to rotate the box." << std::endl
		<< "Press space to toggle between textures." << std::endl
		<< "Press delete to reset." << std::endl;
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
	initRendering();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}