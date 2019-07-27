#pragma once
const int NUM_OF_BUILDINGS = 100;
const double PI = 3.14159;
const int NUM_OF_WINDOWS = 100;
const int NUM_OF_STARS = 100;
const int NUM_OF_WAVES = 8;
const int NUM_OF_BOATS = 8;
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
	double y;
	double x;
	double speed;
	double size;
	double sizeIndex;
	COLOR color;
} BOAT;



typedef struct {
	double x;
	double y;
	double width;
	bool windows[NUM_OF_WINDOWS][NUM_OF_WINDOWS];
	int windowsPerRow;
	int windowStyle;
} BUILDING;


void drawWindows(double x, double y, double w);
void drawSingleBuildingReflection(double x, double y, double w);
void drawWindowsReflection(double x, double y, double w, double windowWidth, double windowBuffer);
double fRand(double fMin, double fMax);