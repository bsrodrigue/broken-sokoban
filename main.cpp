#include <cstdint>
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>

//-------------[Broken Sokoban]----------------------------//

//-------------[Window]-------------//
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

#define WINDOW_TITLE "Broken Sokoban"

//-------------[Performance]-------------//
#define FPS 30

//-------------[Grid]-------------//
#define CELL_COUNT 30
#define CELL_SIZE WINDOW_WIDTH / CELL_COUNT

#define GET_INGAME_POS(ABS_POS) ABS_POS *CELL_SIZE

//-------------[Entities]-------------//

//-------------[RANDOM]-------------//
#define RANDOM_SEED 0xBFA

namespace grid {
void draw_cell(int x, int y, Color color = WHITE) {
  DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
}
} // namespace grid

Vector2 player_pos = {(float)CELL_COUNT / 2, CELL_COUNT - 5};

void draw_level(Vector2 pos, uint8_t shape[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      int cell = shape[i][j];

      if (cell != 0) {
        grid::draw_cell(pos.x + j, pos.y + i);
      }
    }
  }
}

bool check_out_of_bounds(Vector2 pos) { return false; }

void render() {}

void update_pos() {}

void get_player_input(int pressed_key) {}

void game_loop() {
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    int pressed_key = GetKeyPressed();
    get_player_input(pressed_key);

    render();
    EndDrawing();
  }
}

void init_game() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

  if (!IsWindowReady()) {
    exit(EXIT_FAILURE);
  }

  HideCursor();

  SetRandomSeed(RANDOM_SEED);

  SetTargetFPS(FPS);
}

void close_game() {}

int main(int argc, char *argv[]) {
  init_game();

  game_loop();

  close_game();
  return EXIT_SUCCESS;
}
