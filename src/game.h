#include <stdbool.h>
#ifndef GAME_H
#define GAME_H
#define PI 3.14159265358979323846

void glInit();
void initShaders();
void loadTexture();
void renderTruckDeco();
void loadOBJ();
void initVAO();
class Player;

class Obstacle;

#endif