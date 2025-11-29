#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

float majorRadius = 14;
float minorRadius = 6;
int n1 = 24, n2 = 8;

const char *dark = "\x1b[94m█\x1b[0m";
const char *light = "\x1b[96m█\x1b[0m";
const char *bg = " ";

float parametricIncrementValue = 0.01;
float K1 = 40;
float K2 = 40;

int height = 38, width = 150;
char buffer[150 * 38];
float zBuffer[150 * 38];

float calculateRotationX(float x, float y, float z, float A, float B, float C) {
  return y * sin(A) * sin(B) * cos(C) + z * cos(A) * sin(B) * cos(C) -
         y * cos(A) * sin(C) + z * sin(A) * sin(C) + x * cos(B) * cos(C);
}

float calculateRotationY(float x, float y, float z, float A, float B, float C) {
  return y * sin(A) * sin(B) * sin(C) + z * cos(A) * sin(B) * sin(C) +
         y * cos(A) * cos(C) - z * sin(A) * cos(C) + x * cos(B) * sin(C);
}

float calculateRotationZ(float x, float y, float z, float A, float B, float C) {
  return y * sin(A) * cos(B) + z * cos(A) * cos(B) - x * sin(B);
}

void calculateBuffer(float x, float y, float z, char ch) {
  float ooz = 1 / (z + 50);
  int xp = (int)(width / 2.0 + K1 * ooz * x * 2);
  int yp = (int)(height / 2.0 - K2 * ooz * y);

  int idx = xp + yp * width;
  if (idx >= 0 && idx < width * height) {
    if (zBuffer[idx] < ooz) {
      zBuffer[idx] = ooz;
      buffer[idx] = ch;
    }
  }
}

float t = 0;
float a = 0;
float f(float x) { return (15 - 5 * cosf(a)) * sinf(x * 2 * M_PI / 20 - t); }

int main() {
  while (1) {
    memset(zBuffer, 0, width * height * 4);
    memset(buffer, ' ', width * height);

    for (float x = -40; x < 40; x += parametricIncrementValue) {
      float y = f(x);
      float z = 0;

      calculateBuffer(x, y, z, '*');
    }

    printf("\x1b[H");
    for (int k = 0; k < width * height; k++) {
      putchar(k % width ? buffer[k] : '\n');
      //      printf("%s",
      //             k % width
      //                 ? (buffer[k] == '#' ? dark : (buffer[k] == '.' ? light
      //                 : bg)) : "\n");
    }
    t += 0.18;
    a += 0.1;
    usleep(33000);
  }

  return 0;
}
