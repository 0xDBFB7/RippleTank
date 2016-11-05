#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#define grid_width 200
#define pixel_size 3
#define num_waves 3
#define damping_multiplier 0.0005
#define amplitude_multiplier 10
#define timestep .2
float wave_locations_x[200] = {100,150,150};
float wave_locations_y[200] = {100,150,100};

float wave_frequencies[200] = {0.5,1,2};
float wave_phases[200] = {0.31,1,2};

float grid[grid_width*grid_width] = {0};

void update() {
   glutPostRedisplay();
}

void drawRect(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(grid_width*pixel_size, grid_width*pixel_size);
    glutCreateWindow("Genetic");
	glViewport(0, 0, grid_width*pixel_size, grid_width*pixel_size);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, grid_width*pixel_size, 0.0f, grid_width*pixel_size, 0.0f, 1.0f);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);
    float t = 0;
    while(1){
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    glLoadIdentity();
	    for(int x=0;x<grid_width;x++){//y
	    	for(int y=0;y<grid_width;y++){//x
	    		float amplitude = 0;
	    		for(int wave = 0;wave<num_waves;wave++){
	    			amplitude += sin(t*wave_frequencies[wave]+wave_phases[wave])*sin(sqrtf(pow(wave_locations_x[wave]-x,2)+pow(wave_locations_y[wave]-y,2)))*(1/(damping_multiplier*(pow(wave_locations_x[wave]-x,2)+pow(wave_locations_y[wave]-y,2)))*amplitude_multiplier); //* for some cool patterns
	        	}
	        	if(amplitude > 0){
	        		glColor3f(amplitude/100.0,0.0f, amplitude);
	        	}
	        	else{
	        		//continue;
	        		glColor3f(0.0f,-amplitude/30.0,0.0f);
	        	}
	        	drawRect(x*pixel_size,y*pixel_size,pixel_size,pixel_size);
	        }
	    }
	    glutSwapBuffers();
		update(0);
		t+=timestep;
	}
}

//		for (int i = 0; i < num_waves; i++)
	    // {
	    // 	wave_locations_x[i] = (rand()/(float)RAND_MAX)*grid_width;
	    //     wave_locations_y[i] = (rand()/(float)RAND_MAX)*grid_width;
	    // }