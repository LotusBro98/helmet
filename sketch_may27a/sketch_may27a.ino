#include <Adafruit_NeoPixel.h>

#define BRIGHTNESS 10
#define WIDTH 10
#define HEIGHT 30
#define SERIAL_TIMEOUT 10000

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
      line.strip.setPixelColor(line.length - 1 - i + line.offset, pixel[0], pixel[1], pixel[2]);
    }
  }
}

float func(float x, float t) {
  t = t / 10;
  float y1 = sin(t);
  float y2 = 0.7 * sin(2 * t + 0.6);
  float y3 = 0.4 * sin(3 * t - 0.5);
  float y4 = 0.3 * sin(4 * t + 0.45);
  float y5 = 0.2 * sin(5 * t - 0.134);
  float m = 1 + y1 + y2 + y3 + y4 + y5;

  float mx = 0.125 + 2 * sin(x) + 0.1 * sin(2*x - 0.08);

  return m * x * exp(-1*mx*mx);
}

void generate_image(float t) {
  for (int j = 0; j < WIDTH; j++) {
    auto & line = LINES[j];
    line.strip.clear();
    float x = ((j + 0.5) / WIDTH) * 2 - 1;
    float y = func(x, t);
    y = (0.5 * y + 0.5) * HEIGHT;
    if (y < 0) y = 0;
    if (y > HEIGHT - 1) y = HEIGHT - 1;
    int i = y;
    float f = y - i;
    line.strip.setPixelColor(i - line.offset, (int)((1 - f) * 255), 0, 0);
    if (i < HEIGHT - 1) {
      line.strip.setPixelColor(i + 1 - line.offset, (int)(f * 255), 0, 0);
    }
  }
}

int t = 1;

void loop() {
  // Serial.write('\n');
  // while(!Serial.available());
  // read_image();

  generate_image((float) t);
  t += 1;

  render_image();
}