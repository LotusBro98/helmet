#include <Adafruit_NeoPixel.h>

#define BRIGHTNESS 10
#define WIDTH 10
#define HEIGHT 30
#define SERIAL_TIMEOUT 1000

struct {
  Adafruit_NeoPixel strip;
  int offset;
  int length;
} 
LINES[] = {
  {{21, 12, NEO_GRB + NEO_KHZ800}, 4, 21},
  {{24, 11, NEO_GRB + NEO_KHZ800}, 3, 24},
  {{26, 10, NEO_GRB + NEO_KHZ800}, 2, 26},
  {{28, 9, NEO_GRB + NEO_KHZ800}, 1, 28},
  {{30, 8, NEO_GRB + NEO_KHZ800}, 0, 30},
  {{30, 3, NEO_GRB + NEO_KHZ800}, 0, 30},
  {{28, 4, NEO_GRB + NEO_KHZ800}, 1, 28},
  {{26, 5, NEO_GRB + NEO_KHZ800}, 2, 26},
  {{24, 6, NEO_GRB + NEO_KHZ800}, 3, 24},
  {{21, 7, NEO_GRB + NEO_KHZ800}, 4, 21}
};

uint8_t* image[WIDTH];

void setup() {
  for (int i = 0; i < (sizeof(LINES) / sizeof(LINES[0])); i++) {
    LINES[i].strip.begin();
    LINES[i].strip.setBrightness(BRIGHTNESS);
    LINES[i].strip.clear();
    LINES[i].strip.show();
    image[i] = LINES[i].strip.getPixels();
  }

  Serial.begin(115200);
}

void render_image() {
  for (int j = 0; j < (sizeof(LINES) / sizeof(LINES[0])); j++) {
    auto& line = LINES[j];
    // line.strip.clear();
    // for (int i = 0; i < line.length; i++) {
    //   uint8_t* pixel = image[i + line.offset][j];
    //   // uint8_t pixel[3] = {255, 0, 0};
    //   line.strip.setPixelColor(line.length - 1 - i, pixel[0], pixel[1], pixel[2]);
    // }
    line.strip.show();
  }
}

void read_image() {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      auto& line = LINES[j];
      uint8_t pixel[3];
      for (int k = 0; k < 3; k++) {
        for (int to = 0; to < SERIAL_TIMEOUT; to++) {
          if (Serial.available())
            break;
        }
        if (!Serial.available())  {
          return;
        }
        pixel[k] = Serial.read();
      }
      if (i < line.offset || i >= line.offset + line.length)
          continue;
      // line.strip.setPixelColor(line.length - 1 - i, pixel[0], pixel[1], pixel[2]);
      image[j][(line.length - 1 - i + line.offset) * 3 + 0] = pixel[1];
      image[j][(line.length - 1 - i + line.offset) * 3 + 1] = pixel[0];
      image[j][(line.length - 1 - i + line.offset) * 3 + 2] = pixel[2];
    }
  }
}

void loop() {
  while(!Serial.available());

  read_image();

  render_image();
}