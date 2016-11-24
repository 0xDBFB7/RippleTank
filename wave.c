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

#define grid_width 1920*2
#define grid_length 1080*2
#define pixel_size 0.5
#define num_waves 2
#define damping_multiplier 0.00001
#define amplitude_multiplier 1
#define timestep .05
#define wave_speed 1000.0

int shot_counter = 0;

float wave_locations_x[200] = {grid_width/2+300,grid_width/2-30};
float wave_locations_y[200] = {grid_length/2,grid_length/2};

float wave_frequencies[200] = {0.4,0.3};
float wave_phases[200] = {0.31,0.31};
float wave_periods[num_waves] = {0.3,0.3};
float wave_powers[num_waves] = {1,1,1};

float material_x[200] = {50,51,52,53,54,55,56};
float material_y[200] = {30,30,30,30,30,30,30};
float material_refractive[200] = {1,1,1,1,1,1,1,1,1};

void update() {
   glutPostRedisplay();
}
void TakeScreenshot(float time)
{
	unsigned char *buffer;
	char filename[200];
	int w = grid_width;
	int h = grid_length;
	int buf_size = 18 + (w * h * 3);
	int i;
	unsigned char temp;
	FILE *out_file;
	char filename2[200];
	sprintf(filename2,"/media/arthurdent/6304EE2C3516D9B3/Interferance/shots/%i.tga",shot_counter);
	if (!(out_file = fopen(filename2, "wb")))
	{
		return;
	}
 
	// allocate mem to read from frame buf
	if (!(buffer = (unsigned char *) calloc(1, buf_size)))
	{
		return;
	}
 
	// set header info
	buffer[2] = 2;	// uncompressed
	buffer[12] = w & 255;
	buffer[13] = w >> 8;
	buffer[14] = h & 255;
	buffer[15] = h >> 8;
	buffer[16] = 24;	// 24 bits per pix
 
	// read frame buf
	glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer + 18);
 
	// RGB to BGR
	for (i = 18; i < buf_size; i += 3)
	{
		temp = buffer[i];
		buffer[i] = buffer[i + 2];
		buffer[i + 2] = temp;
	}
	// write header + color buf to file
	fwrite(buffer, sizeof(unsigned char), buf_size, out_file);
	// cleanup
	fclose(out_file);
	free(buffer);
	shot_counter++;
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glEnable(GL_MULTISAMPLE);
    glutInitWindowSize(grid_width*pixel_size, grid_length*pixel_size);
    glutCreateWindow("Genetic");
	glViewport(0, 0, grid_width*pixel_size, grid_length*pixel_size);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, grid_width*pixel_size, 0.0f, grid_length*pixel_size, 0.0f, 1.0f);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_MULTISAMPLE_ARB);
    float t = 0;
    while(1){
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    glLoadIdentity();
	    for(int x=0;x<grid_width;x++){//y
	    	for(int y=0;y<grid_length;y++){//x
	    		float amplitude = 0;
	    		for(int wave = 0;wave<num_waves;wave++){
	    			float distance = pow(wave_locations_x[wave]-x,2)+pow(wave_locations_y[wave]-y,2);
	    			if(distance < pow(t*10.0,2)*wave_speed){
	    				amplitude += sin(t*wave_frequencies[wave]+wave_phases[wave])*sin(sqrtf(distance)*wave_periods[wave])*(1.0/(damping_multiplier*distance)*amplitude_multiplier); //* for some cool patterns
	    			}
	        	}
	        	if(amplitude > 50){
	        		glColor3f(0.0f,amplitude/2000.0,0.0f);
	        	}
	        	else if(amplitude > 0){//amplitude/100.0,0.0f,
	        		glColor3f(0.0f,0.0f,amplitude);
	        	}
	        	else{
	        		//continue;
	        		glColor3f(-amplitude/30.0,0.0f,0.0f);
	        	}
	        	drawRect(x*pixel_size,y*pixel_size,pixel_size,pixel_size);
	        }
	    }
	    glutSwapBuffers();
		update(0);
		t+=timestep;
		if(t == 2){

		}
		//printf("%i\n",(int)floor(t/timestep));
		TakeScreenshot(t);
		//break;
	}
}

//		for (int i = 0; i < num_waves; i++)
	    // {
	    // 	wave_locations_x[i] = (rand()/(float)RAND_MAX)*grid_width;
	    //     wave_locations_y[i] = (rand()/(float)RAND_MAX)*grid_width;
	    // }