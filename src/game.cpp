#include <stdio.h>
#include <stdlib.h>
#include <glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../common/shader.hpp"
#include "../common/controls.hpp"
#include "game.h"
#include "gameobjects.cpp"

GLFWwindow* window;

GLuint  programID, matrixID, textureID, lightID, viewMatID, modelMatID;

glm::vec3 lightDir;
glm::mat4 MVP, VP, model;
float WORLD_SPEED = 0.1;
float CAR_SPEED  = 0.1;
float jump1, jump2, move1, move2;
int wheel_direction = 0, frame = 0;
float gravity = 10;

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
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_BLEND);
}

void flushBuffers() {
	glDeleteBuffers(1, &car_vertexbuffer);
	glDeleteBuffers(1, &car_normalbuffer);
	glDeleteBuffers(1, &car_uvbuffer);
	glDeleteVertexArrays(1, &car_VertexArrayID);
	glDeleteBuffers(1, &road_vertexbuffer);
	glDeleteBuffers(1, &road_normalbuffer);
	glDeleteBuffers(1, &road_uvbuffer);
	glDeleteVertexArrays(1, &road_VertexArrayID);
	glDeleteBuffers(1, &truck_vertexbuffer);
	glDeleteBuffers(1, &truck_normalbuffer);
	glDeleteBuffers(1, &truck_uvbuffer);
	glDeleteVertexArrays(1, &truck_VertexArrayID);
	glDeleteBuffers(1, &sideroad_vertexbuffer);
	glDeleteBuffers(1, &sideroad_normalbuffer);
	glDeleteBuffers(1, &sideroad_uvbuffer);
	glDeleteVertexArrays(1, &sideroad_VertexArrayID);
	glDeleteBuffers(1, &tree1_vertexbuffer);
	glDeleteBuffers(1, &tree1_normalbuffer);
	glDeleteBuffers(1, &tree1_uvbuffer);
	glDeleteVertexArrays(1, &tree1_VertexArrayID);
	glDeleteBuffers(1, &tree2_vertexbuffer);
	glDeleteBuffers(1, &tree2_normalbuffer);
	glDeleteBuffers(1, &tree2_uvbuffer);
	glDeleteVertexArrays(1, &tree2_VertexArrayID);
	glDeleteBuffers(1, &tree3_vertexbuffer);
	glDeleteBuffers(1, &tree3_normalbuffer);
	glDeleteBuffers(1, &tree3_uvbuffer);
	glDeleteVertexArrays(1, &tree3_VertexArrayID);
	glDeleteProgram(programID);
}

void initShaders() {
	programID = LoadShaders("./res/shaders/vertexshader.glsl", "./res/shaders/fragmentshader.glsl");
	matrixID = glGetUniformLocation(programID, "MVP");
	viewMatID = glGetUniformLocation(programID, "V");
	modelMatID = glGetUniformLocation(programID, "M");
	lightID = glGetUniformLocation(programID, "lightDir_worldspace");
	textureID = glGetUniformLocation(programID, "tex0Sampler");
	glUseProgram(programID);
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
        float x = 0, y = 0;
        float jumpSpeed = 1;
        float jumpSpeedBuffer = 0;
        float airTime = 0;
        bool onAir = false;
		bool onLane = true;
		int playerLane = 0;
        void draw() {
			glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(this->x, this->y, 0));
            glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), 3.14159f, glm::vec3(0, 1, 0));
			model = translate * rotate;
			glBindVertexArray(truck_VertexArrayID);
			MVP = VP * model;
			glUniform1i(textureID, 7);
			glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &model[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, truck_vertices.size());
        }

        void handleTrug(){
            // jump logic memakai konsep percepatan gravitasi
			// TODO : airtime berbanding terbalik terhadap world speed
            if(onAir) {
				jump2 = glfwGetTime();
				deltaTime = jump2 - jump1;
				y = -16 * deltaTime * (deltaTime - 1);
                if (this->y < 0) {
                    onAir = false; 
                    this -> y = 0;
                }
                // printf("thisy: %f\n", this->y);
                // printf("jumpSpeedBuffer: %f\n", jumpSpeedBuffer);
                // printf("airTime: %f\n", airTime);
                // printf(onAir ? "onAir\n" : "notOnAir\n");
            }
			// printf("truck x = %f\n", this -> x);
			if (!onLane) {
				// printf("not on lane");
				move2 = glfwGetTime();
				deltaTime = move2 - move1;
				x += wheel_direction * ((1 * deltaTime) + (0.5 * 1 * deltaTime * deltaTime));
				if (this->x < -6.5) {
					playerLane = -1;
					onLane = true;
					this -> x = -6.5;
				} else if (this -> x > 6.5) {
					playerLane = -1;
					onLane = true;
					this -> x = 6.5;
				} else if ((this -> x > -0.2 && this -> x < 0.2) && playerLane != 0) {
					playerLane = 0;
					onLane = true;
					this -> x = 0;
				}
			}
			draw();
        }

        void jump() {
            if (!onAir) {
				jump1 = glfwGetTime();
                airTime = 0;
                onAir = true;
                jumpSpeedBuffer = jumpSpeed;
            }
        }

		void move(int direction) {
			if (onLane) {
				onLane = false;
				move1 = glfwGetTime();
				wheel_direction = direction;
			}
		}
};

class Car {
	public :
		float x,y,z;
        GLint texId;
		Car() {
			x = 0, y = 0, z = -180;
            x = -6.5 + (rand() % 3) * 6.5;
            texId = rand() % 6;
		}
		void draw() {
            model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
			glBindVertexArray(car_VertexArrayID);
			MVP = VP * model;
			glUniform1i(textureID, texId);
			glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &model[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, car_vertices.size());
		}
        void update() {
            z += CAR_SPEED;
        }
};

class Environment{
	public:
		double z;
		Environment(double m_z){
			this -> z = m_z;
		}
		void draw(){
			// Static objects
			// Road
			glBindVertexArray(road_VertexArrayID);
			glUniform1i(textureID, 6);
			model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, this->z-34));
			MVP = VP * model;
			glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &model[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, road_vertices.size());
			
			// Sideroad
			glBindVertexArray(sideroad_VertexArrayID);
			glUniform1i(textureID, 8);
			model = glm::translate(glm::mat4(1.0f), glm::vec3(-12.0f, 0.0f, this->z));
			MVP = VP * model;
			glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &model[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, sideroad_vertices.size());
			model = glm::translate(glm::mat4(1.0f), glm::vec3(12.0f, 0.0f, this->z));
			MVP = VP * model;
			glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &model[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, sideroad_vertices.size());
		}
		void update(){
			z += WORLD_SPEED;
		}
};