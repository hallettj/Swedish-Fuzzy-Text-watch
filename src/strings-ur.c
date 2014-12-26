#include "strings-ur.h"

const char* const HOURS_UR[] = {
  // AM hours
  "bara",
  "aik",
  "do",
  "teen",
  "chaar",
  "paanch",
  "chhe",
  "saath",
  "aath",
  "nau",
  "das",
  "gyara",

  // PM hours
  "bara",
  "aik",
  "do",
  "teen",
  "chaar",
  "paanch",
  "chhe",
  "saath",
  "aath",
  "nau",
  "das",
  "gyara"
};

/**
 * The string "$1" will be replaced with the current hour (e.g., "three"
 * at 3:45).  The string "$2" will be replaced with the *next* hour
 * (e.g., "four" at 3:45).
 *
 * A "*" character before a word makes that word bold.
 */
const char* const RELS_UR[] = {
  "*$1",
  "*$1 baj ke paanch",
  "*$1 baj ke das",
  "sava *$1",
  "*$1 baj ke bees",
  "*$1 baj ke pachees",
  "sarhe *$1",
  "*$2 mein pachees",
  "*$2 mein bees",
  "paunay *$2",
  "*$2 mein das",
  "*$2 mein paanch"
};