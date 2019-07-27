#include "glut.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Func.h"

const int NUM_OF_BUILDINGS = 100;
const double PI = 3.14159;
const int NUM_OF_WINDOWS = 100;
const int NUM_OF_STARS = 100;
const int NUM_OF_WAVES = 8;
double waveSpeed = 0.001;

typedef struct {
	double x, y, size, speed;
} STAR;

typedef struct {
	double y;
	double size;
	double sizeIndex;
} WAVE;

typedef struct {
	double red, green, blue;
} COLOR;

typedef struct {
	double x;
	double y;
	double width;
	bool windows[NUM_OF_WINDOWS][NUM_OF_WINDOWS];
	int windowsPerRow;
	int windowStyle;
} BUILDING;

BUILDING buildings[NUM_OF_BUILDINGS];
STAR stars[NUM_OF_STARS];
WAVE waves[NUM_OF_WAVES];

int buildingIndex = 0;

void init()
{
	int i, j;
	glClearColor(0.5, 0.5, 0.5, 0);// color of window background
	glOrtho(-1, 1, -1, 1, -1, 1); // set the coordinates system
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	srand(time(NULL));

	for (int i = 0; i < NUM_OF_BUILDINGS; i++)
	{
		buildings[i].windowsPerRow = rand() % 5 + 3;
		buildings[i].windowStyle = rand() % 1000 / 1000.0 > 0.8 ? 0 : 1;
		for (int j = 0; j < NUM_OF_WINDOWS; j++)
		{
			for (int k = 0; k < NUM_OF_WINDOWS; k++)
			{
				buildings[i].windows[j][k] = rand() % 1000 / 1000.0 > 0.1 ? 1 : 0;
			}
		}
	}

	for (int i = 0; i < NUM_OF_STARS; i++)
	{
		stars[i].x = rand() % 1000 / 500.0 - 1;
		stars[i].y = rand() % 1000 / 1000.0;
		stars[i].size = rand() % 1000 / 100000.0;
		stars[i].speed = rand() % 1000 / 1000000.0 + 0.001;
	}

	for (int i = 0; i < NUM_OF_WAVES; i += 1)
	{
		waves[i].y = -i * i / (double)(NUM_OF_WAVES*NUM_OF_WAVES);
		waves[i].size = (i + 1)*(i + 1) / (double)(NUM_OF_WAVES*NUM_OF_WAVES) + waves[i].y;
		waves[i].sizeIndex = i;
	}

}

void drawStars()
{
	for (int i = 0; i < NUM_OF_STARS; i++)
	{
		glBegin(GL_POLYGON);
		glColor3d(1, 1, 0.5);
		glVertex2d(stars[i].x, stars[i].y); //1
		glVertex2d(stars[i].x + stars[i].size, stars[i].y); //2
		glVertex2d(stars[i].x + stars[i].size, stars[i].y - stars[i].size); //3
		glVertex2d(stars[i].x, stars[i].y - stars[i].size); //4
		glEnd();
	}

	// Reflection

	for (int i = 0; i < NUM_OF_STARS; i++)
	{
		glBegin(GL_POLYGON);
		glColor3d(1, 1, 0.5);
		glVertex2d(stars[i].x, -stars[i].y); //1
		glVertex2d(stars[i].x + stars[i].size, -stars[i].y); //2
		glVertex2d(stars[i].x + stars[i].size, -stars[i].y + stars[i].size); //3
		glVertex2d(stars[i].x, -stars[i].y + stars[i].size); //4
		glEnd();
	}
}

void drawBackground()
{
	glColor3d(0, 0, 0.1);
	glBegin(GL_POLYGON);
	glVertex2d(-1, 1); //1
	glVertex2d(1, 1); //2
	glColor3d(0, 0, 1);
	glVertex2d(1, 0); //3
	glVertex2d(-1, 0); //4
	glEnd();

	glColor3d(0, 0, 1);
	glBegin(GL_POLYGON);
	glVertex2d(-1, 0); //1
	glVertex2d(1, 0); //2
	glColor3d(0, 0, 0.1);
	glVertex2d(1, -1); //3
	glVertex2d(-1, -1); //4
	glEnd();
}

