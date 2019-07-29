#include "glut.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Func.h"

BUILDING buildings[NUM_OF_BUILDINGS];
STAR stars[NUM_OF_STARS];
WAVE waves[NUM_OF_WAVES];
BOAT boats[NUM_OF_BOATS];
STREETLIGHT lights[NUM_OF_STREETLIGHTS];

int buildingIndex = 0;

void init()
{
	int i, j;
	glClearColor(0.5, 0.5, 0.5, 0);// color of window background
	glOrtho(-1, 1, -1, 1, -1, 1); // set the coordinates system
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	srand(time(NULL));

	// init buildings with random number of windows and style
	for (int i = 0; i < NUM_OF_BUILDINGS; i++)
	{
		buildings[i].windowsPerRow = rand() % 5 + 3;
		buildings[i].windowStyle = rand() % 1000 / 1000.0 > 0.8 ? 0 : 1;
		for (int j = 0; j < NUM_OF_WINDOWS; j++)
		{
			for (int k = 0; k < NUM_OF_WINDOWS; k++)
			{
				buildings[i].windows[j][k] = rand() % 1000 / 1000.0 > WINDOWS_ON ? 0 : 1;
			}
		}
	}

	// init stars with random position, speed and size
	for (int i = 0; i < NUM_OF_STARS; i++)
	{
		stars[i].x = rand() % 1000 / 500.0 - 1;
		stars[i].y = rand() % 1000 / 1000.0;
		stars[i].size = fRand(STAR_SIZE / 10, STAR_SIZE) + STAR_SIZE / 10;
		stars[i].speed = fRand(STAR_SPEED / 10, STAR_SPEED) + STAR_SPEED / 10;
	}

	// init waves with size index for each wave
	for (int i = 0; i < NUM_OF_WAVES; i += 1)
	{
		waves[i].sizeIndex = i;
	}

	// init boats with random values
	for (int i = 0; i < NUM_OF_BOATS; i += 1)
	{
		boats[i].sizeIndex = i;
		boats[i].size = fRand((double)i / (double)NUM_OF_BOATS, ((double)i + 1) / (double)NUM_OF_BOATS) / 2;
		boats[i].x = rand() % 1000 / 500.0 - 1;
		boats[i].speed = boats[i].size / 1000;
	}

	for (int i = 0; i < NUM_OF_STREETLIGHTS; i++)
	{
		lights[i].height = POSTS_HEIGHT;
		lights[i].width = POSTS_WIDTH;
		lights[i].lightInsideColor = { 1,1,0.8,1,TRUE };
		lights[i].lightOutsideColor = { 1,1,0.4,0.0001,TRUE };
		lights[i].isLightFlickering = rand() % 1000 / 1000.0 > LIGHT_FLICKER_CHANCE ? 0 : 1;
		if (lights[i].isLightFlickering)
			lights[i].isLightOn = rand() % 1000 / 1000.0 > 0.5 ? 1 : 0;
		else
			lights[i].isLightOn = 1;
	}
}

// returns double type random from min to max
double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

void drawStars()
{
	const int numOfPoints = 4;

	for (int i = 0; i < NUM_OF_STARS; i++)
	{
		POINT_2D points[numOfPoints] = { { stars[i].x , stars[i].y - stars[i].size / 2, { 1,1,0.5 } },
										{ stars[i].x + stars[i].size / 2 , stars[i].y , { 1,1,0.5 } },
										{ stars[i].x + stars[i].size , stars[i].y - stars[i].size / 2 , { 1,1,0.5 } },
										{ stars[i].x + stars[i].size / 2 , stars[i].y - stars[i].size , { 1,1,0.5 } } };

		drawHelper(points, numOfPoints, FALSE);
		drawHelper(points, numOfPoints, TRUE); // Reflection
	}
}

void drawHelper(POINT_2D * points, int numOfPoints, bool isReflection)
{
	if (isReflection)
	{
		for (int i = 0; i < numOfPoints; i++)
		{
			points[i].y = -points[i].y;
		}
	}
	glBegin(GL_POLYGON);
	for (int i = 0; i < numOfPoints; i++)
	{
		if (points[i].color.useAlpha)
			glColor4d(points[i].color.red, points[i].color.green, points[i].color.blue, points[i].color.alpha);
		else
			glColor3d(points[i].color.red, points[i].color.green, points[i].color.blue);
		glVertex2d(points[i].x, points[i].y);
	}
	glEnd();
}

