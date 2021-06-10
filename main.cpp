#include <math.h>
#include <stdlib.h>
#include <string>
#include <GL/glut.h>
#include "imageloader.h"
#include "glm.h"
#include <cmath>
#include <iostream>

#define PI 3.14

double eye[] = { 0, 7, 10 };
double center[] = { 0, 0, 2 };
double up[] = { 0, 1, 0 };


int rightshoulderZ = 0, rightshoulderY = 0, leftshoulderZ = 0, leftshoulderX = 0, rightelbow = 0 , leftelbow = 0, fingerBase = 0, fingerUp = 0;
int Right_FumerX = 0, Right_FumerZ = 0, Left_FumerX = 0, Left_FumerZ = 0, Right_Tibia = 0, Left_Tibia = 0;
int scale = 1;
int moving;
GLfloat startx;

// Body Positions
static float xBody = -3;
static float yBody = 0;
static float zBody = -2;

//ball position
float xBall = 6.3;
float yBall = -6;
float zBall = -1 ;

//sofa position
static float xsofa = -5 ;
static float ysofa = -4;
static float zsofa = -0.5;

//door position
static float xdoor = 9;
static float ydoor = 0;
static float zdoor = 2;

//doorframe position
static float xdoorframe = 9;
static float ydoorframe = 0;
static float zdoorframe = 2;

GLuint textureId;

GLfloat angle = 0.0;
int mainBody = 0.0;   /* in degrees */
//GLfloat angle2 = 0.0;   /* in degrees */

int rCounter = 0;
int leg_state = 1;
bool reverse = false;

int doorangle = 90;

// RGBA
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat light0_position[] = { 0, 0 , 2.0, 1.0 };

// x , y, z, w
GLfloat light_position[] = { 0.5, 5.0, 0.0, 1.0 };
GLfloat lightPos1[] = { -0.5, -5.0, -2.0, 1.0 };

void scene();
void turnRight();



class Model
{
private:
	char* path;
	GLMmodel* model;

public:
	//Model Constructor
	Model( char* path)
	{
		this->path = path;
		model = glmReadOBJ(path);
		glmUnitize(model);             //magic
		glmFacetNormals(model);        //more magic
		glmVertexNormals(model, 90.0); //other type of magi
	}
	
	void draw()

	{
		//render with vertex normal, texture and materials
		glmDraw(model, GLM_SMOOTH | GLM_MATERIAL);
	}

	void scale(float factor)
	{
		glmScale(model, factor);
	}
};

char sofaa[] = "data/cornerSofa.obj";
char balll[] = "data/soccerball.obj";
char doorr[] = "data/door.obj";
char doorframee[] = "data/doorFrame.obj";

Model sofa(sofaa);
Model ball(balll);
Model door(doorr);
Model doorframe(doorframee);

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(85.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
}

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image)
{

	glGenTextures(1, &textureId);			 //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,				  //Always GL_TEXTURE_2D
		0,							  //0 for now
		GL_RGB,					  //Format OpenGL uses for image
		image->width, image->height, //Width and height
		0,							  //The border of the image
		GL_RGB,					  //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE,			  //GL_UNSIGNED_BYTE, because pixels are stored
									 //as unsigned numbers
		image->pixels);			  //The actual pixel data
	return textureId;						  //Returns the id of the texture
}


//GLuint textureId; //The id of the texture

//Initializes 3D rendering
void initRendering(const char* floorname, GLuint textureId)
{
	glClearColor(0.0, 0.0, 1.0, 0.0);
	glShadeModel(GL_FLAT);


	Image* image = loadBMP(floorname);
	textureId = loadTexture(image);
	delete image;
	// Turn on the power
	glEnable(GL_LIGHTING);
	// Flip light switch
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	// assign light parameters
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	// Material Properties
	GLfloat lightColor1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
	glEnable(GL_NORMALIZE);
	//Enable smooth shading
	glShadeModel(GL_SMOOTH);
	// Enable Depth buffer
	glEnable(GL_DEPTH_TEST);
}




void displayr(void)
{
	glClearColor(0.502, 0.502, 0.502, 1.0);

	// Clear Depth and Color buffers

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		up[0], up[1], up[2]);
	glColor3f(1.0, 1.0, 1.0);

	
	scene();


	glutSwapBuffers();
}

void scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--------------------------------
	// 
	// 	   start total body
	glPushMatrix(); // body start
	//glRotatef(angle2, 1.0, 0.0, 0.0);
	//glRotatef(mainBody, 0.0, 1.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);



	//----Body----
	glPushMatrix();
	glTranslatef(xBody, yBody, zBody);


	glPushMatrix();
	glScalef(scale * 2.0, scale * 4.0, scale * 1.0);
	glutWireCube(1.0);
	glPopMatrix();

	//----Head----
	glPushMatrix();
	glTranslatef(0, 3.0, 0.0);
	glTranslatef(0.0, 0.0, 0.0);

	glScalef(scale * 0.25, scale * 0.25, scale * 0.25);
	glutWireSphere(3.0, 20, 20); // double radius , slices , stacks
	glPopMatrix();

	//----Right Arm----

	glPushMatrix();
	//----Right shoulder----
	glTranslatef(-1.2, 1.7, 0.0);
	glRotatef((GLfloat)rightshoulderZ, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)rightshoulderY, 0.0, 1.0, 0.0);

	glTranslatef(-0.1, -0.5, 0.0);

	glPushMatrix();
	glScalef(scale * 0.5, scale * 1.5, scale * 1.0);
	glutWireCube(1.0);
	glPopMatrix();
	//----Right elbow----
	glPushMatrix();
	glTranslatef(0.0, -0.75, 0.0);
	glRotatef((GLfloat)rightelbow, 0.0, 0.0, 1.0);

	glTranslatef(0.0, -0.75, 0.0);
	glPushMatrix();
	glScalef(scale * 0.5, scale * 1.5, scale * 1.0);
	glutWireCube(1.0);
	glPopMatrix();


	// ----Right 4 fingers----
	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glTranslatef(-0.2, -0.8, 0.45 - 0.3 * i);
		glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
		glTranslatef(0, -0.1, 0);

		glPushMatrix();
		glScalef(scale * 0.1, scale * 0.3, scale * 0.1);
		glutWireCube(1);
		glPopMatrix();

		glTranslatef(0, -0.2, 0);
		glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
		glTranslatef(0, -0.1, 0);

		glPushMatrix();
		glScalef(scale * 0.1, scale * 0.3, scale * 0.1);
		glutWireCube(1);
		glPopMatrix();

		glPopMatrix();
	}
	// ----Right Thumb----
	glPushMatrix();

	glTranslatef(0.2, -0.8, 0.0);
	glRotatef((GLfloat)fingerBase, 0.0, 0.0, -1.0);
	glTranslatef(0, -0.1, 0);

	glPushMatrix();
	glScalef(scale * 0.1, scale * 0.3, scale * 0.1);
	glutWireCube(1);
	glPopMatrix();

	glTranslatef(0, -0.2, 0);
	glRotatef((GLfloat)fingerUp, 0.0, 0.0, -1.0);
	glTranslatef(0, -0.1, 0);

	glPushMatrix();
	glScalef(scale * 0.1, scale * 0.3, scale * 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();

	glPopMatrix();

	//----Right Leg----
	glPushMatrix();

	glTranslatef(-0.72, -2.0, 0.0);
	glRotatef((GLfloat)Right_FumerX, -1.0, 0.0, 0.0);
	glRotatef((GLfloat)Right_FumerZ, 0, 0.0, 1.0);
	glTranslatef(0.0, -1.0, 0.0);


	glPushMatrix();
	glScalef(scale * 0.6, scale * 2.0, scale * 1.0);
	glutWireCube(1.0);
	glPopMatrix();

	//----Right Tibia----

	glPushMatrix();
	glTranslatef(0.0, -1.0, 0.0);
	glRotatef((GLfloat)Right_Tibia, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -1.0, 0.0);

	glPushMatrix();
	glScalef(scale * 0.6, scale * 2.0, scale * 1.0);
	glutWireCube(1.0);
	glPopMatrix();


	// ----Right Foot----
	glPushMatrix();

	glTranslatef(-0.23, -1.6, 0.0);
	glTranslatef(0.2, 0.3, 0);

	glPushMatrix();
	glScalef(scale * 0.6, scale * 0.5, scale * 2.0);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

	//----Left Arm----
	glPushMatrix();
	//----Left shoulder----
	glTranslatef(1.2, 1.7, 0.0);

	glRotatef((GLfloat)leftshoulderZ, 0.0, 0.0, -1.0);
	//glRotatef((GLfloat)leftshoulderX, 0.0, -1.0, 0.0);////
	glRotatef((GLfloat)leftshoulderX, 1.0, 0.0, 0.0);


	glTranslatef(0.1, -0.5, 0.0);

	glPushMatrix();
	glScalef(scale * 0.5, scale * 1.5, scale * 1.0);
	glutWireCube(1.0);
	glPopMatrix();
	//----Left elbow----
	glPushMatrix();
	glTranslatef(0.0, -0.75, 0.0);

	glRotatef((GLfloat)leftelbow, 0.0, 0.0, -1.0);

	glTranslatef(0.0, -0.75, 0.0);
	glPushMatrix();
	glScalef(scale * 0.5, scale * 1.5, scale * -1.0);
	glutWireCube(1.0);
	glPopMatrix();


	// ----Left 4 fingers----
	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glTranslatef(0.2, -0.8, 0.45 - 0.3 * i);
		glRotatef((GLfloat)fingerBase, 0.0, 0.0, -1.0);
		glTranslatef(0, -0.1, 0);

		glPushMatrix();
		glScalef(scale * 0.1, scale * 0.3, scale * 0.1);
		glutWireCube(1);
		glPopMatrix();

		glTranslatef(0, -0.2, 0);
		glRotatef((GLfloat)fingerUp, 0.0, 0.0, -1.0);
		glTranslatef(0, -0.1, 0);

		glPushMatrix();
		glScalef(scale * 0.1, scale * 0.3, scale * 0.1);
		glutWireCube(1);
		glPopMatrix();

		glPopMatrix();
	}
	// ----Left Thumb----
	glPushMatrix();

	glTranslatef(-0.2, -0.8, 0.0);
	glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0, -0.1, 0);

	glPushMatrix();
	glScalef(scale * 0.1, scale * 0.3, scale * 0.1);
	glutWireCube(1);
	glPopMatrix();

	glTranslatef(0, -0.2, 0);
	glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0, -0.1, 0);

	glPushMatrix();
	glScalef(scale * 0.1, scale * 0.3, scale * 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

	//----Left Leg----	

	glPushMatrix();

	glTranslatef(0.72, -2.0, 0.0);
	glRotatef((GLfloat)Left_FumerX, -1.0, 0.0, 0.0);
	glRotatef((GLfloat)Left_FumerZ, 0, 0.0, -1.0);
	glTranslatef(0.0, -1.0, 0.0);


	glPushMatrix();
	glScalef(scale * 0.6, scale * 2.0, scale * 1.0);
	glutWireCube(1.0);
	glPopMatrix();
	//----Left Tibia----
	glPushMatrix();
	glTranslatef(0.0, -1.0, 0.0);
	glRotatef((GLfloat)Left_Tibia, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -1.0, 0.0);

	glPushMatrix();
	glScalef(scale * 0.6, scale * 2.0, scale * 1.0);
	glutWireCube(1.0);
	glPopMatrix();


	// ----Left Foot----
	glPushMatrix();

	glTranslatef(0, -1.6, 0.0);
	glTranslatef(0.0, 0.3, 0);

	glPushMatrix();
	glScalef(scale * 0.6, scale * 0.5, scale * 2.0);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

	glPopMatrix();

	glPopMatrix(); // end body


	// draw floor

	glPushMatrix();
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	//glTranslatef(0, -2, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-10, -6.5, 10);
	glTexCoord2f(3.0f, 0.0f); glVertex3f(10, -6.5, 10);
	glTexCoord2f(3.0f, 3.0f); glVertex3f(10, -6.5, -10);
	glTexCoord2f(0.0f, 3.0f); glVertex3f(-10, -6.5, -10);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();



	// Draw ball
	glPushMatrix();
	glTranslatef(xBall, yBall, zBall);
	ball.draw();
	glPopMatrix();



	// Draw sofa
	glPushMatrix();
	glTranslatef(xsofa, ysofa, zsofa);
	glTranslatef(-1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glTranslatef(1.0, 0.0, 0.0);
	sofa.draw();
	glPopMatrix();

	// Draw door
	glPushMatrix();
	glTranslatef(xdoor, ydoor, zdoor);
	glTranslatef(-1.0, 0.0, 0.0);
	glRotatef(doorangle, 0.0, 1.0, 0.0);
	glTranslatef(1.0, 0.0, 0.0);
	door.draw();
	glPopMatrix();

	// Draw doorframe
	glPushMatrix();
	glTranslatef(xdoorframe, ydoorframe, zdoorframe);
	glTranslatef(-1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glTranslatef(1.0, 0.0, 0.0);
	doorframe.draw();
	glPopMatrix();


	glPopMatrix();
}

void rotatePoint(double a[], double theta, double p[])
{

	double temp[3];
	temp[0] = p[0];
	temp[1] = p[1];
	temp[2] = p[2];

	temp[0] = -a[2] * p[1] + a[1] * p[2];
	temp[1] = a[2] * p[0] - a[0] * p[2];
	temp[2] = -a[1] * p[0] + a[0] * p[1];

	temp[0] *= sin(theta);
	temp[1] *= sin(theta);
	temp[2] *= sin(theta);

	temp[0] += (1 - cos(theta)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
	temp[1] += (1 - cos(theta)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
	temp[2] += (1 - cos(theta)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

	temp[0] += cos(theta) * p[0];
	temp[1] += cos(theta) * p[1];
	temp[2] += cos(theta) * p[2];

	p[0] = temp[0];
	p[1] = temp[1];
	p[2] = temp[2];
}

void crossProduct(double a[], double b[], double c[])
{
	c[0] = a[1] * b[2] - a[2] * b[1];
	c[1] = a[2] * b[0] - a[0] * b[2];
	c[2] = a[0] * b[1] - a[1] * b[0];
}


void normalize(double a[])
{
	double norm;
	norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
	norm = sqrt(norm);
	a[0] /= norm;
	a[1] /= norm;
	a[2] /= norm;
}

void turnLeft()
{
	double theta = -PI / 100;
	rotatePoint(up, theta, eye);
}

/*void turnRight()
{
	if (moving)
	{
		angle = (angle - startx) / 25.0;
		rotatePoint(up, angle, eye);
		startx = angle;
		glutPostRedisplay();
	}


	double theta = -PI / 100;
	//angle = angle + (PI*5 / 100);
}*/

void turnRight()
{
	double theta = PI / 100;
	rotatePoint(up, theta, eye);
}


void moveUp()
{
	double horizontal[3];
	double look[] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
	crossProduct(up, look, horizontal);
	normalize(horizontal);
	rotatePoint(horizontal, PI / 100, eye);
	rotatePoint(horizontal, PI / 100, up);
}


void moveDown()
{

	double horizontal[3];
	double look[] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
	crossProduct(up, look, horizontal);
	normalize(horizontal);
	rotatePoint(horizontal, -PI / 100, eye);
	rotatePoint(horizontal, -PI / 100, up);
}

void moveForward1()
{

	double speed = 0.1;
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
	normalize(direction);

	eye[0] += direction[0] * speed;
	eye[2] += direction[2] * speed;

	center[0] += direction[0] * speed;
	center[2] += direction[2] * speed;
}

void moveBack1()
{

	double speed = -0.1;
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];

	normalize(direction);

	eye[0] += direction[0] * speed;
	eye[2] += direction[2] * speed;

	center[0] += direction[0] * speed;
	center[2] += direction[2] * speed;
}

void sitdown()
{
	if (Left_FumerX < 30)
	{
		Left_FumerX = (Left_FumerX + 90) % 360;
		Right_FumerX = (Right_FumerX + 90) % 360;
		Right_Tibia = (Right_Tibia + 90) % 360;
		Left_Tibia = (Left_Tibia + 90) % 360;
		zBody = zBody - 2;
		yBody = yBody - 2;

	}
	
	glutPostRedisplay();

}

void standup()
{
	if (Left_FumerX >= 90)
	{
		Left_FumerX = (Left_FumerX - 90) % 360;
		Right_FumerX = (Right_FumerX - 90) % 360;
		Right_Tibia = (Right_Tibia - 90) % 360;
		Left_Tibia = (Left_Tibia - 90) % 360;
		zBody = zBody + 2;
		yBody = yBody + 2;

	}
	glutPostRedisplay();
}

void opendoor()
{
	if (leftshoulderX >= 30)
	{
		doorangle = (doorangle + 45) % 360;
	}
	glutPostRedisplay();

}


void moveForward()
{
	if (reverse == false)
	{
		Right_FumerX = (Right_FumerX + 1) % 360;
		Left_FumerX = (Left_FumerX - 1) % 360;
		zBody = zBody + 0.05;
		if (Right_FumerX == 10)
		{
			reverse = true;
		}
	}
	else
	{
		Right_FumerX = (Right_FumerX - 1) % 360;
		Left_FumerX = (Left_FumerX + 1) % 360;
		zBody = zBody + 0.05;
		if (Right_FumerX == -10)
		{
			reverse = false;
		}
	}
	glutPostRedisplay();

}

void moveleft()
{
	
	xBody = xBody + 0.05;
	
	glutPostRedisplay();
}


void moveBackword()
{
	if (reverse == false)
	{
		Right_FumerX = (Right_FumerX + 1) % 360;
		Left_FumerX = (Left_FumerX - 1) % 360;
		zBody = zBody - 0.05;
		if (Right_FumerX == 10)
		{
			reverse = true;
		}
	}
	else
	{
		Right_FumerX = (Right_FumerX - 1) % 360;
		Left_FumerX = (Left_FumerX + 1) % 360;
		zBody = zBody - 0.05;
		if (Right_FumerX == -10)
		{
			reverse = false;
		}
	}
	glutPostRedisplay();

}

void moveright()
{

	xBody = xBody - 0.05;
	glutPostRedisplay();

}

void specialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		turnLeft();
		break;
	case GLUT_KEY_RIGHT:
		turnRight();
		break;
	case GLUT_KEY_UP:
		if (center[1] <= 1.5)
			moveUp();
		break;
	case GLUT_KEY_DOWN:
		if (center[1] >= -1.5)
			moveDown();
		break;
	}
	glutPostRedisplay();
}



void Keyboard(unsigned char Key, int x, int y)
{
	switch (Key)
	{
	case 'p':
		moveleft();

		break;
	case 'b':
		//moveBack(0);
		moveright();

		break;

	case '1':
		moveForward();
		break;
	case '2':
		moveBackword();
		break;

	case 'n':
		moveForward1();
		break;
	case 'm':
		moveBack1();
		break;

	case '8':
		if (center[1] <= 1.5)
			moveUp();
		break;

	case '5':
		//if (center[1] <= 1.5)
			sitdown();
		break;

	case '6':
		//if (center[1] <= 1.5)
		standup();
		break;


	case '7':
		if (center[1] >= -1.5)
			moveDown();
		break;

	case 'Q':
		if (rightshoulderZ == 0)
			break;
		rightshoulderZ = (rightshoulderZ + 5) % 360;
		glutPostRedisplay();
		break;
	case 'q':
		if (rightshoulderZ == -145)
			break;
		rightshoulderZ = (rightshoulderZ - 5) % 360;
		glutPostRedisplay();
		break;

	case 'H':
		if (leftshoulderZ == 0)
			break;
		leftshoulderZ = (leftshoulderZ + 5) % 360;
		glutPostRedisplay();
		break;
	case 'h':
		if (leftshoulderZ == -145)
			break;
		leftshoulderZ = (leftshoulderZ - 5) % 360;
		glutPostRedisplay();
		break;
	case 'W':
		if (rightshoulderY == 0)
			break;
		rightshoulderY = (rightshoulderY + 5) % 360;
		glutPostRedisplay();
		break;
	case 'w':
		if (rightshoulderY == -90)
			break;
		rightshoulderY = (rightshoulderY - 5) % 360;
		glutPostRedisplay();
		break;

	case 'T':
		if (leftshoulderX == 0)
			break;
		leftshoulderX = (leftshoulderX + 5) % 360;
		glutPostRedisplay();
		break;
	case 't':
		if (leftshoulderX == -90)
			break;
		leftshoulderX = (leftshoulderX - 5) % 360;
		glutPostRedisplay();
		break;

	case 'e':
		if (rightelbow == 120)
			break;
		rightelbow = (rightelbow + 5) % 360;
		glutPostRedisplay();
		break;
	case 'E':
		if (rightelbow == 0)
			break;
		rightelbow = (rightelbow - 5) % 360;
		glutPostRedisplay();
		break;

	case 'r':
		if (leftelbow == 120)
			break;
		leftelbow = (leftelbow + 5) % 360;
		glutPostRedisplay();
		break;
	case 'R':
		if (leftelbow == 0)
			break;
		leftelbow = (leftelbow - 5) % 360;
		glutPostRedisplay();
		break;

	case 'f':
		if (fingerBase == 60)
			break;
		fingerBase = (fingerBase + 5) % 360;
		glutPostRedisplay();
		break;
	case 'F':
		if (fingerBase == 0)
			break;
		fingerBase = (fingerBase - 5) % 360;
		glutPostRedisplay();
		break;
	case 'g':
		if (fingerUp == 60)
			break;
		fingerUp = (fingerUp + 5) % 360;
		glutPostRedisplay();
		break;
	case 'G':
		if (fingerUp == 0)
			break;
		fingerUp = (fingerUp - 5) % 360;
		glutPostRedisplay();
		break;

	case 'a':
		if (Right_FumerX == 100)
			break;
		Right_FumerX = (Right_FumerX + 5) % 360;
		glutPostRedisplay();
		break;
	case 'A':
		if (Right_FumerX == -45)
			break;
		Right_FumerX = (Right_FumerX - 5) % 360;
		glutPostRedisplay();
		break;
	case 'S':
		if (Right_FumerZ == 0)
			break;
		Right_FumerZ = (Right_FumerZ + 5) % 360;
		glutPostRedisplay();
		break;
	case 's':
		if (Right_FumerZ == -100)
			break;
		Right_FumerZ = (Right_FumerZ - 5) % 360;
		glutPostRedisplay();
		break;
	case 'd':
		if (Right_Tibia == 120)
			break;
		Right_Tibia = (Right_Tibia + 5) % 360;
		glutPostRedisplay();
		break;
	case 'D':
		if (Right_Tibia == 0)
			break;
		Right_Tibia = (Right_Tibia - 5) % 360;
		glutPostRedisplay();
		break;
	case 'z':
		if (Left_FumerX == 100)
			break;
		Left_FumerX = (Left_FumerX + 5) % 360;
		glutPostRedisplay();
		break;
	case 'Z':
		if (Left_FumerX == -45)
			break;
		Left_FumerX = (Left_FumerX - 5) % 360;
		glutPostRedisplay();
		break;
	case 'X':
		if (Left_FumerZ == 0)
			break;
		Left_FumerZ = (Left_FumerZ + 5) % 360;
		glutPostRedisplay();
		break;
	case 'x':
		if (Left_FumerZ == -100)
			break;
		Left_FumerZ = (Left_FumerZ - 5) % 360;
		glutPostRedisplay();
		break;
	case 'c':
		if (Left_Tibia == 120)
			break;
		Left_Tibia = (Left_Tibia + 5) % 360;
		glutPostRedisplay();
		break;
	case 'C':
		if (Left_Tibia == 0)
			break;
		Left_Tibia = (Left_Tibia - 5) % 360;
		glutPostRedisplay();
		break;

	case 27:
		exit(0);
		break;

	default:
		break;
	}
	glutPostRedisplay();
}


void reset()
{
	double e[] = { 0.0, 0.0, 1.0 };
	double c[] = { 0.0, 0.0, 0.0 };
	double u[] = { 0.0, 1.0, 0.0 };
	for (int i = 0; i < 3; i++)
	{
		eye[i] = e[i];
		center[i] = c[i];
		up[i] = u[i];
	}
}

void screen_menu(int value)
{

	switch (value)
	{
	case '1':
		initRendering("images/Floor1.bmp", textureId);

		break;
	case '2':
		initRendering("images/Floor2.bmp", textureId);

		break;
	case '3':
		initRendering("images/Floor3.bmp", textureId);

		break;

	}
	//reset();
	glutPostRedisplay();
}



void attachMenu()
{
	glutCreateMenu(screen_menu);
	glutAddMenuEntry("Floor1", '1');
	glutAddMenuEntry("Floor2", '2');
	glutAddMenuEntry("Floor3", '3');

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600); // width ,height
	glutInitWindowPosition(100, 100);
	glutCreateWindow("mix");
	initRendering("images/wood.bmp", textureId);
	glutDisplayFunc(displayr);
	sofa.scale(7);
	ball.scale(0.5);
	door.scale(6);
	doorframe.scale(6);

	glutReshapeFunc(reshape);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60, 1.0, 0.1, 10);
	attachMenu();
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(specialKeys);
	glutMainLoop();
	return 0;
}

