#include <iostream>
#include <raylib.h>

int score1 = 0;
int score2 = 0;

class Ball {
public:
  float x; float y;
  float radius;
  int v_x = 8, v_y = 8;

  Ball(int x, int y, int radius) {
    this->x = x;
    this->y = y;
    this->radius = radius;
  }

  void draw() { DrawCircle(x, y, radius, WHITE); }

  void update() {
    if (x + radius >= GetScreenWidth()) {
      v_x = -v_x;
      score1++;
    }
    if (x - radius <= 0) {
      v_x = -v_x;
      score2++;
    }
    if (y + radius >= GetScreenHeight() || y - radius <= 0) {
      v_y = -v_y;
    }
    x = x + v_x;
    y = y + v_y;
  }
};

class Bar {
public:
  float x, y;
  float height, width;
  const int speed = 5;
  int up_key;
  int down_key;

  Bar(int x, int y, int width, int height, int up_key, int down_key) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->up_key = up_key;
    this->down_key = down_key;
  }

  void draw() { 
    DrawRectangleRounded( Rectangle{x, y, width, height}, 1, 10, WHITE);
  }

  void update() {
    if (IsKeyDown(up_key) && y >= 5) {
      if (y - speed >= 5) {
        y = y - speed;
      } else {
        y = 5;
      }
    }
    if (IsKeyDown(down_key) && y + height <= GetScreenHeight() - 5) {
      if (y + speed <= GetScreenHeight() - height - 5) {
        y = y + speed;
      } else {
        y = GetScreenHeight() - height - 5;
      }
    }
  }
};

void drawScore(void) {
    DrawText(TextFormat("%i", score1), GetScreenWidth() / 4 - 40/2, 40, 40, WHITE);
    DrawText(TextFormat("%i", score2), 3 * GetScreenWidth() / 4 - 40/2, 40, 40, WHITE);
}

void checkCollisionsBallBar(Ball& ball, Bar& bar1, Bar& bar2) {
  if (CheckCollisionCircleRec(
            Vector2{(float)ball.x, (float)ball.y}, ball.radius,
            Rectangle{(float)bar1.x, (float)bar1.y, (float)bar1.width,
                      (float)bar1.height})) {
      if (ball.x - ball.radius <= bar1.x + bar1.width) {
        ball.x = bar1.x + bar1.width + ball.radius;
      }
      ball.v_x = -ball.v_x;
    }

    if (CheckCollisionCircleRec(
            Vector2{(float)ball.x, (float)ball.y}, ball.radius,
            Rectangle{(float)bar2.x, (float)bar2.y, (float)bar2.width,
                      (float)bar2.height})) {
      if (ball.x + ball.radius >= GetScreenWidth() - bar2.width) {
        ball.x = bar2.x - bar2.width - ball.radius;
      }
      ball.v_x = -ball.v_x;
    }
}

void drawField(void) {
  float center_x = GetScreenWidth() / 2.0f;
  float center_y = GetScreenHeight() / 2.0f;
  ClearBackground(DARKGREEN);
  DrawRectangle(center_x, 0, 2 * center_x, 2 * center_y, LIME);
  DrawCircle(center_x, center_y, 75, LIME);
  DrawCircleSector(Vector2 {center_x, center_y}, 75, 90, -90, 80, DARKGREEN);
  DrawLine(center_x, 0, center_x,2 * center_y, WHITE);
}

int main() {
  std::cout << "Hello World!" << std::endl;

  const int width = 640;
  const int height = 420;

  InitWindow(width, height, "Pong");
  SetTargetFPS(60);

  Ball ball = Ball(width / 2, height / 2, 8);

  int barWidth = 15;
  int barHeight = 80;
  int barDisplacement = 5;
  Bar bar1 = Bar(barDisplacement, height / 2 - barHeight / 2, barWidth,
                 barHeight, KEY_UP, KEY_DOWN);
  Bar bar2 = Bar(width - barDisplacement - barWidth, height / 2 - barHeight / 2,
                 barWidth, barHeight, KEY_K, KEY_J);

  while (WindowShouldClose() == false) {
    BeginDrawing();
      // draw field
      drawField();

      // draw and update ball end bars
      ball.draw();
      ball.update();

      bar1.update();
      bar1.draw();

      bar2.update();
      bar2.draw();

      // check for collisions
      checkCollisionsBallBar(ball, bar1, bar2);

      // draw score
      drawScore();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}