#pragma once
const int NUM_OF_BUILDINGS = 100;
const double PI = 3.14159;
const int NUM_OF_WINDOWS = 100;
const int NUM_OF_STARS = 200;
const int NUM_OF_WAVES = 8;
const int NUM_OF_BOATS = 8;
const int NUM_OF_STREETLIGHTS = 14;
const double STAR_SPEED = 0.0001;
const double STAR_SIZE = 0.01;
const double WAVE_SPEED = -0.0005; // minus = towards city, plus = away from city
const double FLIP_WINDOW_RATE = 0.2;
const double WINDOWS_ON = 0.6;
const double POSTS_WIDTH = 0.01;
const double POSTS_HEIGHT = 0.1;
const double POSTS_MIN = -1.2;
const double POSTS_MAX = 1.2;
const double POST_SHIFT = 0;
const double LIGHT_SIZE = 0.08;
const double LIGHT_FLICKER_CHANCE = 0.1;
const double LIGHT_FLICKER_SPEED = 0.05;

typedef struct {
	double x, y, size, speed;
} STAR;

typedef struct {
	double y;
	double size;
	double sizeIndex;
} WAVE;

typedef struct {
	double red, green, blue, alpha;
	bool useAlpha;
} COLOR;

typedef struct {
	double x, y;
	COLOR color;
} POINT_2D;

typedef struct {
	double y;
	double x;
	double speed;
	double size;
	double sizeIndex;
	COLOR color;
} BOAT;

typedef struct {
	POINT_2D * points;
	int numOfPoints;
	bool windows[NUM_OF_WINDOWS][NUM_OF_WINDOWS];
	int windowsPerRow;
	int windowStyle;
} BUILDING;

typedef struct {
	double width;
	double height;
	bool isLightOn;
	bool isLightFlickering;
	COLOR lightInsideColor;
	COLOR lightOutsideColor;
	COLOR poleBottom;
	COLOR poleTop;
} STREETLIGHT;


void drawWindows(double x, double y, double w);
double fRand(double fMin, double fMax);
void drawHelper(POINT_2D * pointsArr, int numOfPoints, bool isReflection);