void drawBoats()
{
	double size = 0.4;
	double position = 0.0;
	for (int i = 0; i < NUM_OF_BOATS; i++)
	{
		glBegin(GL_POLYGON);
		glColor3d(0.2*boats[i].size, 0.1*boats[i].size, 0.4*boats[i].size);
		for (double x = -1; x < 1; x += 0.1)
		{
			glVertex2d(sin(x*PI / 3)*boats[i].size + boats[i].x, -cos(x*PI / 4)*boats[i].size); //1
		}
		glEnd();
	}
}

void drawBackground()
{
	const int numOfPoints = 4;
	POINT_2D points[numOfPoints] = { { -1 , 1 , { 0,0,0.1 } },
									{ 1 , 1 , { 0,0,0.1 } },
									{ 1 , 0  , { 0, 0, 1 } },
									{ -1 , 0  , { 0, 0, 1 } } };
	drawHelper(points, numOfPoints, FALSE);
	drawHelper(points, numOfPoints, TRUE); // Reflection
}

void drawSingleBuilding(double x, double y, double w, bool windows, bool shadow)
{
	//buildings[buildingIndex].x = x;
	//buildings[buildingIndex].y = y;
	//buildings[buildingIndex].width = w;
	//COLOR color[4] = { { fabs(y),0.7*fabs(y),0.5*fabs(y) },
	//				{ 0.8 - fabs(0.3*x),0.5 - fabs(0.3*x),0.8 - fabs(0.3*x) },
	//				{ 0.8 - fabs(0.3*x),0.5 - fabs(0.3*x),0.8 - fabs(0.3*x) },
	//				{ 0.3*fabs(y), 0.5*fabs(y), 0.7*fabs(y) } };
	const int numOfPoints = 4;
	POINT_2D points_building[numOfPoints] = { { x , y , { fabs(y),0.7*fabs(y),0.5*fabs(y) } },
									{ x, 0 , { 0.8 - fabs(0.3*x),0.5 - fabs(0.3*x),0.8 - fabs(0.3*x) } },
									{ x + w, 0  , { 0.8 - fabs(0.3*x),0.5 - fabs(0.3*x),0.8 - fabs(0.3*x) } },
									{ x + w, y  , { 0.3*fabs(y), 0.5*fabs(y), 0.7*fabs(y) } } };

	drawHelper(points_building, numOfPoints, FALSE);
	drawHelper(points_building, numOfPoints, TRUE); // reflection

	if (shadow)
	{
		// add shadow
		double shadowWidth = w / 3;

		POINT_2D points_shadow[numOfPoints] = { { x , y , { 0.2, 0, 0.2 } },
												{ x - shadowWidth, y - shadowWidth , { 0.2, 0, 0.2 } },
												{ x - shadowWidth, 0  , { 0.4, 0.2, 0.2 } },
												{ x, 0  , { 0.4, 0.2, 0.2 } } };

		drawHelper(points_shadow, numOfPoints, FALSE);
		drawHelper(points_shadow, numOfPoints, TRUE); // reflection
	}

	if (windows)
	{
		// add windows
		drawWindows(x, y, w);
	}
}

void drawWindows(double x, double y, double w)
{
	const int numOfPoints = 4;
	double windowBuffer = w / buildings[buildingIndex].windowsPerRow / 3;
	double windowWidth = (w - windowBuffer * ((double)buildings[buildingIndex].windowsPerRow + 1)) / (buildings[buildingIndex].windowsPerRow);
	double windowBuffer_x = windowBuffer * buildings[buildingIndex].windowStyle;
	int k = 0, l = 0;
	for (double i = x + windowBuffer; i < x + w - windowWidth; i += (windowWidth + windowBuffer_x), k++)
	{
		for (double j = y - windowBuffer * 2; j >= windowWidth; j -= (windowWidth + windowBuffer), l++)
		{
			COLOR color;
			buildings[buildingIndex].windows[k][l] == 1 ? color = { 1, 1, 0 } : color = { 0.2, 0.2, 0 };
			POINT_2D points[numOfPoints] = { { i, j , color } ,
											{ i + windowWidth, j , color },
											{ i + windowWidth, j - windowWidth  , color },
											{ i, j - windowWidth  , color } };
			drawHelper(points, numOfPoints, FALSE);
			drawHelper(points, numOfPoints, TRUE);
		}
	}
}


