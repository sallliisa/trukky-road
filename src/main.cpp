#include <stdio.h>
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
float carmov = 0;
float pinpo = 0.0f;

std::vector<Car> car(1, Car());

void renderCar(glm::mat4 model, GLuint varray_id){
	glBindVertexArray(varray_id);
	MVP = VP * model;
	glUniform1i(textureID, 2);
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &model[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, car_vertices.size());
}

void handleCar(){
    // std::vector<Obstacle>::iterator it = obstacles.begin();
    // for(int i = 0; i < obstacles.size();i++){
    //     if(obstacles[i].obsX > -10){
    //         obstacles[i].draw();
    //         obstacles[i].update();
            
    //     }
    //     else{
            
    //     }

    // }
    for(auto it = car.begin(); it != car.end();){
        if(it->z < 30){
            it->draw();
            it->update();
            ++it;
        }
        else{
            it = car.erase(it);
            car.insert(it, Car());
        }
    }
}

void displayPaint() {
	// glBindVertexArray(car_VertexArrayID);
	// model = glm::mat4(1.0f);
	// MVP = VP * model;
	// glUniform1i(textureID, 0);
	// glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
	// glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &model[0][0]);
	// glDrawArrays(GL_TRIANGLES, 0, car_vertices.size());

	handleCar();
	// model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
	// MVP = VP * model;
	// glUniform1i(textureID, 1);
	// glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
	// glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &model[0][0]);
	// glDrawArrays(GL_TRIANGLES, 0, car_vertices.size());

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