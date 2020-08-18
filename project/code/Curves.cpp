#include <SDKDDKVer.h>
#include <windows.h>
#include <GL/GL.h>
#include "GL/glext.h"
#include "GL/wglext.h"
#include <stdio.h>

void CreateCurve(GLuint* VAO, GLuint* VBO, GLfloat* vertices, GLuint length);

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


int BezierCurve(GLfloat vertex[][3], int length, int steps, GLuint* VAO, GLuint* VBO)
{
    if(length % 4 != 0)
    {
        return -1;
    }
    
	GLfloat *vertices = (GLfloat*)malloc(sizeof(GLfloat)*100*3);
	int counter=0;

    for(int k = 0; k < length; k+=4)
    {
        //glBegin(GL_LINES);
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
            
            
            //glVertex3f(xPrev, yPrev, zPrev);
            //glVertex3f(valX, valY, valZ);
            xPrev = valX;
            yPrev = valY;
            zPrev = valZ;
			
			vertices[counter] = valX;
			vertices[counter+1] = valY;
			vertices[counter+2] = valZ;
			counter += 3;
        }
        //glVertex3f(xPrev, yPrev, zPrev);
        //glVertex3f(vertex[k+3][0], vertex[k+3][1], vertex[k+3][1]);
    }
    

	/* for(int i = 0; i < length; i++)
	{
		vertices[i*3] = vertex[i][0];
		vertices[i*3+1] = vertex[i][1];
		vertices[i*3+2] = vertex[i][2];
	}
 */

	CreateCurve(VAO, VBO, vertices, length*3);

    return 0;
}


void CreateCurve(GLuint* VAO, GLuint* VBO, GLfloat* vertices, GLuint length)
{   
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);
    
	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*length, vertices, GL_STATIC_DRAW);
    
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    
	glBindVertexArray(0);

	
    //glDrawArrays(GL_LINE_STRIP, 0, 6);
}
