
#include <SDKDDKVer.h>
#include <windows.h>
#include <GL/GL.h>
#include "GL/glext.h"
#include "GL/wglext.h"
#include <stdio.h>
#include <cmath>
#include "shaderHelper.cpp"
#include "OpenGLHelper.cpp"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


struct Config {
	int width;
	int	height;
	int posX;
	int posY;
	bool windowed;
} config;

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265 / 180.0f;

GLuint VAO, VBO, shader, uniformModel;

bool direction = true;
float triOffset = 0.0f;
float triMaxoffset = 0.7f;
float triIncrement = 0.0005f;
float curAngle = 0.0f;
bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;


void CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};
    
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
    
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    
	glBindVertexArray(0);
}


void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    
	GLuint theShader = glCreateShader(shaderType);
    
	const GLchar* theCode[1];
	theCode[0] = shaderCode;
    
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);
    
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);
    
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
    
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}
    
	glAttachShader(theProgram, theShader);
}




void CompileShaders()
{
	shader = glCreateProgram();
    
	if (!shader)
	{
		printf("Error creating shader program!\n");
		return;
	}
    
    
    char vShader[BUFFERSIZE];
    char fShader[BUFFERSIZE];
    ReadShader("shaders/shader.vert", vShader);
    ReadShader("shaders/shader.frag", fShader);
    
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);
    
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
    
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}
    
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}
    
	uniformModel = glGetUniformLocation(shader, "model");
}

ATOM registerClass(HINSTANCE hInstance) {
    
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = "Core";
    
	return RegisterClassEx(&wcex);
}


void showMessage(LPCSTR message) {
    
	MessageBox(0, message, "Window::create", MB_ICONERROR);
}


void adjustSize(DWORD style) {
	RECT rect = { 0, 0, config.width, config.height };
	AdjustWindowRect(&rect, style, false);
	config.width = rect.right - rect.left;
	config.height = rect.bottom - rect.top;
}


void center() {
	RECT primaryDisplaySize;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &primaryDisplaySize, 0);	// system taskbar and application desktop toolbars not included
	config.posX = (primaryDisplaySize.right - config.width) / 2;
	config.posY = (primaryDisplaySize.bottom - config.height) / 2;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



