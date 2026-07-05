#ifndef CONSTANTS_H
#define CONSTANTS_H

const int   WIN_W     = 800;
const int   WIN_H     = 600;
const int   CELL_W    = 20;
const int   N_PARTS   = 10000;
const int   N_TS      = 2.0;
const int   N_THREADS = 5;
const float PART_R    = 5;
const float G_FORCE   = 0.098;

constexpr int GRID_ROWS = WIN_H / CELL_W;
constexpr int GRID_COLS = WIN_W / CELL_W;

#endif
