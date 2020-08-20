#include <SDKDDKVer.h>
#include <windows.h>
#include <GL/GL.h>
#include "GL/glext.h"
#include "GL/wglext.h"
#include <stdio.h>

void BicubicSurface(GLuint* VAO, GLuint* VBO)
{

}

void CreateTriangle(GLuint* VAO, GLuint* VBO)
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f 
	};
    
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);
    
	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    
	glBindVertexArray(0);
}
