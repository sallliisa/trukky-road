#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <glew.h>
#include <GLFW/glfw3.h>
#include "game.h"
#include "game.cpp"
using namespace glm;
/* TODO: 
player
load object optimization
collision
 */

bool game = true;
std::vector<Car> car(1, Car());
std::vector<Environment> environment(1, Environment(30));

Player player;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_UNKNOWN) return;
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
		if (player.x < 6.5) {
			player.move(1);
			// printf("%d\n", player.x);
		}	
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
		if (player.x > -6.5) {
			player.move(-1);
			// printf("%d\n", player.x);
		}	
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
		player.jump();
	}
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
		if (noclip == true) noclip = false;
		else noclip = true;
	}
}

void youLose() {
	// game = false;
}

void init_environment(){
	// environment.push_back(Environment(-39.6));
	// environment.push_back(Environment(-109.2));
	// environment.push_back(Environment(-178.8));
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

void handleCar(){
    for(auto it = car.begin(); it != car.end();){
        if (((it -> x != player.x && it -> z < 10) || (it -> x == player.x && ((it -> z < -5.5 && !player.onAir) || player.onAir)))) {
            it->draw();
            it->update();
            ++it;
        } else if (it -> z >= -5.5 && it -> z < 10 && !player.onAir) {
			youLose();
			// printf("YOU LOSE");
			return;
		} else {
			// printf("sampe nigga");
			CAR_SPEED += 0.001;
			// CAR_SPEED = std::clamp(CAR_SPEED, 0.0f, 1.5f);
            car.erase(it);
			// break;
			// it = car.begin();
            // car.insert(it, Car());
        }
    }
	if (frame % 20 == 0) {
		car.push_back(Car());
	}
}

void handleWorld(){
	for(auto it = environment.begin(); it != environment.end();){
        if (it -> z < 100) {
            it->draw();
            it->update();
			// if(it->z == 10) printf("duar");
			// printf("%f\n", it->z);
            ++it;
			// printf("drew environment at %f\n", it -> z);
        } 
        else {
			WORLD_SPEED += 0.01;
			WORLD_SPEED = std::clamp(WORLD_SPEED, 0.0f, 1.0f);
			// printf("WS = %f | CS = %f\n", WORLD_SPEED, CAR_SPEED);
            it = environment.erase(it);
            // environment.insert(it, Environment(-178.8));
			environment.insert(it, Environment(-180.0));
        }
    }
}

void displayPaint() {
	// Interactive objects
	handleCar();
	player.handleTrug();
	handleWorld();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	VP = calculateVP();
	lightDir = glm::vec3(10, 12, -8);
	glUniform3f(lightID, lightDir.x, lightDir.y, lightDir.z);
	displayPaint();
	glfwSwapBuffers(window);
	glfwPollEvents();
	// printf("%d\n", frame % 100);
	frame++;
	frame = frame % 500;
}

int main() {
	glInit();
	float mult = 1.0f;
	glClearColor(mult*0.973f, mult*0.314f, mult*0.072f, 1.0f);
	initShaders();
	loadTexture();
	loadOBJ();
	initVAO();
	init_environment();
	glfwSetKeyCallback(window, key_callback);
	while ((glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) 
			&& (glfwWindowShouldClose(window) == 0) && game)
		display();
	flushBuffers();
	glDeleteTextures(1, &texture0);
	glDeleteTextures(1, &texture1);
	glfwTerminate();
	return 0;
}