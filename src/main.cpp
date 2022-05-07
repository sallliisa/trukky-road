#include <stdio.h>
#include <stdlib.h>
#include "../include/glew.h"
#include "../include/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../common/shader.hpp"
#include "../common/controls.hpp"
#include "../common/texture.hpp"
#include "../common/objloader.hpp"

using namespace glm;
GLFWwindow* window;

GLuint  programID, matrixID, textureID, lightID,
	    texture0, texture1, texture2, texture3, car_vertexbuffer, car_uvbuffer, car_normalbuffer, 
		tree_vertexbuffer, tree_uvbuffer, tree_normalbuffer,
		car_VertexArrayID, tree_VertexArrayID, viewMatID, modelMatID;
std::vector<glm::vec3> car_vertices, car_normals, tree_vertices, tree_normals;
std::vector<glm::vec2> car_uvs, tree_uvs;
glm::vec3 lightPos;
glm::mat4 MVP, VP, model;
float carmov = 0;

glm::mat4 calculateVP() {
	getMatrices(window);
	glm::mat4 projection = getProjectionMatrix();
	glm::mat4 view = getViewMatrix();
	glUniformMatrix4fv(viewMatID, 1, GL_FALSE, &view[0][0]);
	return (glm::mat4) projection * view;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void glInit() {
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(1024, 768, "nigga", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// Init GLEW
	glewExperimental = true;
	glewInit();
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
}

void initShaders() {
	programID = LoadShaders("./res/shaders/vertexshader.glsl", "./res/shaders/fragmentshader.glsl");
	matrixID = glGetUniformLocation(programID, "MVP");
	viewMatID = glGetUniformLocation(programID, "V");
	modelMatID = glGetUniformLocation(programID, "M");
	lightID = glGetUniformLocation(programID, "lightPos_worldspace");
	glUseProgram(programID);
}

void loadTexture() {
	texture0 = loadBMP("./res/tex/car_1.bmp");
	texture1 = loadBMP("./res/tex/car_2.bmp");
	texture2 = loadBMP("./res/tex/car_3.bmp");
	texture3 = loadBMP("./res/tex/tree_1.bmp");
	textureID = glGetUniformLocation(programID, "tex0Sampler");
	glBindTextureUnit(0, texture0);
	glBindTextureUnit(1, texture1);
	glBindTextureUnit(2, texture2);
	glBindTextureUnit(3, texture3);
}

void loadOBJ() {
	bool car = loadOBJ("./res/obj/car_1.obj", car_vertices, car_uvs, car_normals);
	bool tree = loadOBJ("./res/obj/tree_1.obj", tree_vertices, tree_uvs, tree_normals);
}

void initVAO() {
	glGenBuffers(1, &car_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, car_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, car_vertices.size() * sizeof(glm::vec3), &car_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &car_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, car_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, car_uvs.size() * sizeof(glm::vec2), &car_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &car_normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, car_normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, car_normals.size() * sizeof(glm::vec3), &car_normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &tree_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tree_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, tree_vertices.size() * sizeof(glm::vec3), &tree_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &tree_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tree_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, tree_uvs.size() * sizeof(glm::vec2), &tree_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &tree_normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tree_normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, tree_normals.size() * sizeof(glm::vec3), &tree_normals[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &car_VertexArrayID);
	glGenVertexArrays(1, &tree_VertexArrayID);

	glBindVertexArray(car_VertexArrayID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, car_vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, car_uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, car_normalbuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glBindVertexArray(tree_VertexArrayID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, tree_vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, tree_uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, tree_normalbuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void displayPaint() {
	glBindVertexArray(car_VertexArrayID);
	model = glm::mat4(1.0f);
	MVP = VP * model;
	glUniform1i(textureID, 0);
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &model[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, car_vertices.size());

	model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
	MVP = VP * model;
	glUniform1i(textureID, 1);
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &model[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, car_vertices.size());

	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, 0.0f, 0.0f));
	MVP = VP * model;
	glUniform1i(textureID, 2);
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &model[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, car_vertices.size());

	glBindVertexArray(tree_VertexArrayID);
	glUniform1i(textureID, 3);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
	MVP = VP * model;
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &model[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, tree_vertices.size());
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	VP = calculateVP();
	lightPos = glm::vec3(4, 4, 4);
	glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);
	displayPaint();
	glfwSwapBuffers(window);
	glfwPollEvents();
	carmov += 0.01;
}

int main() {
	glInit();
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	initShaders();
	loadTexture();
	loadOBJ();
	initVAO();
	while ((glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) 
			&& (glfwWindowShouldClose(window) == 0))
		display();
	glDeleteBuffers(1, &car_vertexbuffer);
	glDeleteVertexArrays(1, &car_VertexArrayID);
	glDeleteBuffers(1, &tree_vertexbuffer);
	glDeleteVertexArrays(1, &tree_VertexArrayID);
	glDeleteProgram(programID);
	glDeleteTextures(1, &texture0);
	glDeleteTextures(1, &texture1);
	glfwTerminate();
	return 0;
}