void drawSingleBuilding(double x, double y, double w)
{
	buildings[buildingIndex].x = x;
	buildings[buildingIndex].y = y;
	buildings[buildingIndex].width = w;

	glColor3d(fabs(y), 0.7*fabs(y), 0.5*fabs(y));
	glBegin(GL_POLYGON);
	glVertex2d(x, y); // 1
	glColor3d(0.8 - fabs(0.3*x), 0.5 - fabs(0.3*x), 0.8 - fabs(0.3*x));
	glVertex2d(x, 0);//2
	glVertex2d(x + w, 0);//3
	glColor3d(0.3*fabs(y), 0.5*fabs(y), 0.7*fabs(y));
	glVertex2d(x + w, y);//4
	glEnd();

	// add shadow
	double shadowWidth = w / 3;
	glColor3d(0.2, 0, 0.2);
	glBegin(GL_POLYGON);
	glVertex2d(x, y); // 1
	glVertex2d(x - shadowWidth, y - shadowWidth);
	glVertex2d(x - shadowWidth, 0);
	glVertex2d(x, 0); // 1
	glEnd();

	// add reflection
	drawSingleBuildingReflection(x, y, w);
	// add windows
	drawWindows(x, y, w);
}

void drawSingleBuildingReflection(double x, double y, double w)
{
	y = -y;
	glColor3d(fabs(y), 0.7*fabs(y), 0.5*fabs(y));
	glBegin(GL_POLYGON);
	glVertex2d(x, y); // 1
	glColor3d(0.8 - fabs(0.3*x), 0.5 - fabs(0.3*x), 0.8 - fabs(0.3*x));
	glVertex2d(x, 0);//2
	glVertex2d(x + w, 0);//3
	glColor3d(0.3*fabs(y), 0.5*fabs(y), 0.7*fabs(y));
	glVertex2d(x + w, y);//4
	glEnd();

	// add shadow
	double shadowWidth = w / 3;
	glColor3d(0.1, 0, 0.1);
	glBegin(GL_POLYGON);
	glVertex2d(x, y); // 1
	glVertex2d(x - shadowWidth, y + shadowWidth);
	glVertex2d(x - shadowWidth, 0);
	glVertex2d(x, 0); // 1
	glEnd();
}

void drawWindows(double x, double y, double w)
{
	double windowBuffer = w / buildings[buildingIndex].windowsPerRow / 3;
	double windowWidth = (w - windowBuffer * (buildings[buildingIndex].windowsPerRow + 1)) / (buildings[buildingIndex].windowsPerRow);
	double windowBuffer_x = windowBuffer * buildings[buildingIndex].windowStyle;
	int k = 0, l = 0;
	for (double i = x + windowBuffer; i < x + w - windowWidth; i += (windowWidth + windowBuffer_x))
	{
		for (double j = y - windowBuffer * 2; j >= windowWidth; j -= (windowWidth + windowBuffer))
		{
			buildings[buildingIndex].windows[k][l] == 1 ? glColor3d(1, 1, 0) : glColor3d(0.2, 0.2, 0);
			glBegin(GL_POLYGON);
			glVertex2d(i, j); // 1
			glVertex2d(i + windowWidth, j); // 1
			glVertex2d(i + windowWidth, j - windowWidth); // 1
			glVertex2d(i, j - windowWidth); // 1
			k++;
			l++;

			glEnd();

		}
	}
	drawWindowsReflection(x, y, w, windowWidth, windowBuffer);
}

