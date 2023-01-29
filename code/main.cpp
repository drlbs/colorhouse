#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Utils.h"
using namespace std;

#define numVAOs 1
#define numVBOs 2

Utils util = Utils();
float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// variable allocation for display
GLuint mvLoc, projLoc, lookAtLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, lookAtMat;

void setupVertices(void) {
    // Set up 12 triangles with 2 per each cube face.  That's 36 vertices
    // or 108 points.
    /*
	float vertexPositions[108] = {
		-1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};
    */

    float vertexPositions[168] = {  // Using 4 component homogeneous coordinates 
         	 -1.0f, -1.0f, 0.0f, 1.0f,   // Bottom
	         -1.0f,  1.0f, 0.0f, 1.0f,
	          1.0f,  1.0f, 0.0f, 1.0f,
	          1.0f,  1.0f, 0.0f, 1.0f,
	          1.0f, -1.0f, 0.0f, 1.0f,
         	 -1.0f, -1.0f, 0.0f, 1.0f, 
	         -1.0f, -1.0f, 0.0f, 1.0f,  // Left Side
	         -1.0f, -1.0f, 1.0f, 1.0f,
	          1.0f, -1.0f, 1.0f, 1.0f,
	          1.0f, -1.0f, 1.0f, 1.0f,
	          1.0f, -1.0f, 0.0f, 1.0f,
	         -1.0f, -1.0f, 0.0f, 1.0f,
              -1.0f, 1.0f, 0.0f, 1.0f,  // Right Side
              -1.0f, 1.0f, 1.0f, 1.0f,
               1.0f, 1.0f, 1.0f, 1.0f,
               1.0f, 1.0f, 1.0f, 1.0f,
               1.0f, 1.0f, 0.0f, 1.0f,
              -1.0f, 1.0f, 0.0f, 1.0f,
              -1.0f, -1.0f, 0.0f, 1.0f, // Back Side    
              -1.0f, -1.0f, 1.0f, 1.0f,
              -1.0f,  1.0f, 1.0f, 1.0f, 
              -1.0f,  1.0f, 1.0f, 1.0f, 
              -1.0f,  1.0f, 0.0f, 1.0f,
              -1.0f, -1.0f, 0.0f, 1.0f,     
               1.0f, -1.0f, 0.0f, 1.0f, // Front Side      
               1.0f, -1.0f, 1.0f, 1.0f,
               1.0f,  1.0f, 1.0f, 1.0f,
               1.0f,  1.0f, 1.0f, 1.0f,
               1.0f,  1.0f, 0.0f, 1.0f,
               1.0f, -1.0f, 0.0f, 1.0f,       
              -1.0f, -1.0f, 1.0f, 1.0f, // Top Left 
               1.0f, -1.0f, 1.0f, 1.0f,
               1.0f, 0.0f, 2.0f, 1.0f,
               1.0f, 0.0f, 2.0f, 1.0f,
               -1.0f, 0.0f, 2.0f, 1.0f,               
               -1.0f, -1.0f, 1.0f, 1.0f, 
               -1.0f, 1.0f, 1.0f, 1.0f,  // Top Right
               1.0f, 1.0f, 1.0f, 1.0f,
               1.0f, 0.0f, 2.0f, 1.0f,
               1.0f, 0.0f, 2.0f, 1.0f,
               -1.0f, 0.0f, 2.0f, 1.0,
               -1.0f, 1.0f, 1.0f, 1.0f }; 

    float vertexColors[168] = {  // Using 4 component homogeneous coordinates 
         	  1.0f,  0.0f, 0.0f, 1.0f,   // Bottom
	          1.0f,  0.0f, 0.0f, 1.0f,
	          1.0f,  0.0f, 0.0f, 1.0f,
	          1.0f,  0.0f, 0.0f, 1.0f,
	          1.0f,  0.0f, 0.0f, 1.0f,
         	  1.0f,  0.0f, 0.0f, 1.0f, 
	          0.0f,  1.0f, 0.0f, 1.0f,  // Left Side
	          0.0f,  1.0f, 0.0f, 1.0f,
	          0.0f,  1.0f, 0.0f, 1.0f,
	          0.0f,  1.0f, 0.0f, 1.0f,
	          0.0f,  1.0f, 0.0f, 1.0f,
	          0.0f,  1.0f, 0.0f, 1.0f,
	          0.0f,  0.0f, 1.0f, 1.0f,  // Right Side
	          0.0f,  0.0f, 1.0f, 1.0f,
	          0.0f,  0.0f, 1.0f, 1.0f,
	          0.0f,  0.0f, 1.0f, 1.0f,
	          0.0f,  0.0f, 1.0f, 1.0f,
	          0.0f,  0.0f, 1.0f, 1.0f,
	          1.0f,  1.0f, 0.0f, 1.0f,  // Back Side
	          1.0f,  1.0f, 0.0f, 1.0f,
	          1.0f,  1.0f, 0.0f, 1.0f,
	          1.0f,  1.0f, 0.0f, 1.0f,
	          1.0f,  1.0f, 0.0f, 1.0f,
	          1.0f,  1.0f, 0.0f, 1.0f,
	          1.0f,  0.0f, 1.0f, 1.0f,  // Front Side
	          1.0f,  0.0f, 1.0f, 1.0f,
	          1.0f,  0.0f, 1.0f, 1.0f,
	          1.0f,  0.0f, 1.0f, 1.0f,
	          1.0f,  0.0f, 1.0f, 1.0f,
	          1.0f,  0.0f, 1.0f, 1.0f,
	          0.0f,  1.0f, 1.0f, 1.0f,  // Top Left 
	          0.0f,  1.0f, 1.0f, 1.0f,
	          0.0f,  1.0f, 1.0f, 1.0f,
	          0.0f,  1.0f, 1.0f, 1.0f,
	          0.0f,  1.0f, 1.0f, 1.0f,
	          0.0f,  1.0f, 1.0f, 1.0f,
	          1.0f,  0.2f, 1.0f, 1.0f,  // Top Right 
	          1.0f,  0.2f, 1.0f, 1.0f,
	          1.0f,  0.2f, 1.0f, 1.0f,
	          1.0f,  0.2f, 1.0f, 1.0f,
	          1.0f,  0.2f, 1.0f, 1.0f,
	          1.0f,  0.2f, 1.0f, 1.0f};


    // Use ONE vao as before	
    glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
    // Use TWO virtual buffer objects, vbo[0] is for the vertexPositions
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	
    // vbo[1] is for the vertexColors 
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
    // These are the camera positions and cube locations.  We will need them to
    // set up the modelview and perspective matrices.  Note the scope of the variables.
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
	cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f;
	setupVertices();
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(renderingProgram);

    // These are getting the memory locations of the modelview and projection
    // matrices from the renderer
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
	lookAtLoc = glGetUniformLocation(renderingProgram, "lookAt_matrix");

    // Now we set the perspective based on the window size and the aspect ratio.
    // The glm::perspective function rerturn the projection matrix
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

    // Now we are going to move the camera and cube to set up the modelview matrix.
    //  -- NOTE -- now using lookAt to specify camera position, so set the vmat to
    //  the identity matrix using glm:translate
	//vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    // Now let's also leave the cube at the center of the scene
	//mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,0.0f));
    // OpenGL undestands that the following line is actually MATRIX MULTIPLICATION and 
    // produces the modelview matrix from vMat and mMat.
	mvMat = vMat * mMat;

    // Now let's set up the lookAt matrix
    lookAtMat = glm::lookAt( glm::vec3(-8.0f,-5.0f,3.0f), 
                             glm::vec3(0.0f, 0.0f, 0.0f),
                             glm::vec3(0.0f, 0.0f, 1.0f) );

    // Now place the modelview and projection matrices in the memory locations
    // we recovered earlier from the renderer.
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(lookAtLoc, 1, GL_FALSE, glm::value_ptr(lookAtMat));

    // Now we are going to bind the vertex attribute array buffer and then 
    // tell GL what type of data is in the vbo.  They are
    // floating point vertices arranged as 4 component vectors.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);
    // And now we tell GL that this is the array we want to draw with glDrawArrays
	glEnableVertexAttribArray(0);
    // And now the colors
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, 0);
    // And now we tell GL that this is the array we want to draw with glDrawArrays
	glEnableVertexAttribArray(1);

    // Turn on the Z-Buffer Depth Test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

    // Now draw the 42 vertices in the vao 
	glDrawArrays(GL_TRIANGLES, 0, 42);
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "Under Construction", NULL, NULL);
	glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
