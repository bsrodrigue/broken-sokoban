#include <cstdlib>
#include <raylib.h>

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 600

#define CELL_COUNT 15
#define CELL_SIZE (int)(WINDOW_HEIGHT / CELL_COUNT)

#define FPS 30

#define GRID_SIZE CELL_COUNT

enum BlockTypes {
  EMPTY,
  WALL,
  PLAYER,
  BOX,
  TARGET,
};

Color block_colors[5];

void init_block_colors() {
  block_colors[EMPTY] = WHITE;
  block_colors[WALL] = BLACK;
  block_colors[PLAYER] = GRAY;
  block_colors[BOX] = BLUE;
  block_colors[TARGET] = RED;
}

BlockTypes current_block_type = WALL;

int level[GRID_SIZE][GRID_SIZE];

void init_level(int arr[GRID_SIZE][GRID_SIZE], int value) {
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      arr[i][j] = value;
    }
  }
}

void load_level(int arr[GRID_SIZE][GRID_SIZE], int dest[GRID_SIZE][GRID_SIZE]) {
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      dest[i][j] = arr[i][j];
    }
  }
}

void draw_cell(float x, float y, Color color) {
  DrawRectangle((x * CELL_SIZE), (y * CELL_SIZE), CELL_SIZE, CELL_SIZE, color);
}

void draw_grid() {
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      int code = level[i][j];
      Color color = block_colors[code];
      draw_cell(j, i, color);
    }
  }
}

void click(int value) {
  Vector2 pos = GetMousePosition();

  int x = (int)(pos.x / CELL_SIZE);
  int y = (int)(pos.y / CELL_SIZE);

  level[y][x] = value;
}

void render() { draw_grid(); }

void handle_key_input(int pressed_key) {
  switch (pressed_key) {
  case KEY_Q:
    current_block_type = WALL;
    break;
  case KEY_A:
    current_block_type = PLAYER;
    break;
  case KEY_Z:
    current_block_type = TARGET;
    break;
  case KEY_W:
    current_block_type = BOX;
    break;
  case KEY_S:
    // Save
    if (!SaveFileData("level.dat", level, sizeof(level))) {
      TraceLog(LOG_ERROR, "Unable to save level.dat");
      return;
    }

    break;
  }
}

int main(int argc, char *argv[]) {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Broken Sokoban Level Editor");

  SetTargetFPS(FPS);

  if (FileExists("level.dat")) {
    TraceLog(LOG_INFO, "Load Level Data");

    int *dataSize;
    *dataSize = sizeof(level);

    void *data = (void *)LoadFileData("level.dat", dataSize);
  }

  init_level(level, 0);

  init_block_colors();

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    int pressed_key = GetKeyPressed();

    handle_key_input(pressed_key);

    if (IsMouseButtonDown((MOUSE_BUTTON_LEFT))) {
      click(current_block_type);
    } else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      click(0);
    }

    render();

    EndDrawing();
  }

  CloseWindow();
  return EXIT_SUCCESS;
}