void drawWindowsReflection(double x, double y, double w, double windowWidth, double windowBuffer)
{
	y = -y;
	int k = 0, l = 0;
	double windowBuffer_x = windowBuffer * buildings[buildingIndex].windowStyle;
	for (double i = x + windowBuffer; i < x + w - windowWidth; i += (windowWidth + windowBuffer_x))
	{
		for (double j = y + windowBuffer * 2; j <= -windowWidth; j += (windowWidth + windowBuffer))
		{
			buildings[buildingIndex].windows[k][l] == 1 ? glColor3d(1, 1, 0) : glColor3d(0.2, 0.2, 0);
			glBegin(GL_POLYGON);
			glVertex2d(i, j); // 1
			glVertex2d(i + windowWidth, j); // 1
			glVertex2d(i + windowWidth, j + windowWidth); // 1
			glVertex2d(i, j + windowWidth); // 1
			k++;
			l++;
			glEnd();
		}
	}
}

void drawSkyline(double s_freq, double s_amp, double s_phase,
	double c_freq, double c_amp, double c_phase,
	double vertical_shift, double buildingWidth, double sampleFreq, double drawBuildingShift)
{
	// y = abs(z*sin(ax+b) + w*cos(cx+d)) + g
	double x, y;
	for (x = -1 + drawBuildingShift; x <= 1; x += sampleFreq)
	{
		y = vertical_shift + fabs(s_amp * sin(s_phase + s_freq * x) + c_amp * cos(c_phase + c_freq * x));
		drawSingleBuilding(x, y, buildingWidth);
		buildingIndex++;
	}

}

void drawMoon()
{
	glBegin(GL_POLYGON);
	for (double x = -1; x < 1; x += 0.05)
	{
		glColor3d(0, 0, 0.3);
		glVertex2d(cos(x*PI) / 12 - 0.8, sin(x*PI) / 12 + 0.8); //1
	}
	glEnd();

	glBegin(GL_POLYGON);
	for (double x = -1; x < 1; x += 0.05)
	{
		double moonColor = sin(x*PI) > 0.1 ? sin(x*PI) : 0.1;
		glColor4d(moonColor, moonColor, moonColor, sin(x*PI));
		glVertex2d(cos(x*PI) / 12 - 0.8, sin(x*PI) / 12 + 0.8); //1
	}
	glEnd();

	// reflection
	glBegin(GL_POLYGON);
	for (double x = -1; x < 1; x += 0.05)
	{
		glColor3d(0, 0, 0.3);
		glVertex2d(cos(x*PI) / 12 - 0.8, sin(x*PI) / 12 - 0.8); //1
	}
	glEnd();

	glBegin(GL_POLYGON);
	for (double x = -1; x < 1; x += 0.05)
	{
		double moonColor = -sin(x*PI) > 0.1 ? -sin(x*PI) : 0.1;
		glColor4d(moonColor, moonColor, moonColor, -sin(x*PI));
		glVertex2d(cos(x*PI) / 12 - 0.8, sin(x*PI) / 12 - 0.8); //1
	}
	glEnd();
}

void drawStreetLamps()
{
	double step = 0.1;
	for (double x = -0.95; x < 1; x += step)
	{
		glBegin(GL_POLYGON);
		glColor3d(0.5, 0.5, 0);
		glVertex2d(x, step); //1
		glVertex2d(x + step / 10, step); //2
		glColor3d(0, 0, 0);
		glVertex2d(x + step / 10, 0); //3
		glVertex2d(x, 0); //4
		glEnd();

		glBegin(GL_POLYGON);
		glColor4d(1, 1, 0.6, 1);
		glVertex2d(x + step / 20, step);
		for (double i = -1; i < 1; i += 0.1)
		{
			glColor4d(1, 1, 1, 0);
			glVertex2d(cos(i*PI) / 20 + (x + step / 20), sin(i*PI) / 20 + step); // streetlamp's light
		}
		glEnd();

	}

	// reflection
	for (double x = -0.95; x < 1; x += step)
	{
		glBegin(GL_POLYGON);
		glColor3d(0, 0, 0);
		glVertex2d(x, 0); //1
		glVertex2d(x + step / 10, 0); //2
		glColor3d(0.5, 0.5, 0);
		glVertex2d(x + step / 10, -step); //3
		glVertex2d(x, -step); //4
		glEnd();

		glBegin(GL_POLYGON);
		glColor4d(1, 1, 0.6, 1);
		glVertex2d(x + step / 20, -step);
		for (double i = -1; i < 1; i += 0.1)
		{
			glColor4d(1, 1, 1, 0);
			glVertex2d(cos(i*PI) / 20 + (x + step / 20), sin(i*PI) / 20 - step); // streetlamp's light
		}
		glEnd();
	}
}

