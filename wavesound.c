#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <make_wav.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#define grid_width 200
#define pixel_size 3
#define num_waves 3
#define num_instruments 3
#define damping_multiplier 0.5
#define amplitude_multiplier 1
#define timestep .01
#define seconds 5
#define M_PI 3.14159
float wave_locations_x[200] = {100,150,1};
float wave_locations_y[200] = {100,150,1};

float wave_frequencies[200] = {0.34,1,0.25};
float wave_phases[200] = {0.31,1,2};

float instrument_locations_x[num_instruments] = {50,128};
float instrument_locations_y[num_instruments] = {50,128};

float instrument_frequencies[num_instruments] = {200,213};
float instrument_AM[num_instruments] = {0.1,0.1};
float instrument_FM[num_instruments] = {0,0};
#define S_RATE  44100
#define BUF_SIZE (S_RATE*seconds)
int buffer[BUF_SIZE];

float sampled_point = 0;

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
void drawWaves(int argc, char** argv){
    glColor3f(1.0f, 1.0f, 1.0f);
    for(float t=0;t<10;t+=0.1){
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    glLoadIdentity();
	    for(int x=0;x<grid_width;x++){//y
	    	for(int y=0;y<grid_width;y++){//x
	    		float amplitude = 0;
	    		for(int wave = 0;wave<num_waves;wave++){
	    			float distance = pow(wave_locations_x[wave]-x,2)+pow(wave_locations_y[wave]-y,2);
	    			amplitude += sin(t*wave_frequencies[wave]+wave_phases[wave])*sin(sqrtf(distance))*(1/(0.5*distance)*300);
	    			; //* for some cool patterns
	        	}
	        	if(amplitude > 0){
	        		glColor3f(amplitude/100.0,0.0f, amplitude);
	        	}
	        	else{
	        		glColor3f(0.0f,-amplitude/30.0,0.0f);
	        	}
	        	drawRect(x*pixel_size,y*pixel_size,pixel_size,pixel_size);
	        }
	    }
	    for(int m =0;m<num_instruments;m++){
	    	glColor3f(1.0f,1.0f, 1.0f);
	    	drawRect(instrument_locations_x[m]*pixel_size,instrument_locations_y[m]*pixel_size,pixel_size,pixel_size);
	    }
	    glutSwapBuffers();
		update(0);
	}
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
	while(1){
		for (int s=0; s<(int)(((seconds*S_RATE))); s++){
			buffer[s]=0;
		}
	    float phase=0;
	    float amplitude = 0;
	    float freq_radians_per_sample = 0;
	    for(float t = 0;t<(seconds-timestep);t=t+timestep){
	    	for(int in = 0;in<num_instruments;in++){
				amplitude = 0;
				for(int wave = 0;wave<num_waves;wave++){
					amplitude += sin((t*10)*wave_frequencies[wave]+wave_phases[wave])*sin(sqrtf(pow(wave_locations_x[wave]-instrument_locations_x[in],2)*pow(wave_locations_y[wave]-instrument_locations_y[in],2)));
		    	}
				freq_radians_per_sample = (((instrument_frequencies[in]+(instrument_frequencies[in]*instrument_FM[in]*amplitude)))*2.0*3.14159)/(int)S_RATE;
				float phase=0;
				for (int s=(int)(t*S_RATE); s<(int)(((t*S_RATE)+(timestep*S_RATE))); s++)
			    {
			        phase += freq_radians_per_sample;
			    	buffer[s] += (int)(32000 * (amplitude*instrument_AM[in])*sin(phase));
				}
			}
		}
		drawWaves(argc,argv);
		write_wav("test.wav", BUF_SIZE, buffer, S_RATE);
		system("aplay /home/arthurdent/Cpp/test.wav");
	}
}
