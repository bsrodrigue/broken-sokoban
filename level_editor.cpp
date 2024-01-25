#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <raylib.h>

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 600

#define CELL_COUNT 10
#define CELL_SIZE (int)(WINDOW_HEIGHT / CELL_COUNT)

#define FPS 30

#define GRID_SIZE CELL_COUNT

//======== File Format =======//
#define LEVEL_NAME_OFFSET 0
#define LEVEL_NAME_SIZE 256
#define LEVEL_DATA_OFFSET (LEVEL_NAME_SIZE)
#define LEVEL_DATA_SIZE (GRID_SIZE * GRID_SIZE) * (sizeof(int))
#define LEVEL_FILE_SIZE (LEVEL_NAME_SIZE + LEVEL_DATA_SIZE)

void load_level_file(const char *name, int level[GRID_SIZE][GRID_SIZE]) {
  int data_size = LEVEL_DATA_SIZE;
  unsigned char *loaded_data = LoadFileData(name, &data_size);

  if (loaded_data == NULL) {
    return;
  }

  int *data_section = (int *)(loaded_data + LEVEL_DATA_OFFSET);

  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      level[i][j] = *data_section;
      data_section++;
    }
  }
}

void save_level_file(const char *name, int level[GRID_SIZE][GRID_SIZE]) {
  char *data = (char *)malloc(1024);

  strncpy((data + LEVEL_NAME_OFFSET), name, LEVEL_NAME_SIZE);

  int *data_section = (int *)(data + LEVEL_DATA_OFFSET);

  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      *data_section = level[i][j];
      data_section++;
    }
  }

  if (!SaveFileData(name, data, LEVEL_FILE_SIZE)) {
    TraceLog(LOG_ERROR, "Error while saving level data");
    exit(EXIT_FAILURE);
  }

  TraceLog(LOG_INFO, "Level data saved with success");

  free(data);
}

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
      if (code == 0)
        continue;
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

void hover() {
  Vector2 pos = GetMousePosition();

  int x = (int)(pos.x / CELL_SIZE);
  int y = (int)(pos.y / CELL_SIZE);

  if (level[y][x] == 0) {
    draw_cell(x, y, LIGHTGRAY);
  }
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
    save_level_file("level.dat", level);
    break;
  }
}

int main(int argc, char *argv[]) {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Broken Sokoban Level Editor");

  SetTargetFPS(FPS);

  load_level_file("level.dat", level);
  init_block_colors();

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(WHITE);

    int pressed_key = GetKeyPressed();

    handle_key_input(pressed_key);

    if (IsMouseButtonDown((MOUSE_BUTTON_LEFT))) {
      click(current_block_type);
    } else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      click(0);
    }

    render();
    hover();

    EndDrawing();
  }

  CloseWindow();
  return EXIT_SUCCESS;
}
