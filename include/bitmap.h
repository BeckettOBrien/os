#pragma once

#define BITMAP_SIZE(n) (((n) + 7)/8)
#define BITMAP_GET(map, idx) ((map[idx/8] >> ((idx)%8)) & 1)
#define BITMAP_CLEAR(map, idx) map[idx/8] &= ~(1 << ((idx)%8))
#define BITMAP_SET(map, idx) map[idx/8] |= (1 << ((idx)%8))