#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <bits/stdc++.h>

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::mat4 getViewMatrix() {return ViewMatrix;}
glm::mat4 getProjectionMatrix() {return ProjectionMatrix;}
glm::vec3 position = glm::vec3(0, 0, 5);
float 
    horizontalA = 3.14f, 
    verticalA = 0.0f, 
    initFov = 45.0f, 
    movSpeed = 3.0f, 
    mouseSens = 0.05f;
int wWidth, wHeight;

void getMatrices(GLFWwindow* window) {
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    glfwGetWindowSize(window, &wWidth, &wHeight);
    float deltaTime = float(currentTime - lastTime);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    glfwSetCursorPos(window, wWidth/2, wHeight/2);

    horizontalA += mouseSens * deltaTime * float(wWidth/2 - xpos);
    verticalA += mouseSens * deltaTime * float(wHeight/2 - ypos);
    verticalA = std::clamp(verticalA, -1.5708f, 1.5708f);

    glm::vec3 direction(cos(verticalA) * sin(horizontalA), sin(verticalA), cos(verticalA) * cos(horizontalA));
    glm::vec3 right = glm::vec3(sin(horizontalA - 3.14f/2.0f), 0, cos(horizontalA - 3.14f/2.0f));
    glm::vec3 up = glm::cross(right, direction);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += direction * deltaTime * movSpeed;
    } if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= direction * deltaTime * movSpeed;
    } if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += right * deltaTime * movSpeed;
    } if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position -= right * deltaTime * movSpeed;
    } if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        movSpeed = 12.0f;
    } if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        movSpeed = 3.0f;
    }

    float fov = initFov;

    ProjectionMatrix = glm::perspective(glm::radians(fov), (float)wWidth/wHeight, 0.1f, 100.0f);
    ViewMatrix = glm::lookAt(position, position + direction, up);

    lastTime = currentTime;
}