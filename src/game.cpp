#include <stdio.h>
#include <stdlib.h>
#include <glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../common/shader.hpp"
#include "../common/controls.hpp"
#include "../common/texture.hpp"
#include "../common/objloader.hpp"
#include "game.h"

GLFWwindow* window;

GLuint  programID, matrixID, textureID, lightID,
	    texture0, texture1, texture2, texture3, car_vertexbuffer, car_uvbuffer, car_normalbuffer, 
		tree_vertexbuffer, tree_uvbuffer, tree_normalbuffer,
		car_VertexArrayID, tree_VertexArrayID, viewMatID, modelMatID;
std::vector<glm::vec3> car_vertices, car_normals, tree_vertices, tree_normals;
std::vector<glm::vec2> car_uvs, tree_uvs;
glm::vec3 lightDir;
glm::mat4 MVP, VP, model;
float WORLD_SPEED = 0.2;
float gravity = 7;

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
	window = glfwCreateWindow(1366, 768, "nigga", NULL, NULL);
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
	lightID = glGetUniformLocation(programID, "lightDir_worldspace");
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

glm::mat4 calculateVP() {
	getMatrices(window);
	glm::mat4 projection = getProjectionMatrix();
	glm::mat4 view = getViewMatrix();
	glUniformMatrix4fv(viewMatID, 1, GL_FALSE, &view[0][0]);
	return (glm::mat4) projection * view;
}

class Player{
    public:
        float trugPosx = 0, trugPosY = 0;
        float jumpSpeed = 2;
        float jumpSpeedBuffer = 0;
        float airTime = 0;
        bool onAir = false;
        void draw() {
            model = glm::translate(glm::mat4(1.0f), glm::vec3(trugPosx, trugPosY, 25));
			glBindVertexArray(car_VertexArrayID);
			MVP = VP * model;
			glUniform1i(textureID, 2);
			glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &model[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, car_vertices.size());
        }
        void handleTrug(){
            // jump logic memakai konsep percepatan gravitasi
            if(onAir){
                airTime += 0.04;
                trugPosY += jumpSpeedBuffer;
                if(jumpSpeed > (-jumpSpeedBuffer) && trugPosY > 0){
                    jumpSpeedBuffer = jumpSpeed - (gravity*airTime);
                } 
                else if(trugPosY <= 0) {
                    onAir = false; 
                    trugPosY = 0;
                }
                printf("trugPosY: %f\n", trugPosY);
                printf("jumpSpeedBuffer: %f\n", jumpSpeedBuffer);
                printf("airTime: %f\n", airTime);
                printf(onAir ? "onAir\n" : "notOnAir\n");
            }
			draw();
        }
        void jump(){
            if(!onAir){
                airTime = 0;
                onAir = true;
                jumpSpeedBuffer = jumpSpeed;
            }
        }
};

class Car{
	public :
		float x,y,z;
        GLint textid;
		Car(){
			x=0,y=0,z=0;
            x = -5 + (rand() % 3) * 5;
            textid = rand() % 3;
		}
		void draw(){
            model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
			glBindVertexArray(car_VertexArrayID);
			MVP = VP * model;
			glUniform1i(textureID, textid);
			glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &model[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, car_vertices.size());
		}
        void update(){
            z += WORLD_SPEED;
        }
};