#include <SDKDDKVer.h>
#include <windows.h>
#include <GL/GL.h>
#include "GL/glext.h"
#include "GL/wglext.h"

#pragma comment (lib, "opengl32.lib")

#define global static

#define GL_FALSE 0

typedef void WINAPI type_glGenVertexArrays(GLsizei n, GLuint* arrays);
typedef void WINAPI type_glBindVertexArray(GLuint array);
typedef void WINAPI type_glGenBuffers(GLsizei n, GLuint* arrays);
typedef void WINAPI type_glBindBuffer(GLenum target, GLuint buffer);
typedef void WINAPI type_glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void WINAPI type_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
typedef void WINAPI type_glEnableVertexAttribArray(GLuint index);
typedef void WINAPI type_glUniform1f(GLint location, GLfloat v0);
typedef void WINAPI type_glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void WINAPI type_glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void WINAPI type_glUniform3fv(GLint location, GLsizei count, const GLfloat *value);
typedef void WINAPI type_glUniformMatrix4fv(GLint location,	GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void WINAPI type_glCompileShader(GLuint shader);
typedef GLuint WINAPI type_glCreateShader(GLenum shaderType);
typedef void WINAPI type_glShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
typedef void WINAPI type_glGetShaderiv(GLuint shader, GLenum pname, GLint* params);
typedef void WINAPI type_glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
typedef void WINAPI type_glAttachShader(GLuint program, GLuint shader);
typedef GLuint WINAPI type_glCreateProgram(void);
typedef void WINAPI type_glLinkProgram(GLuint program);
typedef void WINAPI type_glGetProgramiv(GLuint program, GLenum pname, GLint* params);
typedef void WINAPI type_glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
typedef void WINAPI type_glValidateProgram(GLuint program);
typedef GLint WINAPI type_glGetUniformLocation(GLuint program, const GLchar* name);
typedef void WINAPI type_glUseProgram(GLuint program);
typedef void WINAPI type_glBindFramebuffer(GLenum target, GLuint framebuffer);

#define OpenGLGlobalFunction(Name) global type_##Name *Name;
OpenGLGlobalFunction(glGenVertexArrays);
OpenGLGlobalFunction(glBindVertexArray);
OpenGLGlobalFunction(glGenBuffers);
OpenGLGlobalFunction(glBindBuffer);
OpenGLGlobalFunction(glBufferData);
OpenGLGlobalFunction(glVertexAttribPointer);
OpenGLGlobalFunction(glEnableVertexAttribArray);
OpenGLGlobalFunction(glCreateShader);
OpenGLGlobalFunction(glShaderSource);
OpenGLGlobalFunction(glCompileShader);
OpenGLGlobalFunction(glGetShaderiv);
OpenGLGlobalFunction(glGetShaderInfoLog);
OpenGLGlobalFunction(glAttachShader);
OpenGLGlobalFunction(glCreateProgram);
OpenGLGlobalFunction(glLinkProgram);
OpenGLGlobalFunction(glGetProgramiv);
OpenGLGlobalFunction(glGetProgramInfoLog);
OpenGLGlobalFunction(glValidateProgram);
OpenGLGlobalFunction(glGetUniformLocation);
OpenGLGlobalFunction(glUseProgram);
OpenGLGlobalFunction(glUniform1f);
OpenGLGlobalFunction(glUniform3f);
OpenGLGlobalFunction(glUniform4f);
OpenGLGlobalFunction(glUniform3fv);
OpenGLGlobalFunction(glUniformMatrix4fv);
OpenGLGlobalFunction(glBindFramebuffer);

#define Win32GetOpenGLFunction(Name) Name = (type_##Name *)wglGetProcAddress(#Name)

void OpenGLInit()
{
	Win32GetOpenGLFunction(glGenVertexArrays);
	Win32GetOpenGLFunction(glBindVertexArray);
	Win32GetOpenGLFunction(glGenBuffers);
	Win32GetOpenGLFunction(glBindBuffer);
	Win32GetOpenGLFunction(glBufferData);
	Win32GetOpenGLFunction(glVertexAttribPointer);
	Win32GetOpenGLFunction(glEnableVertexAttribArray);
	Win32GetOpenGLFunction(glCreateShader);
	Win32GetOpenGLFunction(glShaderSource);
	Win32GetOpenGLFunction(glCompileShader);
	Win32GetOpenGLFunction(glGetShaderiv);
	Win32GetOpenGLFunction(glGetShaderInfoLog);
	Win32GetOpenGLFunction(glAttachShader);
	Win32GetOpenGLFunction(glCreateProgram);
	Win32GetOpenGLFunction(glLinkProgram);
	Win32GetOpenGLFunction(glGetProgramiv);
	Win32GetOpenGLFunction(glValidateProgram);
	Win32GetOpenGLFunction(glGetProgramInfoLog);
	Win32GetOpenGLFunction(glGetUniformLocation);
	Win32GetOpenGLFunction(glUseProgram);
	Win32GetOpenGLFunction(glUniform1f);
	Win32GetOpenGLFunction(glUniform3f);
	Win32GetOpenGLFunction(glUniform4f);
	Win32GetOpenGLFunction(glUniform3fv);
	Win32GetOpenGLFunction(glUniformMatrix4fv);
	Win32GetOpenGLFunction(glBindFramebuffer);
    
}