void overlayReflection()
{
	double y;
	// glColor4d(0, 0.2*fabs(1 + y), 0.6*fabs(1 + y), fabs(y/3) + 0.5);
	glColor4d(0.1, 0, 0.4, 0.5);
	glBegin(GL_POLYGON);
	glVertex2d(-1, 0); //1
	glVertex2d(1, 0); //2
	glColor4d(0, 0, 0.1, 1);
	glVertex2d(1, -1); //3
	glVertex2d(-1, -1); //4
	glEnd();
}

void drawSeaWaves()
{
	for (int i = 0; i < NUM_OF_WAVES; i++)
	{
		glBegin(GL_POLYGON);
		glColor4d(0, 0, 0.2, 0.8);
		glVertex2d(-1, waves[i].y); //1
		glVertex2d(1, waves[i].y); //2
		glColor4d(0, 0, 0, 0);
		glVertex2d(1, waves[i].y - waves[i].size); //3
		glVertex2d(-1, waves[i].y - waves[i].size); //4
		glEnd();
	}
}

void flipWindows(int index)
{
	int i = rand() % NUM_OF_WINDOWS;
	int j = rand() % NUM_OF_WINDOWS;
	buildings[index].windows[i][j] ^= rand() % 1000 / 1000.0 > 0.1 ? 1 : 0;
}

void drawSingleBackBuilding(double x, double y, double w, COLOR color)
{
	glColor3d(color.red, color.green, color.blue);
	glBegin(GL_POLYGON);
	glVertex2d(x, y); // 1
	glVertex2d(x, 0);//2
	glVertex2d(x + w, 0);//3
	glVertex2d(x + w, y);//4
	glEnd();

	// reflection
	y = -y;
	glColor3d(color.red, color.green, color.blue);
	glBegin(GL_POLYGON);
	glVertex2d(x, y); // 1
	glVertex2d(x, 0);//2
	glVertex2d(x + w, 0);//3
	glVertex2d(x + w, y);//4
	glEnd();
}

void drawBackSkyline(double s_freq, double s_amp, double s_phase,
	double c_freq, double c_amp, double c_phase,
	double vertical_shift, double buildingWidth, double sampleFreq, double drawBuildingShift, COLOR color)
{
	double x, y;
	for (x = -1 + drawBuildingShift; x <= 1; x += sampleFreq)
	{
		y = vertical_shift + fabs(s_amp * sin(s_phase + s_freq * x) + c_amp * cos(c_phase + c_freq * x));
		drawSingleBackBuilding(x, y, buildingWidth, color);
	}
}

