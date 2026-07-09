#ifndef CONSTANTS_H
#define CONSTANTS_H

const int   WIN_W     = 800;
const int   WIN_H     = 600;
const int   N_PARTS   = 20000;
const int   N_TS      = 4.0;
const int   N_THREADS = 4;
const float G_FORCE   = 0.098;

constexpr float PART_R    = 3;
constexpr int   CELL_W    = PART_R * 4;
constexpr int   GRID_ROWS = WIN_H / CELL_W;
constexpr int   GRID_COLS = WIN_W / CELL_W;

#endif
