/*	
Project2 - Morgan Ludtke & Faiz Lurman
-Displays an environment based around stools and other objects

Taken heavily from Perry Kivolowitz's Modern Hello World
specifically main.cpp. Cheers!
*/

#include <iostream>
#include <cassert>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "background.h"
#include "stool.h"
#include "Sphere.h"
#include "world.h"
#include "cylinder.h"

using namespace std;
using namespace glm;

class Window
{
public:
	Window()
	{
		this->time_last_pause_began = this->total_time_paused = 0;
		this->normals = this->wireframe = this->paused = false;
		this->slices = 20;
		this->interval = 1000 / 120;
		this->handle = -1;
		this->stacks = 20;
		this->shader = 0;
		this->title  = "Behold... Project 2!";
	}

	GLfloat time_last_pause_began;
	GLfloat total_time_paused;
	GLboolean paused , wireframe, normals;
	GLint handle;
	GLint interval;
	GLint FPS;
	GLint slices;
	GLint stacks;
	GLint shader;
	string title;
	ivec2 size;
	GLfloat aspect;
	vector<string> instructions;
} window;

Background background;
Stool stool1, stool2;
Sphere sphere1, sphere2, sphere3, sphere4;
World world;
cylinder Cylinder;

static GLfloat xrot = 0.0f, yrot = 0.0f; //used for mouse movement
static GLfloat xdiff = 0.0f, ydiff = 0.0f;	//used for mouse movement
GLfloat radius, xpos, ypos, zpos;	//used for camera movement
GLfloat angleH, angleV;	//ditto
float PI = 3.1415926f;



int debug_mode = 1;	//keeps track of what debug mode it is in

void DisplayInstructions()
{
	if (window.handle == -1)
		return;

	vector<string> * s = &window.instructions;
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glColor3f(1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window.size.x, 0, window.size.y, 1, 10);
	glViewport(0, 0, window.size.x, window.size.y);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(10, 15 * s->size(), -5.5);
	glScaled(0.1, 0.1, 1.0);
	for (auto i = s->begin(); i < s->end(); ++i)
	{
		glPushMatrix();
		glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char *) (*i).c_str());
		glPopMatrix();
		glTranslated(0, -150, 0);
	}
}

void CloseFunc()	//Makes sure everything is deleted when the window is closed
{
	window.handle = -1;
	background.TakeDown();
	stool1.TakeDown();
	stool2.TakeDown();
	sphere1.TakeDown();
	sphere2.TakeDown();
	sphere3.TakeDown();
	sphere4.TakeDown();
	world.TakeDown();
	Cylinder.TakeDown();
}

void ReshapeFunc(int w, int h)
{
	if (h > 0)
	{
		window.size = ivec2(w, h);
		window.aspect = float(w) / float(h);
	}
}

void KeyboardFunc(unsigned char c, GLint x, GLint y)
{
	float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	switch (c)
	{
	case 'n':
		//stool1.EnableNormals(window.normals);
		//stool2.EnableNormals(window.normals);
		sphere1.EnableNormals(window.normals = !window.normals);
		break;

	case 'w':
		window.wireframe = !window.wireframe;
		break;

	case 'p':
		if (window.paused)
		{
			// Will be leaving paused state
			window.total_time_paused += (current_time - window.time_last_pause_began);
		}
		else
		{
			// Will be entering paused state
			window.time_last_pause_began = current_time;
		}
		window.paused = !window.paused;
		break;

	case 's':
		window.shader++;
		if(window.shader > 3)
			window.shader = 0;
		cout << window.shader;
		sphere1.TakeDown();
		sphere1.Initialize(window.slices, window.stacks, window.shader);
		stool1.TakeDown();
		stool1.Initialize(window.slices, window.stacks, window.shader);
		stool2.TakeDown();
		stool2.Initialize(window.slices, window.stacks, window.shader);
		break;
	case 'v':
		PI = 2 * PI;
		if(PI > 6.29)
			PI = 3.1415926f;
		break;
	case 'x':
	case 27:
		glutLeaveMainLoop();
		return;
	}
}

