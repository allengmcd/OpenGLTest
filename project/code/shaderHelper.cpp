#include <SDKDDKVer.h>
#include <windows.h>
#include <GL/GL.h>
#include "GL/glext.h"
#include "GL/wglext.h"
#include <stdio.h>

#define BUFFERSIZE 1024

DWORD g_BytesTransferred = 0;


VOID CALLBACK FileIOCompletionRoutine(
__in  DWORD dwErrorCode,
__in  DWORD dwNumberOfBytesTransfered,
__in  LPOVERLAPPED lpOverlapped
);


VOID CALLBACK FileIOCompletionRoutine(
__in  DWORD dwErrorCode,
__in  DWORD dwNumberOfBytesTransfered,
__in  LPOVERLAPPED lpOverlapped )
{
    //_tprintf(TEXT("Error code:\t%x\n"), dwErrorCode);
    //_tprintf(TEXT("Number of bytes:\t%x\n"), dwNumberOfBytesTransfered);
    g_BytesTransferred = dwNumberOfBytesTransfered;
}


void ReadShader(char* fileName, char* outShader)
{
    HANDLE hFile;
    DWORD  dwBytesRead = 0;
    char   ReadBuffer[BUFFERSIZE] = {0};
    OVERLAPPED ol = {0};
    
    hFile = CreateFile(fileName,               // file to open
                       GENERIC_READ,          // open for reading
                       FILE_SHARE_READ,       // share for reading
                       NULL,                  // default security
                       OPEN_EXISTING,         // existing file only
                       FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
                       NULL);                 // no attr. template
    
    if (hFile == INVALID_HANDLE_VALUE) 
    { 
        //_tprintf(TEXT("Terminal failure: unable to open file \"%s\" for read.\n"), "shaders/shader.vert");
        //return ""; 
    }
    
    // Read one character less than the buffer size to save room for
    // the terminating NULL character. 
    
    if( FALSE == ReadFileEx(hFile, ReadBuffer, BUFFERSIZE-1, &ol, FileIOCompletionRoutine) )
    {
        //printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", GetLastError());
        CloseHandle(hFile);
        //return "";
    }
    
    
    SleepEx(5000, TRUE);
    dwBytesRead = g_BytesTransferred;
    // This is the section of code that assumes the file is ANSI text. 
    // Modify this block for other data types if needed.
    
    if (dwBytesRead > 0 && dwBytesRead <= BUFFERSIZE-1)
    {
        ReadBuffer[dwBytesRead]='\0'; // NULL character
        
        //_tprintf(TEXT("Data read from %s (%d bytes): \n"), argv[1], dwBytesRead);
        //printf("%s\n", ReadBuffer);
    }
    
    
    for(int i=0; i < BUFFERSIZE; ++i){
        outShader[i] = ReadBuffer[i];
        
        if(ReadBuffer[i] == '\0')
        {
            continue;
        }
    }
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




unsigned int CompileShaders(char* vertexPath, char* fragmentPath)
{
	unsigned int shader = glCreateProgram();
    
	if (!shader)
	{
		printf("Error creating shader program!\n");
		return -1;
	}
    
    
    char vShader[BUFFERSIZE];
    char fShader[BUFFERSIZE];
    ReadShader(vertexPath, vShader);
    ReadShader(fragmentPath, fShader);
    
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
		return -1;
	}
    
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return -1;
	}
    
	//uniformModel = glGetUniformLocation(shader, "model");

	return shader;
}