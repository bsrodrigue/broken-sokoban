#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <raylib.h>
#include <raymath.h>
#include <vector>

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

enum BlockTypes {
  EMPTY,
  WALL,
  PLAYER,
  BOX,
  TARGET,
};

Color block_colors[5];

std::vector<Vector2> walls;
Vector2 player;
Vector2 box;
Vector2 target;

void init_block_colors() {
  block_colors[EMPTY] = WHITE;
  block_colors[WALL] = BLACK;
  block_colors[PLAYER] = GRAY;
  block_colors[BOX] = BLUE;
  block_colors[TARGET] = RED;
}

int level[GRID_SIZE][GRID_SIZE];

void draw_cell(float x, float y, Color color) {
  DrawRectangle((x * CELL_SIZE), (y * CELL_SIZE), CELL_SIZE, CELL_SIZE, color);
}

void draw_walls() {
  for (int i = 0; i < walls.size(); i++) {
    Vector2 wall = walls[i];
    draw_cell(wall.x, wall.y, block_colors[WALL]);
  }
}

void draw_level() {
  draw_walls();

  draw_cell(player.x, player.y, block_colors[PLAYER]);
  draw_cell(box.x, box.y, block_colors[BOX]);
  draw_cell(target.x, target.y, block_colors[TARGET]);
}

void init_positions() {
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      float x = j;
      float y = i;
      int block_code = level[i][j];
      switch (block_code) {
      case EMPTY:
        continue;
        break;
      case WALL:
        walls.push_back({x, y});
        break;
      case PLAYER:
        player = {x, y};
        break;
      case BOX:
        box = {x, y};
        break;
      case TARGET:
        target = {x, y};
        break;
      }
    }
  }
}

void render() { draw_level(); }

bool check_collision_with_walls(Vector2 next_pos) {
  for (int i = 0; i < walls.size(); i++) {
    Vector2 wall = walls[i];
    if (Vector2Equals(wall, next_pos))
      return true;
  }

  return false;
}

bool check_collision_with_box(Vector2 next_pos) {
  return Vector2Equals(box, next_pos);
}

bool check_collision_with_bounds(Vector2 next_pos) {
  return (next_pos.x < 0 || next_pos.x > CELL_COUNT - 1 || next_pos.y < 0 ||
          next_pos.y > CELL_COUNT - 1);
}

bool can_move_there(Vector2 next_pos) {
  return !check_collision_with_walls(next_pos) &&
         !check_collision_with_bounds(next_pos);
}

bool check_reached_target(Vector2 next_pos) {
  return Vector2Equals(target, next_pos);
}

void handle_input(int pressed_key) {
  Vector2 next = {0, 0};
  Vector2 next_box = {0, 0};
  switch (pressed_key) {
  case KEY_UP:
    next = Vector2Add(player, {0, -1});
    next_box = Vector2Add(box, {0, -1});
    break;
  case KEY_DOWN:
    next = Vector2Add(player, {0, 1});
    next_box = Vector2Add(box, {0, 1});
    break;
  case KEY_LEFT:
    next = Vector2Add(player, {-1, 0});
    next_box = Vector2Add(box, {-1, 0});
    break;
  case KEY_RIGHT:
    next = Vector2Add(player, {1, 0});
    next_box = Vector2Add(box, {1, 0});
    break;
  }

  if (Vector2Equals(next, {0, 0}))
    return;

  if (!can_move_there(next))
    return;

  if (!check_collision_with_box(next)) {
    player = next;
    return;
  }

  if (can_move_there(next_box)) {
    if (check_reached_target(next_box)) {
      CloseWindow();
    }
    box = next_box;
    player = next;
  }
}

int main(int argc, char *argv[]) {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Broken Sokoban");

  SetTargetFPS(FPS);

  load_level_file("level.dat", level);

  init_block_colors();

  init_positions();

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(WHITE);

    int pressed_key = GetKeyPressed();

    handle_input(pressed_key);

    render();

    EndDrawing();
  }

  CloseWindow();
  return EXIT_SUCCESS;
}
