#include "Angel.h"
#include "GL/glut.h"
#include "GL/glew.h"
#include "vec.h"

#include <map>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <algorithm>
#include <math.h>
#include <ctime>
#include <vector>

using namespace Angel;

const GLfloat PI = 3.14169;
const int NUMBER_OF_TRIANGLES = 50;
const GLfloat TWICE_PI = 2.0f * PI;
GLfloat DT = 0.001f;

GLfloat dX = 0.0f;
GLfloat dY = 0.0f;

GLfloat gfPosX = 0.0;
GLfloat gfDeltaX = 0.01;

GLfloat mX;
GLfloat mY;

GLfloat ballSize = .08f;

vec4 pointsArray[500] = {
	vec4(0.0, 0.0, 0.0, 1.0)
};

vec4 mVec;



int Index = 0;
int numVertices = 0;
int g_Width = 640;
int g_Height = 360;
int frame = 0;
int frameCounter = 25;
int ballDir = 0;

GLint BufferID_ProjMat;

mat4 G_Proj_Mat;


struct Ball{
public:
	Ball(vec4 v, vec4 pos, float r) : v(v), pos(pos), r(r) {};

	vec4 v; // Velocity;
	vec4 pos; // Position
	float r; // Radius
	vec4 color; // Color
};

std::vector<Ball> balls;

enum Wall {WALL_LEFT, WALL_RIGHT, WALL_TOP, WALL_BOTTOM};

struct BallPair {
	Ball* ballOne;
	Ball* ballTwo;
};

struct BallWallPair {
	Ball* ball;
	Wall wall;
};

float random(float a, float b) {
	float rando = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = rando * diff;
	return a + r;
	return (float)((rand() % 3 + (-1)) / 10);
}

float getLength(const vec4& a, const vec4& b)
{ 
	return sqrt(pow(fabs(a.x - b.x), 2) + pow(fabs(a.y - b.y), 2) + pow(fabs(a.z - b.z), 2));
}

void createBall() {
	if (balls.size() == 0) {
		balls.push_back(Ball(vec4(random(-10.0, 10.0), random(-10.0, 10.0), 0.0, 1.0), vec4(random(-3.5, 3.5), random(-1.75, 1.75), 0.0, 1.0), ballSize));
	}
	Ball B = Ball(vec4(random(-10.0, 10.0), random(-10.0, 10.0), 0.0, 1.0), vec4(random(-3.5, 3.5), random(-1.9, 1.9), 0.0, 1.0), ballSize);
	bool found = false;
	while (!found) {
		for (int i = 0; i < balls.size(); i++) {
			float distL = getLength(B.pos, balls[i].pos);
			float r = B.r + balls[i].r;

			if (distL < r) {
				B = Ball(vec4(random(-10.0, 10.0), random(-10.0, 10.0), 0.0, 1.0), vec4(random(-3.5, 3.5), random(-1.9, 1.9), 0.0, 1.0), ballSize);
				i = -1;
			}
		}
		balls.push_back(B);
		found = true;
	}
}

void drawBalls() {
	for (int i = 0; i < balls.size(); i++) {
		glBegin(GL_TRIANGLE_FAN);
			glVertex4f(balls[i].pos.x, balls[i].pos.y, 0.0, 1.0);
			for (int j = 0; j <= NUMBER_OF_TRIANGLES; j++) {
				glVertex4f(
					balls[i].pos.x + (balls[i].r * cos(j * TWICE_PI / NUMBER_OF_TRIANGLES)), 
					balls[i].pos.y + (balls[i].r * sin(j * TWICE_PI / NUMBER_OF_TRIANGLES)), 0.0, 1.0);
			}
		glEnd();
	}
}

void ballCollision() {
	if (balls.size() >= 2) {
		for (int i = 0; i < balls.size(); i++) {
			for (int j = i + 1; j < balls.size(); j++) {
				float dist = getLength(balls[i].pos, balls[j].pos);
				float rad = balls[i].r + balls[j].r;
				if (dist < rad + .02) {
					vec4 iCenter = vec4(balls[i].pos.x, balls[i].pos.y, 0.0, 1.0);
					vec4 jCenter = vec4(balls[j].pos.x, balls[j].pos.y, 0.0, 1.0);
					vec4 n = iCenter - jCenter;
					vec4 N = normalize(n);

					float V1 = dot(balls[i].v, N);
					float V2 = dot(balls[j].v, N);

					float optomized = (2.0 * (V1 - V2)) / 2.0;

					vec4 newV1 = balls[i].v - optomized * N;
					vec4 newV2 = balls[j].v + optomized * N;

					balls[i].v = newV1;
					balls[j].v = newV2;
				}
			}
		}
	}
}

