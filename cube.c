#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int cubeWidth = 10;
int width = 100, height = 30;

float zBuffer[100 * 30];
char buffer[100 * 30];
float incrementSpeed = 0.1;
float K = 40;
char backgroundChar = ' ';
float cameraDistance = 60;

float x, y, z;
float A = 0, B = 0, C = 0;
int xp, yp;
float ooz;

float calculateX(int x, int y, int z) {
  return y * sin(A) * sin(B) * cos(C) + z * cos(A) * sin(B) * cos(C) -
         y * cos(A) * sin(C) + z * sin(A) * sin(C) + x * cos(B) * cos(C);
}

float calculateY(int x, int y, int z) {
  return y * sin(A) * sin(B) * sin(C) + z * cos(A) * sin(B) * sin(C) +
         y * cos(A) * cos(C) - z * sin(A) * cos(C) + x * cos(B) * sin(C);
}

float calculateZ(int x, int y, int z) {
  return y * sin(A) * cos(B) + z * cos(A) * cos(B) - x * sin(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, char ch) {
  x = calculateX(cubeX, cubeY, cubeZ);
  y = calculateY(cubeX, cubeY, cubeZ);
  z = calculateZ(cubeX, cubeY, cubeZ) + cameraDistance;

  ooz = 1 / z;

  xp = (int)(width / 2.0 + K * ooz * 2 * x);
  yp = (int)(height / 2.0 + K * ooz * y);

  int idx = xp + yp * width;
  if (idx >= 0 && idx < width * height) {
    if (zBuffer[idx] < ooz) {
      zBuffer[idx] = ooz;
      buffer[idx] = ch;
    }
  }
}

int main(int argc, char *argv[]) {
  printf("\x1b[2J");
  while (1) {
    memset(zBuffer, 0, width * height * 4);
    memset(buffer, backgroundChar, width * height);

    for (float cubeX = -cubeWidth; cubeX <= cubeWidth;
         cubeX += incrementSpeed) {
      for (float cubeY = -cubeWidth; cubeY <= cubeWidth;
           cubeY += incrementSpeed) {
        float cubeZ = -cubeWidth;
        calculateForSurface(cubeX, cubeY, cubeZ, '#');
        calculateForSurface(cubeZ, cubeY, -cubeX, '.');
        calculateForSurface(-cubeZ, cubeY, cubeX, '$');
        calculateForSurface(-cubeX, cubeY, -cubeZ, '/');
        calculateForSurface(cubeX, -cubeZ, cubeY, '?');
        calculateForSurface(cubeX, cubeZ, -cubeY, '%');
      }
    }

    printf("\x1b[H");

    for (int k = 0; k < width * height; k++) {
      putchar(k % width ? buffer[k] : '\n');
    }
    A += 0.05;
    B += 0.05;
    usleep(1000);
  }
  return 0;
}
