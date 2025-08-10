#include <FastLED.h>

#define LED_PIN       6
#define NUM_LEDS      256
#define MATRIX_WIDTH  16
#define MATRIX_HEIGHT 16

CRGB leds[NUM_LEDS];

#define BUTTON_UP    2
#define BUTTON_DOWN  3
#define BUTTON_LEFT  4
#define BUTTON_RIGHT 5
#define BUTTON_FIRE  7

int planeX = 6;
int planeY = 14;

bool obstacles[MATRIX_WIDTH][MATRIX_WIDTH/2] = {false};

int bulletX = -1;
int bulletY = -1;

struct ObstacleBullet {
  int x;
  int y;
};

ObstacleBullet obstacleBullets[11];

int obstacleNumbers = 0;

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_FIRE, INPUT_PULLUP);

  planeX = 6;
  planeY = 14;

  setRandomObstacles();
  initializeObstacleBullets();
  drawObstacles();
  drawPlane();
  FastLED.show();
}

void loop() {
  boolean flag = true;
  // checks if the game is over
   for (int i = 0; i < MATRIX_WIDTH; i++) {
        for (int j = 0; j < MATRIX_HEIGHT/2; j++) {
            if (obstacles[i][j] == true) {
                flag = false;
                break;
            }
        }
    }
  if (flag == true){
    FastLED.clear();
    blink_page(CRGB::Green);
    setup();
  } 
    
  if (digitalRead(BUTTON_UP) == LOW && planeY > MATRIX_HEIGHT/2) {
    planeY--;
    updatePlane();
  }
  if (digitalRead(BUTTON_DOWN) == LOW && planeY < MATRIX_HEIGHT - 2) {
    planeY++;
    updatePlane();
  }
  if (digitalRead(BUTTON_LEFT) == LOW && planeX > 1) {
    planeX--;
    updatePlane();
  }
  if (digitalRead(BUTTON_RIGHT) == LOW && planeX < MATRIX_WIDTH - 2) {
    planeX++;
    updatePlane();
  }
  if (digitalRead(BUTTON_FIRE) == LOW && bulletY == -1) {
    fireBullet();
  }

  updateBullet();
  updateObstacleBullets();
  randomObstacleFire();
  delay(180);
}

void updatePlane() {
  FastLED.clear();
  drawObstacles();
  drawPlane();
  drawBullet();
  drawObstacleBullets();
  FastLED.show();
}

void setRandomObstacles() {
  int count = 0;
  obstacleNumbers = random(8, 12);
  while (count < obstacleNumbers) {
    int x = random(1, MATRIX_WIDTH - 1); // not including first and last column
    int y = random(0, MATRIX_HEIGHT/2);              

    if (!obstacles[x][y]) { // for unque position of obstacles
      obstacles[x][y] = true;
      count++;
    }
  }
}

void initializeObstacleBullets() {
  for (int i = 0; i < obstacleNumbers; i++) {
    obstacleBullets[i] = {-1, -1};
  }
}

void drawObstacles() {
  for (int x = 1; x < MATRIX_WIDTH - 1; x++) {
    for (int y = 0; y < MATRIX_HEIGHT/2; y++) {
      if (obstacles[x][y]) {
        drawPixel(x, y, CRGB::LightSkyBlue);
      }
    }
  }
}

void drawPlane() {
  drawPixel(planeX, planeY, CRGB::Pink);
  drawPixel(planeX - 1, planeY + 1, CRGB::Pink);
  drawPixel(planeX, planeY + 1, CRGB::Pink);
  drawPixel(planeX + 1, planeY + 1, CRGB::Pink);
}

void fireBullet() {
  bulletX = planeX;
  bulletY = planeY;
}

void updateBullet() {
  if (bulletY >= 0) {
    // checks if bullet hits obstacle
    if (bulletY < MATRIX_HEIGHT/2 && obstacles[bulletX][bulletY]) {
      obstacles[bulletX][bulletY] = false;
      bulletX = -1;
      bulletY = -1;
    } else {
      bulletY--;
      if (bulletY < 0) {
        bulletX = -1;
      }
    }
    updatePlane();
  }
}