void animateBalls(float x, float y) {
	for (int i = 0; i < balls.size(); i++) {

		switch (ballDir) {
		case 0:
			//DT = .001f;
			break;
		case 1:
			balls[i].v = vec4(mX - balls[i].pos.x, mY - balls[i].pos.y, 0.0, 1.0);
			DT = (0.005 / getLength(balls[i].pos, vec4(mX, mY, 0.0, 1.0)));
			//ballCollision();
			break;
		case 2:
			balls[i].v = vec4(mX + balls[i].pos.x, mY + balls[i].pos.y, 0.0, 1.0);
			DT = (0.005 / getLength(balls[i].pos, vec4(mX, mY, 0.0, 1.0)));
			//ballCollision();
			break;
		}
		
		if (balls[i].pos.y >= 1.95) {
			balls[i].pos.y -= .0007;
			balls[i].v.y = -(balls[i].v.y);
		}

		if (balls[i].pos.y <= -1.95) {
			balls[i].pos.y += .007;
			balls[i].v.y = -(balls[i].v.y);
		}

		if (balls[i].pos.x >= 3.5) {
			balls[i].pos.x -= .007;
			balls[i].v.x = -(balls[i].v.x);
		}
		if (balls[i].pos.x <= -3.5) {
			balls[i].pos.x += .007;
			balls[i].v.x = -(balls[i].v.x);
		}

		if (!(balls[i].pos.y > 2.0) && !(balls[i].pos.y < -2.0) && !(balls[i].pos.x > 3.5) && !(balls[i].pos.x < -3.5)) {
			balls[i].pos += balls[i].v * DT;
			ballCollision();
		}
	}
}



void mouse(int button, int state, int x, int y) {
	mX = ((7.0*x) / g_Width - 3.5);
	mY = ((3.8 - (3.8*y) / g_Height) - 1.9);

	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		ballDir = 1;
	}

	if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) {
		ballDir = 2;
	}

	if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
		ballDir = 0;
	}

	if (state == GLUT_UP && button == GLUT_RIGHT_BUTTON) {
		ballDir = 0;
	}
}

void key(unsigned char key, int x, int y) {
	if (key == 'W' || key == 'w') {
		DT += .0001;
	}
	
	if (key == 'S' || key == 's') {
		DT -= .0001;
	}

	if (key == 'Q' || key == 'q') {
		exit(0);
	}


}


void init(void) {
	for (int i = 0; i < 20 ; i++) {
		createBall();
	}

	GLuint program = InitShader("vshader32.glsl", "fshader32.glsl");
	glUseProgram(program);


	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	
	glClearColor(0.7529, 0.7529, 0.7529, 1.0);

	BufferID_ProjMat = glGetUniformLocation(program, "ProjMat");
}


void reshape(int w, int h)
{
	g_Width = w; // Set global width
	g_Height = h; // Set global height

	glViewport(0, 0, w, h); // Set the viewport to the window mapping

	GLfloat xmin = -2.0, xmax = 2.0, ymin = -2.0, ymax = 2.0;
	GLfloat aspect = GLfloat(w) / h;

	if (aspect < 1.0) {
		ymin /= aspect;
		ymax /= aspect;
	}
	else {
		xmin *= aspect;
		xmax *= aspect;
	}

	// Let openGL create the matrix for us
	G_Proj_Mat = Ortho2D(xmin, xmax, ymin, ymax);

	// Ship our projection matrix down to the graphics processor.
	glUniformMatrix4fv(BufferID_ProjMat, 1, GL_FALSE, G_Proj_Mat);
}


void animate() {
	if (frame > frameCounter) {
		dX += .01;
		dY += .01;
		frame = 0;
	}
	frame++;
	glutPostRedisplay();
}


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	drawBalls();
	animateBalls(0, 0);
	glutSwapBuffers();	
}


void main(int argc, char **argv) {
	srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(g_Width, g_Height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Gravity");
	glewInit();
	glutFullScreen();
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(key);
	init();
	glutMainLoop();
}
