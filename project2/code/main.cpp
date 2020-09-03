// Includes
#include <stdio.h>
#include <string.h>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm\mat4x4.hpp>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// Global variables
const GLint global_WIDTH = 800, global_HEIGHT = 600;
const float toRadians = 3.14159 / 180.0f;
GLuint VAO, VBO, IBO, shader, uniformModel;
bool direction = true;
float triOffset = 0.0f;
float triMaxoffset = 0.7f;
float triIncrement = 0.0005f;
float curAngle = 0.0f;
bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex Shader
static const char* vShader = "									\n\
#version 330													\n\
																\n\
layout (location = 0) in vec3 pos;								\n\
																\n\
out vec4 vCol;													\n\
																\n\
uniform mat4 model;												\n\
																\n\
void main()														\n\
{																\n\
	gl_Position = model * vec4(pos, 1.0);						\n\
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);					\n\
}";

static const char* fShader = "			\n\
#version 330							\n\
										\n\
										\n\
in vec4 vCol;							\n\
										\n\
out vec4 colour;						\n\
										\n\
void main()								\n\
{										\n\
	colour = vCol;						\n\
}";


void CreateTriangle()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f, 
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

// Main Function
int main()
{
    if(!glfwInit())
    {
        printf("GLFW init failed!");
        glfwTerminate();
        return 1;
    }

    // Set OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *MainWindow = glfwCreateWindow(global_WIDTH, global_HEIGHT, "OpenGL Test Project", NULL, NULL);
    if(!MainWindow)
    {
        printf("GLFW window failed!");
        glfwTerminate();
        return 1;
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(MainWindow, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(MainWindow);
    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK)
    {
        printf("GLEW init failed!");
        glfwDestroyWindow(MainWindow);
        glfwTerminate();
        return 1;
    }

	glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

    // Main loop
    while(!glfwWindowShouldClose(MainWindow))
    {
        glfwPollEvents();

        if (direction)
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxoffset)
		{
			direction = !direction;
		}

		curAngle += 0.01f;
		if (curAngle >= 360)
		{
			curAngle -= 360;
		}

		if (sizeDirection)
		{
			curSize += 0.0001f;
		}
		else
		{
			curSize -= 0.0001f;
		}

		if (curSize >= maxSize || curSize <= minSize)
		{
			sizeDirection = !sizeDirection;
		}

        // Clear Window
        glClearColor(0.4f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);

        glm::mat4 model;
        //model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
        model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		GLfloat modelviewmatrix[] = {
			1.0f , 0.0f , 0.0f , 0.0f,
			0.0f, 1.0f, 0.0f , 0.0f ,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
				};

		glUniform1f(uniformModel, triOffset);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, modelviewmatrix);


		glBindVertexArray(VAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		glUseProgram(0);

		glfwSwapBuffers(MainWindow);
    }

    return 0;
}