#include "strings-nl.h"

const char* const HOURS_NL[] = {
  // AM hours
  "twaalf",
  "een",
  "twee",
  "drie",
  "vier",
  "vijf",
  "zes",
  "zeven",
  "acht",
  "negen",
  "tien",
  "elf",

  // PM hours
  "twaalf",
  "een",
  "twee",
  "drie",
  "vier",
  "vijf",
  "zes",
  "zeven",
  "acht",
  "negen",
  "tien",
  "elf"
};

/**
 * The string "$1" will be replaced with the current hour (e.g., "three"
 * at 3:45).  The string "$2" will be replaced with the *next* hour
 * (e.g., "four" at 3:45).
 *
 * A "*" character before a word makes that word bold.
 */
const char* const RELS_NL[] = {
  "*$1 uur",
  "vijf over *$1",
  "tien over *$1",
  "kwart over *$1",
  "tien voor half *$2",
  "vijf voor half *$2",
  "half *$2",
  "vijf over half *$2",
  "tien over half *$2",
  "kwart voor *$2",
  "tien voor *$2",
  "vijf voor *$2"
};