void drawSkyline(double s_freq, double s_amp, double s_phase,
	double c_freq, double c_amp, double c_phase,
	double vertical_shift, double buildingWidth, double sampleFreq, double drawBuildingShift, bool toDrawWindows, bool toDrawShadow)
{
	// y = abs(z*sin(ax+b) + w*cos(cx+d)) + g
	double x, y;
	for (x = -1 + drawBuildingShift; x <= 1; x += sampleFreq)
	{
		y = vertical_shift + fabs(s_amp * sin(s_phase + s_freq * x) + c_amp * cos(c_phase + c_freq * x));
		drawSingleBuilding(x, y, buildingWidth, toDrawWindows, toDrawShadow);
		buildingIndex++;
	}

}

void drawMoon()
{
	// base color
	const int numOfPoints = 30;
	POINT_2D points[numOfPoints];
	double x = -1;
	const double step = 2.0 / numOfPoints;
	for (int i = 0; i < numOfPoints; i++, x += step)
	{
		points[i].x = cos(x*PI) / 12 - 0.8;
		points[i].y = sin(x*PI) / 12 + 0.8;
		points[i].color = { 0, 0, 0.3 };
	}
	drawHelper(points, numOfPoints, FALSE);
	drawHelper(points, numOfPoints, TRUE); // reflection

	// white overlay
	x = -1;
	for (int i = 0; i < numOfPoints; i++, x += step)
	{
		double moonColor = sin(x*PI) > 0.1 ? sin(x*PI) : 0.1;
		points[i].x = cos(x*PI) / 12 - 0.8;
		points[i].y = sin(x*PI) / 12 + 0.8;
		points[i].color = { moonColor, moonColor, moonColor, sin(x*PI), TRUE };
	}
	drawHelper(points, numOfPoints, FALSE);
	drawHelper(points, numOfPoints, TRUE); // reflection
}

void drawStreetLamps()
{
	const int numOfPointsPost = 4;
	const int numOfPointsLight = 20;

	const double step = (fabs(POSTS_MIN) + fabs(POSTS_MAX)) / NUM_OF_STREETLIGHTS;
	POINT_2D pointsLight[numOfPointsLight + 1];
	int i = 0;
	for (double x = POSTS_MIN + POST_SHIFT; x < POSTS_MAX + POST_SHIFT; x += step, i++)
	{
		COLOR colorTop = lights[i].isLightOn ? lights[i].poleTop : lights[i].poleBottom;
		COLOR colorBottom = lights[i].poleBottom;
		POINT_2D pointsPost[numOfPointsPost] = { {x, POSTS_HEIGHT, {colorTop.red, colorTop.green, colorTop.blue}},
												{x + POSTS_WIDTH, POSTS_HEIGHT, {colorTop.red, colorTop.green, colorTop.blue}},
												{x + POSTS_WIDTH, 0, {colorBottom.red, colorBottom.green, colorBottom.blue}},
												{x, 0, {colorBottom.red, colorBottom.green, colorBottom.blue}} };
		drawHelper(pointsPost, numOfPointsPost, FALSE);
		drawHelper(pointsPost, numOfPointsPost, TRUE);

		if (lights[i].isLightOn)
		{
			//	draw light
			pointsLight[0].x = x + POSTS_WIDTH / 2; // first center light point
			pointsLight[0].y = POSTS_HEIGHT;
			pointsLight[0].color = { 1, 1, 0.8, 1, TRUE };
			double x_lights = -1;
			for (int j = 1; j <= numOfPointsLight; j++, x_lights += (2.0 / (numOfPointsLight - 2)))
			{
				pointsLight[j].x = cos(x_lights*PI) * LIGHT_SIZE + (x + POSTS_WIDTH / 2);
				pointsLight[j].y = sin(x_lights*PI) * LIGHT_SIZE + POSTS_HEIGHT;
				pointsLight[j].color = { 1, 1, 0.4, 0, TRUE };

			}
			drawHelper(pointsLight, numOfPointsLight, FALSE);
			drawHelper(pointsLight, numOfPointsLight, TRUE);
		}
	}
}

void overlayReflection()
{
	const int numOfPoints = 4;
	POINT_2D points[numOfPoints] = { { -1, 0 , { 0.1, 0, 0.4, 0.5, TRUE } },
									{ 1, 0 , { 0.1, 0, 0.4, 0.5, TRUE } },
									{ 1, -1  , { 0, 0, 0.1, 1, TRUE } },
									{ -1, -1  , { 0, 0, 0.1, 1, TRUE } } };
	drawHelper(points, numOfPoints, FALSE);
}

