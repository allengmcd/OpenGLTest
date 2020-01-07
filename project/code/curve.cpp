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
        float xPrev= vertex[k][0], yPrev = vertex[k][1], zPrev = vertex[k][2];
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
        glVertex3f(xPrev, yPrev, zPrev);
        glVertex3f(vertex[k+3][0], vertex[k+3][1], vertex[k+3][1]);

        glEnd();
    }
  
    return 0;
}

float HermiteFunction(float point1, float point2, float tangent1, float tangent2, float t)
{
    return (1-3*POW(t,2)+2*POW(t,3))*point1 + POW(t,2)*(3-2*t)*point2+t*POW((t-1),2)*tangent1+POW(t,2)*(t-1)*tangent2;
}

int HermiteCurve(float vertex[][3], float tangent[][3], int length, int steps)
{
    if(length < 3)
    {
        return -1;
    }

    glBegin(GL_LINES);
    for(int k = 0; k < length-1; k++)
    {
        float xPrev= vertex[k][0], yPrev = vertex[k][1], zPrev = vertex[k][2];
        for (unsigned i = 0; i <= steps; i++)
        {
            float t = (float)i / (float)steps;

            float valX = 0, valY = 0, valZ = 0;
            
            valX = HermiteFunction(vertex[k][0], vertex[k+1][0], tangent[k][0], tangent[k+1][0], t);
            valY = HermiteFunction(vertex[k][1], vertex[k+1][1], tangent[k][1], tangent[k+1][1], t);
            valZ = HermiteFunction(vertex[k][2], vertex[k+1][2], tangent[k][2], tangent[k+1][2], t);

            
            glVertex3f(xPrev, yPrev, zPrev);
            glVertex3f(valX, valY, valZ);
            xPrev = valX;
            yPrev = valY;
            zPrev = valZ;
        }
    }

    glEnd();
  
    return 0;
}



int CatmullRomCurve(float vertex[][3], float tangent[][3], int length, int steps)
{
    if(length < 3)
    {
        return -1;
    }

    glBegin(GL_LINES);
    for(int k = 0; k < length-1; k++)
    {
        float xPrev= vertex[k][0], yPrev = vertex[k][1], zPrev = vertex[k][2];
        for (unsigned i = 0; i <= steps; i++)
        {
            float t = (float)i / (float)steps;

            float valX = 0, valY = 0, valZ = 0;
            
            valX = HermiteFunction(vertex[k][0], vertex[k+1][0], tangent[k][0], tangent[k+1][0], t);
            valY = HermiteFunction(vertex[k][1], vertex[k+1][1], tangent[k][1], tangent[k+1][1], t);
            valZ = HermiteFunction(vertex[k][2], vertex[k+1][2], tangent[k][2], tangent[k+1][2], t);

            
            glVertex3f(xPrev, yPrev, zPrev);
            glVertex3f(valX, valY, valZ);
            xPrev = valX;
            yPrev = valY;
            zPrev = valZ;
        }
    }

    glEnd();
  
    return 0;
}