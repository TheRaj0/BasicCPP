#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

float majorRadius = 14;
float minorRadius = 6;
const int n1 = 16, n2 = 6;

const char *dark = "\x1b[94m█\x1b[0m";
const char *light = "\x1b[96m█\x1b[0m";
const char *bg = " ";

float parametricIncrementValueU = 2 * M_PI / 500;
float parametricIncrementValueV = 2 * M_PI / 500;
float K1 = 240;
float K2 = 100;

int height = 97, width = 420;
char buffer[97 * 420];
float zBuffer[97 * 420];

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
  float ooz = 1 / (z + K2);
  int xp = (int)(width / 2.0 + K1 * ooz * x * 2);
  int yp = (int)(height / 2.0 + K1 * ooz * y);

  int idx = xp + yp * width;
  if (idx >= 0 && idx < width * height) {
    if (zBuffer[idx] < ooz) {
      zBuffer[idx] = ooz;
      buffer[idx] = ch;
    }
  }
}

char calculateChar(float u, float v) {
  int c = ((int)(u / (2 * M_PI / n1))) % 2 == 0;
  c = ((int)(v / (2 * M_PI / n2))) % 2 == 0 ? c : !c;

  return c ? '#' : '.';
}

int main() {
  float A = 0, B = 0, C = 0;
  float x, y, z, rotated_x, rotated_y, rotated_z;

  while (1) {
    memset(zBuffer, 0, width * height * 4);
    memset(buffer, ' ', width * height);

    for (float u = 0; u < 2 * M_PI; u += parametricIncrementValueU) {
      for (float v = 0; v < 2 * M_PI; v += parametricIncrementValueV) {
        x = (majorRadius + minorRadius * cosf(v)) * cosf(u);
        y = (majorRadius + minorRadius * cosf(v)) * sinf(u);
        z = minorRadius * sinf(v);

        rotated_x = calculateRotationX(x, y, z, A, B, C);
        rotated_y = calculateRotationY(x, y, z, A, B, C);
        rotated_z = calculateRotationZ(x, y, z, A, B, C);

        char shade = calculateChar(u, v);

        calculateBuffer(rotated_x, rotated_y, rotated_z, shade);
      }
    }

    printf("\x1b[H");
    for (int k = 0; k < width * height; k++) {
      // putchar(k % width ? buffer[k] : '\n');
      printf("%s",
             k % width
                 ? (buffer[k] == '#' ? dark : (buffer[k] == '.' ? light : bg))
                 : "\n");
    }
    A += 0.08;
    B += 0.04;
    C += 0.005;
    usleep(30000);
  }

  return 0;
}
