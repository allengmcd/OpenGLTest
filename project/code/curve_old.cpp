#include "curve_old.h"

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

void DrawPoint(float vertex[3])
{
    glBegin(GL_TRIANGLES);
    glVertex3f( vertex[0], vertex[1] + 0.01f , vertex[2] );  
    glVertex3f( vertex[0] + 0.01f, vertex[1] - 0.01f , vertex[2]);  
    glVertex3f( vertex[0] - 0.01f, vertex[1] - 0.01f , vertex[2]);
    glEnd();
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


int CatmullRomCurve(float vertex[][3], int length, int steps)
{
    if(length <= 3)
    {
        return -1;
    }
    
    //for(int i = 0; i < length; i++)
    //{
    //    DrawPoint(vertex[i]);
    //}
    
    
    glBegin(GL_LINES);
    for(int k = 1; k < length-2; k++)
    {
        float xPrev= vertex[k][0], yPrev = vertex[k][1], zPrev = vertex[k][2];
        
        //float tanX = 0.5f * (vertex[k+1][0] - vertex[k-1][0]), tanY = 0.5f * (vertex[k+1][1] - vertex[k-1][1]), tanZ = 0.5f * (vertex[k+1][2] - vertex[k-1][2]);
        //float slope = tanY/tanX;
        //glVertex3f( xPrev - 0.1f, yPrev + ( -0.1f)*slope, zPrev);  
        //glVertex3f( xPrev + 0.1f, yPrev + ( 0.1f)*slope, zPrev); 
        
        
        float x1 = vertex[k-1][0], x2 = vertex[k][0], x3 = vertex[k+1][0], x4 = vertex[k+2][0];
        float y1 = vertex[k-1][1], y2 = vertex[k][1], y3 = vertex[k+1][1], y4 = vertex[k+2][1];
        float z1 = vertex[k-1][2], z2 = vertex[k][2], z3 = vertex[k+1][2], z4 = vertex[k+2][2];
        
        float xA = x2;
        float xB = -0.5f * x1 + 0.5f * x3;
        float xC = x1 - 2.5f * x2 + 2 * x3 - 0.5f * x4;
        float xD = -0.5 * x1 + 1.5f * x2 - 1.5 * x3 + 0.5f * x4;
        
        float yA = y2;
        float yB = -0.5f * y1 + 0.5f * y3;
        float yC = y1 - 2.5f * y2 + 2 * y3 - 0.5f * y4;
        float yD = -0.5 * y1 + 1.5f * y2 - 1.5 * y3 + 0.5f * y4;
        
        float zA = z2;
        float zB = -0.5f * z1 + 0.5f * z3;
        float zC = z1 - 2.5f * z2 + 2 * z3 - 0.5f * z4;
        float zD = -0.5 * z1 + 1.5f * z2 - 1.5 * z3 + 0.5f * z4;
        
        
        for (unsigned i = 0; i <= steps; i++)
        {
            float t = (float)i / (float)steps;
            
            float valX = 0, valY = 0, valZ = 0;
            
            valX = xA + xB * t + xC * POW(t, 2) + xD * POW(t, 3);
            valY = yA + yB * t + yC * POW(t, 2) + yD * POW(t, 3);
            valZ = zA + zB * t + zC * POW(t, 2) + zD * POW(t, 3);
            
            
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