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

#define grid_width 500
#define pixel_size 1
#define num_waves 2
#define num_walls 5
#define damping_multiplier 0.0000001
#define amplitude_multiplier 10
#define timestep .03
#define wave_speed 1000
float wave_locations_x[200] = {grid_width/2,grid_width/2};
float wave_locations_y[200] = {grid_width/2,grid_width/2+50};
float wave_locations_z[200] = {grid_width/2,grid_width/2};
float wave_period[num_waves] = {0.5,0.5,0.3};

float wave_frequencies[200] = {0,0};
float wave_phases[200] = {3.14,-3.14};

void update() {
   glutPostRedisplay();
}

void initGL() {
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}


int main(int argc, char** argv){
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glEnable(GL_MULTISAMPLE);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Genetic");
	glViewport(0, 0, 500, 500);
    initGL();
    float t = 0;
    while(1){
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    double aspect = (double)viewport[2] / (double)viewport[3];
    gluPerspective(60, aspect, 1, 400);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // move back a bit
    //glTranslatef(-100*t,0,-1);

    glTranslatef(0,0, -200);
    glRotatef(t*50, 0.1, 0.1, 0);
    glTranslatef(-grid_width/2,-grid_width/2, -grid_width/2);
   	    for(int x=0;x<grid_width;x++){//y
	    	for(int y=0;y<grid_width;y++){//x
	    		for(int z=100;z<110;z++){//x
		    		float amplitude = 0;
		    		for(int wave = 0;wave<num_waves;wave++){
		    			float distance = pow(wave_locations_x[wave]-x,2)+pow(wave_locations_y[wave]-y,2)+pow(wave_locations_z[wave]-z,2);
		    			if(distance < t*wave_speed){
		    				amplitude += sin(t*wave_frequencies[wave]+wave_phases[wave])*sin(sqrtf(distance)*wave_period[wave])*(1/(damping_multiplier*distance)*amplitude_multiplier); //* for some cool patterns
		        		}
		        	}
		        	if(amplitude > 0){
		        		glPushMatrix();
					        glTranslatef(x*pixel_size,y*pixel_size,z*pixel_size);
					        glColor3ub(255,(x/(float)grid_width)*255.0,(z/(float)grid_width)*255.0);
					        glutWireCube(pixel_size);
					    glPopMatrix();
		        	}
		       //  	else{
		       //  		glPushMatrix();
					    //     glTranslatef(x*pixel_size,y*pixel_size,z*pixel_size);
					    //     glColor3ub(0,0,(amplitude/10.0)*255.0);
					    //     glutSolidCube(pixel_size);
					    // glPopMatrix();
		       //  	}
		        }
	        }
	    }
    	//glTranslatef(-grid_width/2,-grid_width/2, -grid_width/2);
	    glutSwapBuffers();
		update(0);
		t+=timestep;
		glPopMatrix();
	}
}

//		for (int i = 0; i < num_waves; i++)
	    // {
	    // 	wave_locations_x[i] = (rand()/(float)RAND_MAX)*grid_width;
	    //     wave_locations_y[i] = (rand()/(float)RAND_MAX)*grid_width;
	    // }