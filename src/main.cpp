#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <glew.h>
#include <GLFW/glfw3.h>
#include "game.h"
#include "game.cpp"
using namespace glm;

bool game = true;
std::vector<Car> car(1, Car());
std::vector<Environment> environment(1, Environment(30));
std::vector<Tree> tree(1, Tree(14, -180));
Player player;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_UNKNOWN) return;
	if (game) {
		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
			if (player.x < 6.5) {
				player.move(1);
			}	
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
			if (player.x > -6.5) {
				player.move(-1);
			}	
		}
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
			player.jump();
		}
	}
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
		if (noclip == true) noclip = false;
		else noclip = true;
	}
}

void youLose() {
	WORLD_SPEED = 0.0f;
	CAR_SPEED = 0.0f;
	carspinc = 0.0f;
	game = false;
}

void init_environment(){
	lightDir = glm::vec3(10, 12, -8);
	glUniform3f(lightID, lightDir.x, lightDir.y, lightDir.z);
	environment.push_back(Environment(-40.0));
	environment.push_back(Environment(-110.0));
	environment.push_back(Environment(-180.0));
}

void renderCar(glm::mat4 model, GLuint varray_id) {
	glBindVertexArray(varray_id);
	MVP = VP * model;
	glUniform1i(textureID, 2);
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &model[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, car_vertices.size());
}

void handleCar() {
    for(auto it = car.begin(); it != car.end();){
            it->draw();
        if ((it -> x != player.x && it -> z < 10) || (it -> x == player.x && ((it -> z < -5.5 && !player.onAir) || player.onAir)) || game == false) {
            it->update();
            ++it;
        } else if (it -> z >= -5.5 && it -> z < 10 && !player.onAir) {
			youLose();
		} else {
			CAR_SPEED += carspinc;
            car.erase(it);
        }
    } if (frame % 20 == 0) {
		car.push_back(Car());
	}
}

void handleWorld(){
	for(auto it = environment.begin(); it != environment.end();){
            it->draw();
        if (it -> z < 100) {
            it->update();
            ++it;
        } else {
            it = environment.erase(it);
			environment.insert(it, Environment(-180.0));
        }
    }
}

void handleTree(){
	for(auto it = tree.begin(); it != tree.end();){
            it->draw();
        if (it -> z < 99.6) {
            it->update();
            ++it;
        } else {
            tree.erase(it);
        }
    }
	if (frame % 3 == 0) {
		tree.push_back(Tree(14, -180));
	}
}

void displayPaint() {
	if (game) {
		score += 1;
		printf("%d\n", score);
	}
	handleCar(); // Obstacles
	player.handleTrug(); // Player
	// Environment
	handleWorld();
	handleTree();


	// Ground
	glBindVertexArray(ground_VertexArrayID);
	glUniform1i(textureID, 12);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.1f, 0.0f));
	MVP = VP * model;
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &model[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, ground_vertices.size());
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	VP = calculateVP();
	displayPaint();
	glfwSwapBuffers(window);
	glfwPollEvents();
	frame++;
	frame = frame % 500;
}

int main() {
	glInit();
	glClearColor(0.973f, 0.314f, 0.072f, 1.0f);
	initShaders();
	loadTexture();
	loadOBJ();
	initVAO();
	init_environment();
	glfwSetKeyCallback(window, key_callback);
	while ((glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) 
			&& (glfwWindowShouldClose(window) == 0))
		display();
	flushBuffers();
	glfwTerminate();
	return 0;
}