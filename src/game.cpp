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

GLuint  programID, matrixID, textureID, lightID, viewMatID, modelMatID, fogColorID,
	    texture0, texture1, texture2, texture3, texture4, texture5, 
		car_vertexbuffer, car_uvbuffer, car_normalbuffer, car_VertexArrayID,
		road_vertexbuffer, road_uvbuffer, road_normalbuffer, road_VertexArrayID,
		truck_vertexbuffer, truck_uvbuffer, truck_normalbuffer, truck_VertexArrayID,
		sideroad_vertexbuffer, sideroad_uvbuffer, sideroad_normalbuffer, sideroad_VertexArrayID;
std::vector<glm::vec3> car_vertices, car_normals, 
					   road_vertices, road_normals,
					   truck_vertices, truck_normals,
					   sideroad_vertices, sideroad_normals;
std::vector<glm::vec2> car_uvs,
					   road_uvs,
					   truck_uvs,
					   sideroad_uvs;
glm::vec3 lightDir;
glm::mat4 MVP, VP, model;
float WORLD_SPEED = 0.4;
float CAR_SPEED  = 0.6;
float jump1, jump2;
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
	texture3 = loadBMP("./res/tex/road.bmp");
	texture4 = loadBMP("./res/tex/truck.bmp");
	texture5 = loadBMP("./res/tex/sideroad.bmp");
	textureID = glGetUniformLocation(programID, "tex0Sampler");
	glBindTextureUnit(0, texture0);
	glBindTextureUnit(1, texture1);
	glBindTextureUnit(2, texture2);
	glBindTextureUnit(3, texture3);
	glBindTextureUnit(4, texture4);
	glBindTextureUnit(5, texture5);
	printf("loaded all texture\n");
}

void loadOBJ() {
	bool car = loadOBJ("./res/obj/car.obj", car_vertices, car_uvs, car_normals);
	bool road = loadOBJ("./res/obj/road.obj", road_vertices, road_uvs, road_normals);
	bool truck = loadOBJ("./res/obj/truck.obj", truck_vertices, truck_uvs, truck_normals);
	bool sideroad = loadOBJ("./res/obj/sideroad.obj", sideroad_vertices, sideroad_uvs, sideroad_normals);
}

void initVAO() {
	// Buffer generation
	// Car
	glGenBuffers(1, &car_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, car_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, car_vertices.size() * sizeof(glm::vec3), &car_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &car_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, car_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, car_uvs.size() * sizeof(glm::vec2), &car_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &car_normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, car_normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, car_normals.size() * sizeof(glm::vec3), &car_normals[0], GL_STATIC_DRAW);

	// Road
	glGenBuffers(1, &road_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, road_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, road_vertices.size() * sizeof(glm::vec3), &road_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &road_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, road_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, road_uvs.size() * sizeof(glm::vec2), &road_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &road_normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, road_normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, road_normals.size() * sizeof(glm::vec3), &road_normals[0], GL_STATIC_DRAW);

	// Truck
	glGenBuffers(1, &truck_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, truck_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, truck_vertices.size() * sizeof(glm::vec3), &truck_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &truck_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, truck_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, truck_uvs.size() * sizeof(glm::vec2), &truck_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &truck_normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, truck_normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, truck_normals.size() * sizeof(glm::vec3), &truck_normals[0], GL_STATIC_DRAW);

	// Sideroad
	glGenBuffers(1, &sideroad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, sideroad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sideroad_vertices.size() * sizeof(glm::vec3), &sideroad_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &sideroad_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, sideroad_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sideroad_uvs.size() * sizeof(glm::vec2), &sideroad_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &sideroad_normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, sideroad_normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, sideroad_normals.size() * sizeof(glm::vec3), &sideroad_normals[0], GL_STATIC_DRAW);

	// Gen VAO
	glGenVertexArrays(1, &car_VertexArrayID);
	glGenVertexArrays(1, &road_VertexArrayID);
	glGenVertexArrays(1, &truck_VertexArrayID);
	glGenVertexArrays(1, &sideroad_VertexArrayID);

	// VAO Binding
	// Car
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

	// Road
	glBindVertexArray(road_VertexArrayID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, road_vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, road_uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, road_normalbuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	// Truck
	glBindVertexArray(truck_VertexArrayID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, truck_vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, truck_uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, truck_normalbuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	// Sideroad
	glBindVertexArray(sideroad_VertexArrayID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, sideroad_vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, sideroad_uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, sideroad_normalbuffer);
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
        float x = 0, y = 0;
        float jumpSpeed = 1;
        float jumpSpeedBuffer = 0;
        float airTime = 0;
        bool onAir = false;
		bool onLane = true;
        void draw() {
			glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(this->x, this->y, 0));
            glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), 3.14159f, glm::vec3(0, 1, 0));
			model = translate * rotate;
			glBindVertexArray(truck_VertexArrayID);
			MVP = VP * model;
			glUniform1i(textureID, 4);
			glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &model[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, truck_vertices.size());
        }

        void handleTrug(){
            // jump logic memakai konsep percepatan gravitasi
			// TODO : airtime berbanding terbalik terhadap world speed
            if(onAir){
				jump2 = glfwGetTime();
				deltaTime = jump2 - jump1;
				y = -16 * deltaTime * (deltaTime - 1);
                // }
                if (this->y < 0) {
                    onAir = false; 
                    this -> y = 0;
                }
                printf("thisy: %f\n", this->y);
                printf("jumpSpeedBuffer: %f\n", jumpSpeedBuffer);
                printf("airTime: %f\n", airTime);
                printf(onAir ? "onAir\n" : "notOnAir\n");
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
};

class Car {
	public :
		float x,y,z;
        GLint texId;
		Car() {
			x = 0, y = 0, z = -60;
            x = -6.5 + (rand() % 3) * 6.5;
            texId = rand() % 3;
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
		float z;
		Environment(float m_z){
			this -> z = m_z;
		}
		void draw(){
			// Static objects
			// Road
			glBindVertexArray(road_VertexArrayID);
			glUniform1i(textureID, 3);
			model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, this->z-34));
			MVP = VP * model;
			glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &model[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, road_vertices.size());
			
			// Sideroad
			glBindVertexArray(sideroad_VertexArrayID);
			glUniform1i(textureID, 5);
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