void SpecialFunc(GLint key, GLint xPt, GLint yPt)
{
	switch (key)
	{
	case GLUT_KEY_PAGE_UP:	//increases the number of vertices for all objects (slices and stacks)
							//makes sure the objects are reinitialized 
		++window.slices;
		++window.stacks;
		stool1.TakeDown();
		stool1.Initialize(window.slices, window.stacks, window.shader);
		stool2.TakeDown();
		stool2.Initialize(window.slices, window.stacks, window.shader);
		sphere1.TakeDown();
		sphere1.Initialize(window.slices, window.stacks, window.shader);
		Cylinder.TakeDown();
		Cylinder.Initialize(window.slices, window.stacks, window.shader);
		break;

	case GLUT_KEY_PAGE_DOWN:	//decreases the number of vertices for all objects
		if (window.slices > 1)
		{
			--window.slices;
			window.slices = clamp(window.slices, 2, 50);
			--window.stacks;
			window.stacks = clamp(window.stacks, 2, 50);
			if (window.stacks < 1)
				window.stacks = 1;
			stool1.TakeDown();
			stool1.Initialize(window.slices, window.stacks, window.shader);
			stool2.TakeDown();
			stool2.Initialize(window.slices, window.stacks, window.shader);
			sphere1.TakeDown();
			sphere1.Initialize(window.slices, window.stacks, window.shader);
			Cylinder.TakeDown();
			Cylinder.Initialize(window.slices, window.stacks, window.shader);

		}
		break;

	case GLUT_KEY_F1:	//changes between debug modes and final view
		debug_mode++;
		if(debug_mode > 3)
			debug_mode = 0;
		break;

	case GLUT_KEY_UP: 
		if (angleH < 89) 
		{
			angleH = angleH + 1.0f;	
			ypos = sin(angleH * (PI / 180)) * radius;
			zpos = -  cos(angleH * (PI / 180)) * -cos(angleV * (PI /180)) * radius; 
			xpos =  -cos(angleH * PI / 180) * -sin(angleV * PI /180) * radius;
		}
		break;

	case GLUT_KEY_DOWN:
		if (angleH > -89)
		{
			angleH = angleH - 1.0f;
			ypos = sin(angleH * (PI/ 180)) * radius;
			zpos = -cos(angleH * (PI / 180)) * -cos(angleV * (PI/180)) * radius;
			xpos = -cos(angleH * (PI / 180)) * -sin(angleV * (PI / 180)) * radius;
		}
		break;

	case GLUT_KEY_RIGHT: 
		angleV = angleV + 1.0f;
		ypos = ypos;
		xpos = sin(angleV * (PI / 180)) * cos(angleH * (PI / 180)) * radius;
		zpos = cos(angleV * (PI / 180)) * cos(angleH * (PI / 180)) * radius;
		break;
	case GLUT_KEY_LEFT:
		angleV = angleV - 1.0f;
		ypos = ypos;
		xpos = sin(angleV * (PI / 180)) * cos(angleH * (PI / 180)) * radius;
		zpos = cos(angleV * (PI / 180)) * cos(angleH * (PI / 180)) * radius;
		break;
	}
	return;

}

