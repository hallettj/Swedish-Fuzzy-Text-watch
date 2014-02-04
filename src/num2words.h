#pragma once
#include "string.h"

typedef enum {
  DE = 0x0,
  EN = 0x1,
  ES = 0x2,
  FR = 0x3,
  NO = 0x4,
  SV = 0x5
} Language;

void time_to_words(Language lang, int hours, int minutes, int seconds, char* words, size_t length);