void display()
{
	double x, y;

	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	drawBackground();
	drawStars();
	drawMoon();

	double s_freq, s_amp, s_phase, c_freq, c_amp, c_phase, vertical_shift, buildingWidth, sampleFreq, drawBuildingShift;
	COLOR color;

	s_freq = PI * 10;
	s_amp = 0.05;
	s_phase = 0.3;
	c_freq = PI * 0.3;
	c_amp = 0.5;
	c_phase = -0.1;
	vertical_shift = 0.18;
	buildingWidth = 0.098;
	sampleFreq = 0.082;
	drawBuildingShift = -0.04;
	color.red = 0.1;
	color.green = 0;
	color.blue = 0.1;

	drawBackSkyline(s_freq, s_amp, s_phase, c_freq, c_amp, c_phase, vertical_shift, buildingWidth, sampleFreq, drawBuildingShift, color); // Background Buildings #1


	s_freq = PI * 20;
	s_amp = 0.05;
	s_phase = 0.4;
	c_freq = PI * 0.3;
	c_amp = 0.4;
	c_phase = -0.1;
	vertical_shift = 0.2;
	buildingWidth = 0.092;
	sampleFreq = 0.08;
	drawBuildingShift = -0.05;
	color.red = 0.15;
	color.green = 0;
	color.blue = 0.15;

	drawBackSkyline(s_freq, s_amp, s_phase, c_freq, c_amp, c_phase, vertical_shift, buildingWidth, sampleFreq, drawBuildingShift, color); // Background Buildings #2

	s_freq = PI * 5;
	s_amp = 0.1;
	s_phase = 0.4;
	c_freq = PI * 0.5;
	c_amp = 0.2;
	c_phase = -0.1;
	vertical_shift = 0.3;
	buildingWidth = 0.15;
	sampleFreq = 0.15;
	drawBuildingShift = 0.01;

	drawSkyline(s_freq, s_amp, s_phase, c_freq, c_amp, c_phase, vertical_shift, buildingWidth, sampleFreq, drawBuildingShift); // First Buildings Layer


	s_freq = PI * 10;
	s_amp = 0.1;
	s_phase = 0.4;
	c_freq = PI / 2;
	c_amp = 0.5;
	c_phase = -0.2;
	vertical_shift = 0.2;
	buildingWidth = 0.08;
	sampleFreq = 0.11;
	drawBuildingShift = 0.01;

	drawSkyline(s_freq, s_amp, s_phase, c_freq, c_amp, c_phase, vertical_shift, buildingWidth, sampleFreq, drawBuildingShift);// Second Buildings Layer

	s_freq = PI * 5;
	s_amp = 0.2;
	s_phase = 0.3;
	c_freq = PI / 2;
	c_amp = 0.2;
	c_phase = -0.1;
	vertical_shift = 0.2;
	buildingWidth = 0.09;
	sampleFreq = 0.12;
	drawBuildingShift = -0.01;

	drawSkyline(s_freq, s_amp, s_phase, c_freq, c_amp, c_phase, vertical_shift, buildingWidth, sampleFreq, drawBuildingShift);// Third Buildings Layer

	s_freq = PI * 5;
	s_amp = 0.1;
	s_phase = 0.4;
	c_freq = PI * 0.5;
	c_amp = 0.05;
	c_phase = -0.1;
	vertical_shift = 0.08;
	buildingWidth = 0.21;
	sampleFreq = 0.34;
	drawBuildingShift = 0.08;

	drawSkyline(s_freq, s_amp, s_phase, c_freq, c_amp, c_phase, vertical_shift, buildingWidth, sampleFreq, drawBuildingShift); // Foreground Buildings


	for (int i = 0; i < buildingIndex; i++)
	{
		flipWindows(i);
	}
	drawStreetLamps();
	overlayReflection();
	drawSeaWaves();

	buildingIndex = 0;



	glutSwapBuffers(); // show all
}

void idle()
{
	int i, j;

	for (int i = 0; i < NUM_OF_STARS; i++)
	{
		stars[i].x -= stars[i].speed;

		if (fabs(stars[i].x) > 1)
		{
			stars[i].x = 1;
			stars[i].y = -1 + (rand() % 1000) / 500.0; // random number in (-1,1)
		}
	}

	for (int i = 0; i < NUM_OF_WAVES; i++)
	{
		waves[i].sizeIndex += waveSpeed;
		waves[i].y = -waves[i].sizeIndex * waves[i].sizeIndex / (double)(NUM_OF_WAVES*NUM_OF_WAVES);
		waves[i].size = (waves[i].sizeIndex + 1)*(waves[i].sizeIndex + 1) / (double)(NUM_OF_WAVES*NUM_OF_WAVES) + waves[i].y;
		if (waves[i].sizeIndex <= 0)
		{
			waves[i].sizeIndex = NUM_OF_WAVES;
		}
		else if (waves[i].sizeIndex > NUM_OF_WAVES)
		{
			waves[i].sizeIndex = 0;
		}
	}

	glutPostRedisplay(); // go to display
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("City Example");

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	init();

	glutMainLoop();
}