void DisplayFunc()
{
	float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	glEnable(GL_CULL_FACE);

	glClearColor(0.0, 0.5, 0.5, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window.size.x, window.size.y);

	if(debug_mode == 0)
		background.Draw(window.size);
	mat4 projection = perspective(25.0f, window.aspect, 1.0f, 100.0f);

	radius = 20.0f;

	mat4 modelview = lookAt(vec3(xpos, ypos, zpos), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	glPolygonMode(GL_FRONT_AND_BACK, window.wireframe ? GL_LINE : GL_FILL);
	modelview = rotate(modelview, -90.0f, vec3(1.0f, 0.0f, 0.0f));
	modelview = translate(modelview, vec3(0.0f, -2.0f, 0.0f));
	modelview = rotate(modelview, xrot, vec3(1.0f, 0.0f, 0.0f));
	modelview = rotate(modelview, yrot, vec3(0.0f, 1.0f, 0.0f));
	world.Draw(projection, modelview, window.size, xrot, yrot, window.shader, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
	modelview = rotate(modelview, 90.0f, vec3(1.0f, 0.0f, 0.0f));
	modelview = translate(modelview, vec3(0.0f, 2.0f, 0.0f));
	//Draws objects in specific debug modes
	if(debug_mode == 2)
		sphere1.Draw(projection, modelview, window.size, xrot, yrot, window.shader, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
	if(debug_mode == 3)
		Cylinder.Draw(projection, modelview, window.size, xrot, yrot, window.shader, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
	if (debug_mode < 2)
		stool1.Draw(projection, modelview, window.size, xrot, yrot, window.shader, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
	modelview = translate(modelview, vec3(2.0f, 0.0f, 0.0f));

	if(debug_mode == 0)
	{
		stool2.Draw(projection, modelview, window.size, xrot, yrot, window.shader, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		vec3 location;
		modelview = translate(modelview, vec3(-4.0f, 0.0f, 0.0f));
		stool2.Draw(projection, modelview, window.size, xrot, yrot, window.shader, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
		modelview =  translate(modelview, vec3(2.0f, 0.0f, 0.0f));


		float x = 0.0f, y = 0.0f, z = 0.0f;
		float theta = PI, phi = 0.0f;
		float slices = 20.0f;
		float stacks = 12.0f;

		float increment_slices = (PI) / float(slices);
		float increment_stacks = (PI) / float(stacks);
		float sphere_radius = 6.0f;
		//draws a sphere of spheres
		//uses the same formula as the sphere class
		for(int i = 0; i < slices; ++i) 								
		{
			for(int j = 0; j < stacks; ++j)
			{
				modelview = translate(modelview, -location);
				x = sphere_radius * cos(theta) * sin(phi);
				y = sphere_radius * sin(theta) * sin(phi);
				z = sphere_radius * cos(phi);
				location = vec3(x, y, z);
				modelview = translate(modelview, location);
				sphere1.Draw(projection, modelview, window.size, xrot, yrot, window.shader, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
				theta += increment_stacks;
			}
			theta = PI;
			phi += increment_slices;
		}
		Cylinder.Draw(projection, modelview, window.size, xrot, yrot, window.shader, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	DisplayInstructions();
	glFlush();
	glutPostRedisplay(); // FPS
}

GLvoid TimerFunc(GLint value)	//keeps track of time to be displayed on window
{
	if (0 != value) // first time timer runs
	{
		ostringstream title, FPS, dimension, elapsedTime;
		title     << window.title << " : ";
		FPS       << (window.FPS *= 4) << " Frames Per Second ";
		dimension << "@ " << window.size.x << " x " << window.size.y;
		window.FPS = 0;
		elapsedTime.precision(0);
		elapsedTime << fixed;
		elapsedTime << " Elapsed Time: " << GLfloat(glutGet(GLUT_ELAPSED_TIME)) / 1E3 << "s";
		string windowTitle = title.str() + FPS.str() + dimension.str() + elapsedTime.str();
		glutSetWindowTitle(windowTitle.c_str());
	}
	glutTimerFunc(250, TimerFunc, 1); // 250ms = 1/4 of a second
}

GLvoid MouseFunc(GLint button, GLint state, GLint xPt, GLint yPt)
{
	xdiff = xPt - yrot;
	ydiff = -yPt + xrot;
}

GLvoid MotionFunc(GLint xPt, GLint yPt)
{
	yrot = xPt - xdiff;
	xrot = yPt + ydiff;
}

GLvoid passiveMotionFunc(GLint x, GLint y)
{
	//asdasdsa
}

GLint main(GLint argc, GLchar * argv[])
{
	radius = 20.0f;
	xpos = 0.0f;
	ypos = 0.0f;
	zpos = radius;
	angleH = 0.0f;
	angleV = 0.0f;

	glutInit(&argc, argv);
	glutInitWindowSize(1096, 1024);
	glutInitWindowPosition(500, 100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);

	window.handle = glutCreateWindow(window.title.c_str());
	glutReshapeFunc(ReshapeFunc);
	glutCloseFunc(CloseFunc); // our takedown actions
	glutDisplayFunc(DisplayFunc);

	glutMouseFunc(MouseFunc);
	glutMotionFunc(MotionFunc); 
	glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);

	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutTimerFunc(window.interval, TimerFunc, 0);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION); // ensure complete takedown of resources

	//Writes words to the screen (instructions
	window.instructions.push_back("Program 2: Ikea chair with creepy sphere of spheres");
	window.instructions.push_back("using modern OpenGL.");
	window.instructions.push_back("");
	window.instructions.push_back("Morgan Ludtke & Faiz Lurman - CS 559");
	window.instructions.push_back("");
	window.instructions.push_back("Page UP /page DN - changes slice count");
	window.instructions.push_back("Arrow keys - moves camera");
	window.instructions.push_back("Mouse - moves around the world");
	window.instructions.push_back("n - toggles normals");
	window.instructions.push_back("s - switches between shaders: ");
	window.instructions.push_back("v - toggles sphere vs half sphere");
	window.instructions.push_back("w - toggles wireframe");
	window.instructions.push_back("x - exits");

	assert(GLEW_OK == glewInit());	

	//initialize all objects, and returns error if failed
	assert(background.Initialize());		
	assert(stool1.Initialize(window.slices, window.stacks, window.shader));		
	assert(stool2.Initialize(window.slices, window.stacks, window.shader));
	assert(sphere1.Initialize(window.slices, window.stacks, window.shader));
	assert(sphere2.Initialize(window.slices, window.stacks, window.shader));
	assert(sphere3.Initialize(window.slices, window.stacks, window.shader));
	assert(sphere4.Initialize(window.slices, window.stacks, window.shader));		
	assert(world.Initialize(window.slices));		
	assert(Cylinder.Initialize(window.slices, window.stacks, window.shader));
	

	glutMainLoop();
}