void drawBullet() {
  if (bulletY >= 0) {
    drawPixel(bulletX, bulletY, CRGB::White);
  }
}

void randomObstacleFire() {
  if (random(0, 10) < 2) {
    int obstacleIndex = random(0, obstacleNumbers);
    int count = 0;

    for (int x = 1; x < MATRIX_WIDTH - 1; x++) {
      for (int y = 0; y < MATRIX_HEIGHT/2; y++) {
        if (obstacles[x][y]) {
          if (count == obstacleIndex) {
            fireObstacleBullet(x, y);
            return;
          }
          count++;
        }
      }
    }
  }
}

void fireObstacleBullet(int x, int y) {
  for (int i = 0; i < obstacleNumbers; i++) {
    if (obstacleBullets[i].x == -1 && obstacleBullets[i].y == -1) {
      obstacleBullets[i] = {x, y + 1};
      return;
    }
  }
}

void updateObstacleBullets() {
  for (int i = 0; i < obstacleNumbers; i++) {
    if (obstacleBullets[i].x != -1 && obstacleBullets[i].y != -1) {
      obstacleBullets[i].y++;
      if (obstacleBullets[i].y >= MATRIX_HEIGHT) {
        obstacleBullets[i] = {-1, -1};
      } else if (
        (obstacleBullets[i].x == planeX && obstacleBullets[i].y == planeY) ||
        (obstacleBullets[i].x == planeX - 1 && obstacleBullets[i].y == planeY + 1) ||
        (obstacleBullets[i].x == planeX && obstacleBullets[i].y == planeY + 1) ||
        (obstacleBullets[i].x == planeX + 1 && obstacleBullets[i].y == planeY + 1)
      ) {
        gameOver();
      }
    }
  }
  updatePlane();
}

void gameOver(){
  for (int x = 1; x < MATRIX_WIDTH - 1; x++) {
    for (int y = 0; y < MATRIX_HEIGHT/2; y++) {
      obstacles[x][y] = false;
    }
  }
  blink_page(CRGB::Red);
  setup();
}

void blink_page(CRGB color){
  for(int i = 0; i<3; i++){
    FastLED.clear();
    fill_solid(leds, NUM_LEDS, color);
    FastLED.show();
    delay(100);
    FastLED.clear();
  }
  
}

void drawObstacleBullets() {
  for (int i = 0; i < obstacleNumbers; i++) {
    if (obstacleBullets[i].x != -1 && obstacleBullets[i].y != -1) {
      drawPixel(obstacleBullets[i].x, obstacleBullets[i].y, CRGB::Yellow);
    }
  }
}

void drawPixel(int x, int y, CRGB color) {
  if (x >= 0 && x < MATRIX_WIDTH/2 && y >= 0 && y < MATRIX_HEIGHT/2) {
    int index = y * MATRIX_WIDTH/2 + x;
    leds[index] = color;
  }
  else if (x >= MATRIX_WIDTH/2 && x<MATRIX_WIDTH && y >= 0 && y < MATRIX_HEIGHT/2){
    x = x - MATRIX_WIDTH/2;
    int index = NUM_LEDS/4 + y * MATRIX_WIDTH/2 + x ;
    leds[index] = color;
  }
  else if (x >= 0 && x < MATRIX_WIDTH/2 && y >= MATRIX_HEIGHT/2 && y < MATRIX_HEIGHT){
    y = y - MATRIX_HEIGHT/2;
    int index = NUM_LEDS/2 +  y * MATRIX_WIDTH/2 + x;
    leds[index] = color;
  }
  else if (x >= MATRIX_WIDTH/2 && x < MATRIX_WIDTH && y >= MATRIX_HEIGHT/2 && y < MATRIX_HEIGHT){
    x = x - MATRIX_WIDTH/2;
    y = y - MATRIX_HEIGHT/2;
    int index = 192 +  y * MATRIX_WIDTH/2 + x;
    leds[index] = color;
  }
}
