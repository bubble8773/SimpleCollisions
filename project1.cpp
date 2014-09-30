#include <string>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <glload/gl_3_3.h>
#include <GL/freeglut.h>
#include "../framework/framework.h"
#include <glm/glm.hpp>
// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
// glm::perspective
#include <glm/gtc/matrix_projection.hpp>
// glm::translate, glm::rotate, glm::scale
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>


#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

GLuint theProgram;

//Particle system;
#define INITIAL_NUM_PARTICLES 25
#define INITIAL_POINT_SIZE 3.0
#define INITIAL_SPEED 0.01
#define NUM_COLORS 8

#define TRUE 1
#define FALSE 0

glm::mat4 myRotationMatrix;
//glm::mat4 P;
GLuint myRotationMatrixUnif;

typedef struct particle
{ 
     int color;
     glm::vec4 position;
     glm::vec4 velocity;
     float mass;
} particle;
particle particles[INITIAL_NUM_PARTICLES]; 

int present_time;
int last_time;
//const int num_particles = INITIAL_NUM_PARTICLES;
float point_size = INITIAL_POINT_SIZE;
float speed = INITIAL_SPEED;
bool gravity = TRUE; /* gravity on*/
//bool gravity = FALSE ; /* gravity off*/
float coef = 1.0; /* perfectly elastic collisions */


glm::vec4 pcolors[INITIAL_NUM_PARTICLES];
glm::vec4 ppoints[INITIAL_NUM_PARTICLES]; 

glm::vec4 colors[8]={
	glm::vec4(0.5, 0.5, 0.5,1.0), glm::vec4(1.0, 0.0, 0.0,1.0),
	glm::vec4(1.0, 1.0, 0.0,1.0), glm::vec4(0.0, 1.0, 0.0,1.0),
	glm::vec4(0.0, 0.0, 1.0,1.0), glm::vec4(1.0, 0.0, 1.0,1.0),
	glm::vec4(0.0, 1.0, 1.0,1.0), glm::vec4(1.0, 1.0, 1.0,1.0)
};
void deinit(){}
void InitializeProgram()
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(Framework::LoadShader(GL_VERTEX_SHADER, "project1.vert"));
	shaderList.push_back(Framework::LoadShader(GL_GEOMETRY_SHADER, "project1.geom"));
	shaderList.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER, "project1.frag"));

	theProgram = Framework::CreateProgram(shaderList);
}

GLuint positionBufferObject;
GLuint buffers[2];
GLuint vao;

void InitializeVertexBuffer()
{
	glGenBuffers(2, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ppoints), ppoints, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pcolors), pcolors, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
float forces(int i, int j)
{
   float force = 0.0;
   if(!gravity) force = 0.0;
   else if(j==1) force = -1.0;
   return(force);
}

void collision(int n)/* tests for collisions against cube and reflect particles if necessary */
{
     int i;
     for (i=0; i<3; i++) 
     {
           if(particles[n].position[i]>=1.0) 
           {
                particles[n].velocity[i] = -coef*particles[n].velocity[i];
                particles[n].position[i] = 1.0-coef*(particles[n].position[i]-1.0f);
           }
           if(particles[n].position[i]<=-1.0) 
           {
                particles[n].velocity[i] = -coef*particles[n].velocity[i];
                particles[n].position[i] = -1.0-coef*(particles[n].position[i]+ 1.0f);
           }
     }
}
int theta=0;
void idle()
{
    int i, j;
    float dt;
    present_time = glutGet(GLUT_ELAPSED_TIME);
    dt = 0.001f*(present_time -  last_time);
	theta = (theta+1)%360;
    for(i=0; i<INITIAL_NUM_PARTICLES; i++) 
    {
       for(j=0; j<3; j++)
       {
           particles[i].position[j]+=dt*particles[i].velocity[j];    
           particles[i].velocity[j]+=dt*forces(i,j)/particles[i].mass;
	   }
    collision(i);
    }
	
	last_time = present_time;
	glutPostRedisplay();
}
void init()
{
	//int i,j;
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	InitializeProgram();
	InitializeVertexBuffer();
	//int const c = 2;
	for(int i=0; i<INITIAL_NUM_PARTICLES; i++)
	{
		particles[i].mass = 4.0f;
		particles[i].color = i%NUM_COLORS;
		for(int j = 0; j<3; j++)
		{
			particles[i].position[j] = 2.0f*((float) rand()/RAND_MAX)-1.0f;
			particles[i].velocity[j] = speed*2.0f*((float) rand()/RAND_MAX)-1.0f;
		}
		//particles[i].position[2] = 1.0f; particles[i].position[2] = 1.0f;

		particles[i].position[3] = 1.0f;
		particles[i].velocity[3] = 0.0f;
		pcolors[i] = colors[particles[i].color];
		ppoints[i] = particles[i].position;
	}
	
	glPointSize(point_size);
	glutIdleFunc(idle);

  	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

void adjustVertexData()
{
	for(int i=0; i< INITIAL_NUM_PARTICLES; i++)
	{
		pcolors[i] = colors[particles[i].color];
		ppoints[i] = particles[i].position;
	}
	
	myRotationMatrixUnif = glGetUniformLocation(theProgram, "myRotationMatrix");
	myRotationMatrix = glm::rotate(glm::mat4(1.0f),float(theta),glm::vec3(0.0f,0.0f,1.0f));

	glUniformMatrix4fv(myRotationMatrixUnif, 1, GL_FALSE, glm::value_ptr(myRotationMatrix));
	
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ppoints), ppoints);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//float *f = (float *)pcolors;
	//for (int i=0; i < num_particles*4; i++) printf(" %f ",f[i]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pcolors), pcolors);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_POINTS, 0,INITIAL_NUM_PARTICLES );
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

//Called to update the display.
//You should call glutSwapBuffers after all of your rendering to display what you rendered.
//If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.


void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUseProgram(theProgram);
		adjustVertexData();
	glUseProgram(0);
	glutSwapBuffers();
	glutPostRedisplay();
}

//Called whenever the window is resized. The new window size is given, in pixels.
//This is an opportunity to call glViewport or glScissor to keep up with the change in size.
void reshape (int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	
}

//Called whenever a key on the keyboard was pressed.
//The key is given by the ''key'' parameter, which is in ASCII.
//It's often a good idea to have the escape key (ASCII value 27) call glutLeaveMainLoop() to 
//exit the program.
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	  case 27:
		  glutLeaveMainLoop();
		  break;
	}
}


unsigned int defaults(unsigned int displayMode, int &width, int &height) {return displayMode;}
