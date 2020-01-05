#include "curve.h"

float POW(float value, int power)
{
	float returnValue = 1;
	for (int i = 0; i < power; i++)
	{
		returnValue = returnValue * value;
	}

	return returnValue;
}

int binomialCoeff(int n, int k)
{
	// Base Cases
	if (k == 0 || k == n)
		return 1;

	// Recur
	return  binomialCoeff(n - 1, k - 1) + binomialCoeff(n - 1, k);
}

float bezFunc(int i, int n, float t, float P)
{
	float binCoe = binomialCoeff(n, i);
	
	return (binCoe * POW(1-t, n-i) * POW(t, i) * P);
}


int BezierCurve(float vertex[][3], int length, int steps)
{
    if(length % 4 != 0)
    {
        return -1;
    }


    for(int k = 0; k < length; k+=4)
    {
        glBegin(GL_LINES);
        float xPrev= -0.4f, yPrev = 0.8f, zPrev = 0.0f;
        for (unsigned i = 0; i < steps; ++i)
        {
            float t = (float)i / (float)steps;
            float x, y, z;

            float valX = 0, valY = 0, valZ = 0;
            for (int j = 0; j < 4; j++)
            {
                valX += bezFunc(j, 3, t, vertex[k+j][0]);
                valY += bezFunc(j, 3, t, vertex[k+j][1]);
                valZ += bezFunc(j, 3, t, vertex[k+j][2]);
            }

            
            glVertex3f(xPrev, yPrev, zPrev);
            glVertex3f(valX, valY, valZ);
            xPrev = valX;
            yPrev = valY;
            zPrev = valZ;
        }
        glEnd();
    }
  
    return 0;
}