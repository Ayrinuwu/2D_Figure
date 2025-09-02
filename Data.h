#define _USE_MATH_DEFINES
#include <cmath>

double					// Исходные значения параметров X0, Y0, px, py
	X0=200,
	Y0=200,
	px=50,
	py=100;

double Parabola(double x)
{
	return x*x-2;
}

double Sinusoid(double x)
{
	return sin(x);
}

double TestPolar1(double phi)
{
	return 12*sin(2*phi)-10*cos(phi);
}

double TestPolar2(double phi)
{
	return 5.;
}