int CALLBACK
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow)
{
	LPTSTR windowClass;	// Window Class
	HGLRC RC;			// Rendering Context
	HDC	DC;				// Device Context
	HWND WND;			// Window
	DWORD style;
    
	config.width = 1024;
	config.height = 720;
	config.posX = CW_USEDEFAULT;
	config.posY = 0;
	config.windowed = true;
	style = WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    
    windowClass = MAKEINTATOM(registerClass(hInstance));
	if (windowClass == 0) {
		showMessage("registerClass() failed.");
		return 1;
	}
    
	// create temporary window
    
	HWND fakeWND = CreateWindow(
                                windowClass, "Fake Window",
                                style,
                                0, 0,						// position x, y
                                1, 1,						// width, height
                                NULL, NULL,					// parent window, menu
                                hInstance, NULL);			// instance, param
    
	HDC fakeDC = GetDC(fakeWND);	// Device Context
    
	PIXELFORMATDESCRIPTOR fakePFD;
	ZeroMemory(&fakePFD, sizeof(fakePFD));
	fakePFD.nSize = sizeof(fakePFD);
	fakePFD.nVersion = 1;
	fakePFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	fakePFD.iPixelType = PFD_TYPE_RGBA;
	fakePFD.cColorBits = 32;
	fakePFD.cAlphaBits = 8;
	fakePFD.cDepthBits = 24;
    
	const int fakePFDID = ChoosePixelFormat(fakeDC, &fakePFD);
	if (fakePFDID == 0) {
		showMessage("ChoosePixelFormat() failed.");
		return 1;
	}
    
	if (SetPixelFormat(fakeDC, fakePFDID, &fakePFD) == false) {
		showMessage("SetPixelFormat() failed.");
		return 1;
	}
    
	HGLRC fakeRC = wglCreateContext(fakeDC);	// Rendering Contex
    
	if (fakeRC == 0) {
		showMessage("wglCreateContext() failed.");
		return 1;
	}
    
	if (wglMakeCurrent(fakeDC, fakeRC) == false) {
		showMessage("wglMakeCurrent() failed.");
		return 1;
	}
    
	// get pointers to functions (or init opengl loader here)
    
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
	wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
	if (wglChoosePixelFormatARB == nullptr) {
		showMessage("wglGetProcAddress() failed.");
		return 1;
	}
    
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
	wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
	if (wglCreateContextAttribsARB == nullptr) {
		showMessage("wglGetProcAddress() failed.");
		return 1;
	}
    
	if (config.windowed == true) {
		adjustSize(style);
		center();
	}
    
	// create a new window and context
    
	WND = CreateWindow(
                       windowClass, "OpenGL Window",	// class name, window name
                       style,							// styles
                       config.posX, config.posY,		// posx, posy. If x is set to CW_USEDEFAULT y is ignored
                       config.width, config.height,	// width, height
                       NULL, NULL,						// parent window, menu
                       hInstance, NULL);				// instance, param
    
	DC = GetDC(WND);
    
	const int pixelAttribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, 4,
		0
	};
    
	int pixelFormatID; UINT numFormats;
	const bool status = wglChoosePixelFormatARB(DC, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);
    
	if (status == false || numFormats == 0) {
		showMessage("wglChoosePixelFormatARB() failed.");
		return 1;
	}
    
	PIXELFORMATDESCRIPTOR PFD;
	DescribePixelFormat(DC, pixelFormatID, sizeof(PFD), &PFD);
	SetPixelFormat(DC, pixelFormatID, &PFD);
    
	const int major_min = 4, minor_min = 0;
	const int contextAttribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        //		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
		0
	};
    
	RC = wglCreateContextAttribsARB(DC, 0, contextAttribs);
	if (RC == NULL) {
		showMessage("wglCreateContextAttribsARB() failed.");
		return 1;
	}
    
	// delete temporary context and window
    
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(fakeRC);
	ReleaseDC(fakeWND, fakeDC);
	DestroyWindow(fakeWND);
	if (!wglMakeCurrent(DC, RC)) {
		showMessage("wglMakeCurrent() failed.");
		return 1;
	}
    
	SetWindowText(WND, reinterpret_cast<LPCSTR>(glGetString(GL_VERSION)));
	ShowWindow(WND, nCmdShow);
    
    OpenGLInit();
    
	// Get Buffer Size information
	int bufferWidth = 1040, bufferHeight = 759;
    
	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);
    
	CreateTriangle();
	CompileShaders();
    
    float timeValue = 0.0f;
    
	MSG Message;
	bool active = true;
	while (active) 
	{
		while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
		{		
			if (Message.message == WM_QUIT) {
				active = false;
			}
			TranslateMessage(&Message);
			DispatchMessageA(&Message);
		}
        
        
		// set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            0.0f,  0.5f, 0.0f   // top 
        };
        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        // glBindVertexArray(0);
        
        
        // bind the VAO (it was already bound, but just to demonstrate): seeing as we only have a single VAO we can 
        // just bind it beforehand before rendering the respective triangle; this is another approach.
        glBindVertexArray(VAO);
        
        
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // be sure to activate the shader before any calls to glUniform
        glUseProgram(shader);
        
        // update shader uniform
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shader, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        
        // render the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        
		glUseProgram(0);
        
		SwapBuffers(DC);
        
        timeValue += 0.001;
	}
    
    return(0);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
        }
        break;
		case WM_CLOSE:
        PostQuitMessage(0);
        break;
		default:
        return DefWindowProc(hWnd, message, wParam, lParam);
	}
    
	return 0;
}