void drawSeaWaves()
{
	const int numOfPoints = 4;
	for (int i = 0; i < NUM_OF_WAVES; i++)
	{
		POINT_2D points[numOfPoints] = { { -1, waves[i].y , { 0, 0, 0.1, 0.6, TRUE } },
								{ 1, waves[i].y , { 0, 0, 0.1, 0.6, TRUE } },
								{ 1, waves[i].y - waves[i].size , { 0, 0, 0, 0, TRUE } },
								{ -1, waves[i].y - waves[i].size  , { 0, 0, 0, 0, TRUE } } };
		drawHelper(points, numOfPoints, FALSE);
	}
}

// UNIMPLEMENTED
void drawGround()
{
	double y;
	glBegin(GL_POLYGON);
	glColor3d(1, 1, 1);
	glVertex2d(1, -0.1); //2
	glVertex2d(-1, -0.1); //1
	for (double x = -1.1; x <= 1.1; x += 0.1)
	{
		glColor3d(x, x, x);
		//y = vertical_shift + fabs(s_amp * sin(s_phase + s_freq * x) + c_amp * cos(c_phase + c_freq * x));
		glVertex2d(x, 0.05 + 0.05*sin(x * 20)); //1
		x += 0.1;
		glVertex2d(x, 0.05 + 0.05*sin(x * 20)); //1
	}

	//glVertex2d(-1, 0.01); //3
	//glVertex2d(-1, -0.01); //4
	glEnd();
}

void flipWindows(int index)
{
	int i = rand() % NUM_OF_WINDOWS;
	int j = rand() % NUM_OF_WINDOWS;
	buildings[index].windows[i][j] ^= rand() % 1000 / 1000.0 > FLIP_WINDOW_RATE ? 0 : 1;
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

	// Background Buildings #1
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
	drawBackSkyline(s_freq, s_amp, s_phase, c_freq, c_amp, c_phase, vertical_shift, buildingWidth, sampleFreq, drawBuildingShift, color);

	// Background Buildings #2
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
	drawBackSkyline(s_freq, s_amp, s_phase, c_freq, c_amp, c_phase, vertical_shift, buildingWidth, sampleFreq, drawBuildingShift, color);

	// First Buildings Layer
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
	drawSkyline(s_freq, s_amp, s_phase, c_freq, c_amp, c_phase, vertical_shift, buildingWidth, sampleFreq, drawBuildingShift, TRUE, TRUE);

	// Second Buildings Layer
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
	drawSkyline(s_freq, s_amp, s_phase, c_freq, c_amp, c_phase, vertical_shift, buildingWidth, sampleFreq, drawBuildingShift, TRUE, TRUE);

	// Third Buildings Layer
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
	drawSkyline(s_freq, s_amp, s_phase, c_freq, c_amp, c_phase, vertical_shift, buildingWidth, sampleFreq, drawBuildingShift, TRUE, TRUE);

	// Foreground Buildings
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
	drawSkyline(s_freq, s_amp, s_phase, c_freq, c_amp, c_phase, vertical_shift, buildingWidth, sampleFreq, drawBuildingShift, TRUE, TRUE);


	for (int i = 0; i < buildingIndex; i++)
	{
		flipWindows(i);
	}
	drawStreetLamps();
	overlayReflection();
	drawSeaWaves();
	//drawGround();


	//drawBoats();
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
		waves[i].sizeIndex += WAVE_SPEED;
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

	for (int i = 0; i < NUM_OF_BOATS; i++)
	{
		boats[i].x += boats[i].speed;
		if (boats[i].x > 1.5)
		{
			boats[i].x = -1.5;

			boats[i].size = fRand((double)i / NUM_OF_BOATS, ((double)i + 1) / NUM_OF_BOATS);
		}
		else if (boats[i].x < -1.5)
		{
			boats[i].x = 1.5;
			boats[i].size = fRand((double)i / NUM_OF_BOATS, ((double)i + 1) / NUM_OF_BOATS);
		}
	}

	for (int i = 0; i < NUM_OF_STREETLIGHTS; i++)
	{
		if (lights[i].isLightFlickering)
		{
			lights[i].isLightOn = rand() % 1000 / 1000.0 > LIGHT_FLICKER_SPEED ? lights[i].isLightOn : lights[i].isLightOn ^ 1;
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