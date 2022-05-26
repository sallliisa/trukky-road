#include <stdio.h>
#include <stdlib.h>
#include <glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../common/texture.hpp"
#include "../common/objloader.hpp"

GLuint  texture0, texture1, texture2, texture3, texture4, texture5, texture6, texture7, texture8, texture9, texture10, texture11, texture12,
		car_vertexbuffer, car_uvbuffer, car_normalbuffer, car_VertexArrayID,
		road_vertexbuffer, road_uvbuffer, road_normalbuffer, road_VertexArrayID,
		truck_vertexbuffer, truck_uvbuffer, truck_normalbuffer, truck_VertexArrayID,
		sideroad_vertexbuffer, sideroad_uvbuffer, sideroad_normalbuffer, sideroad_VertexArrayID,
		tree1_vertexbuffer, tree1_uvbuffer, tree1_normalbuffer, tree1_VertexArrayID,
		tree2_vertexbuffer, tree2_uvbuffer, tree2_normalbuffer, tree2_VertexArrayID,
		tree3_vertexbuffer, tree3_uvbuffer, tree3_normalbuffer, tree3_VertexArrayID,
		ground_vertexbuffer, ground_uvbuffer, ground_normalbuffer, ground_VertexArrayID;

std::vector<glm::vec3> car_vertices, car_normals, 
					   road_vertices, road_normals,
					   truck_vertices, truck_normals,
					   sideroad_vertices, sideroad_normals,
					   tree1_vertices, tree1_normals,
					   tree2_vertices, tree2_normals,
					   tree3_vertices, tree3_normals,
					   ground_vertices, ground_normals;

std::vector<glm::vec2> car_uvs,
					   road_uvs,
					   truck_uvs,
					   sideroad_uvs,
					   tree1_uvs,
					   tree2_uvs,
					   tree3_uvs,
					   ground_uvs;

void loadTexture() {
	texture0 = loadBMP("./res/tex/car_1.bmp");
	texture1 = loadBMP("./res/tex/car_2.bmp");
	texture2 = loadBMP("./res/tex/car_3.bmp");
    texture3 = loadBMP("./res/tex/car_4.bmp");
    texture4 = loadBMP("./res/tex/car_5.bmp");
    texture5 = loadBMP("./res/tex/car_6.bmp");
	texture6 = loadBMP("./res/tex/road.bmp");
	texture7 = loadBMP("./res/tex/truck.bmp");
	texture8 = loadBMP("./res/tex/sideroad.bmp");
	texture9 = loadBMP("./res/tex/tree1.bmp");
	texture10 = loadBMP("./res/tex/tree2.bmp");
	texture11 = loadBMP("./res/tex/tree3.bmp");
	texture12 = loadBMP("./res/tex/ground.bmp");
	glBindTextureUnit(0, texture0);
	glBindTextureUnit(1, texture1);
	glBindTextureUnit(2, texture2);
	glBindTextureUnit(3, texture3);
	glBindTextureUnit(4, texture4);
	glBindTextureUnit(5, texture5);
	glBindTextureUnit(6, texture6);
	glBindTextureUnit(7, texture7);
	glBindTextureUnit(8, texture8);
    glBindTextureUnit(9, texture9);
    glBindTextureUnit(10, texture10);
    glBindTextureUnit(11, texture11);
	glBindTextureUnit(12, texture12);
}

void loadOBJ() {
	bool car = loadOBJ("./res/obj/car.obj", car_vertices, car_uvs, car_normals);
	bool road = loadOBJ("./res/obj/road.obj", road_vertices, road_uvs, road_normals);
	bool truck = loadOBJ("./res/obj/truck.obj", truck_vertices, truck_uvs, truck_normals);
	bool sideroad = loadOBJ("./res/obj/sideroad.obj", sideroad_vertices, sideroad_uvs, sideroad_normals);
	bool tree1 = loadOBJ("./res/obj/tree1.obj", tree1_vertices, tree1_uvs, tree1_normals);
	bool tree2 = loadOBJ("./res/obj/tree2.obj", tree2_vertices, tree2_uvs, tree2_normals);
	bool tree3 = loadOBJ("./res/obj/tree3.obj", tree3_vertices, tree3_uvs, tree3_normals);
	bool ground = loadOBJ("./res/obj/ground.obj", ground_vertices, ground_uvs, ground_normals);
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

	// Tree1
	glGenBuffers(1, &tree1_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tree1_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, tree1_vertices.size() * sizeof(glm::vec3), &tree1_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &tree1_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tree1_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, tree1_uvs.size() * sizeof(glm::vec2), &tree1_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &tree1_normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tree1_normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, tree1_normals.size() * sizeof(glm::vec3), &tree1_normals[0], GL_STATIC_DRAW);

	// Tree2
	glGenBuffers(1, &tree2_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tree2_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, tree2_vertices.size() * sizeof(glm::vec3), &tree2_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &tree2_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tree2_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, tree2_uvs.size() * sizeof(glm::vec2), &tree2_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &tree2_normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tree2_normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, tree2_normals.size() * sizeof(glm::vec3), &tree2_normals[0], GL_STATIC_DRAW);

	// Tree3
	glGenBuffers(1, &tree3_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tree3_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, tree3_vertices.size() * sizeof(glm::vec3), &tree3_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &tree3_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tree3_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, tree3_uvs.size() * sizeof(glm::vec2), &tree3_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &tree3_normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tree3_normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, tree3_normals.size() * sizeof(glm::vec3), &tree3_normals[0], GL_STATIC_DRAW);

	// Ground
	glGenBuffers(1, &ground_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ground_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, ground_vertices.size() * sizeof(glm::vec3), &ground_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ground_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ground_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, ground_uvs.size() * sizeof(glm::vec2), &ground_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ground_normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ground_normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, ground_normals.size() * sizeof(glm::vec3), &ground_normals[0], GL_STATIC_DRAW);

	// Gen VAO
	glGenVertexArrays(1, &car_VertexArrayID);
	glGenVertexArrays(1, &road_VertexArrayID);
	glGenVertexArrays(1, &truck_VertexArrayID);
	glGenVertexArrays(1, &sideroad_VertexArrayID);
	glGenVertexArrays(1, &tree1_VertexArrayID);
	glGenVertexArrays(1, &tree2_VertexArrayID);
	glGenVertexArrays(1, &tree3_VertexArrayID);
	glGenVertexArrays(1, &ground_VertexArrayID);

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

	// Tree1
	glBindVertexArray(tree1_VertexArrayID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, tree1_vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, tree1_uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, tree1_normalbuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	// Tree2
	glBindVertexArray(tree2_VertexArrayID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, tree2_vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, tree2_uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, tree2_normalbuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	// Tree3
	glBindVertexArray(tree3_VertexArrayID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, tree3_vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, tree3_uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, tree3_normalbuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	// Ground
	glBindVertexArray(ground_VertexArrayID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, ground_vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, ground_uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, ground_normalbuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}