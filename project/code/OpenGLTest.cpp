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
OpenGLGlobalFunction(glUniformMatrix4fv);

#define Win32GetOpenGLFunction(Name) Name = (type_##Name *)wglGetProcAddress(#Name)




LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


int CALLBACK
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow)
{


    WNDCLASSA WindowClass = {};
    
    WindowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    WindowClass.lpfnWndProc = WndProc;
    WindowClass.hInstance = hInstance;
//    WindowClass.hIcon;
    WindowClass.lpszClassName = "HandmadeHeroWindowClass";
    
    if(RegisterClassA(&WindowClass))
    {
        HWND Window =
            CreateWindowExA(
                0,
                WindowClass.lpszClassName,
                "Handmade Hero",
                WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                0,
                0,
                hInstance,
                0);
	}

	HWND Window = CreateWindowEx(WS_EX_CLIENTEDGE, "Edit", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,  50, 100, 300, 300, NULL, NULL, hInstance, NULL);

	MSG Message;
	while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
	{		
		TranslateMessage(&Message);
		DispatchMessageA(&Message);
	}

    //MessageBoxA(0, "This is Handmade Hero.", "Handmade Hero", MB_OK|MB_ICONINFORMATION);
    
    return(0);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CREATE:
		{
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
			PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
			32,                   // Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,                   // Number of bits for the depthbuffer
			8,                    // Number of bits for the stencilbuffer
			0,                    // Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		HDC ourWindowHandleToDeviceContext = GetDC(hWnd);

		int  letWindowsChooseThisPixelFormat;
		letWindowsChooseThisPixelFormat = ChoosePixelFormat(ourWindowHandleToDeviceContext, &pfd); 
		SetPixelFormat(ourWindowHandleToDeviceContext,letWindowsChooseThisPixelFormat, &pfd);

		HGLRC ourOpenGLRenderingContext = wglCreateContext(ourWindowHandleToDeviceContext);
		wglMakeCurrent (ourWindowHandleToDeviceContext, ourOpenGLRenderingContext);

		MessageBoxA(0,(char*)glGetString(GL_VERSION), "OPENGL VERSION",0);

		wglDeleteContext(ourOpenGLRenderingContext);
		PostQuitMessage(0);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}