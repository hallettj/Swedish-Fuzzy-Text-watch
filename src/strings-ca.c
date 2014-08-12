#include "strings-ca.h"

const char* const MONTHS_CA[] = {
  // Jan = 0
  "January",
  "Ferbuary",
  "March",
  "April",
  "May",
  "June",
  "July",
  "August",
  "September",
  "October",
  "November",
  "December",
};

const char* const HOURS_CA[] = {
  // AM hours
  "dotze",
  "una",
  "dos",
  "tres",
  "quatre",
  "cinc",
  "sis",
  "set",
  "vuit",
  "nou",
  "deu",
  "onze",

  // PM hours
  "dotze",
  "una",
  "dos",
  "tres",
  "quatre",
  "cinc",
  "sis",
  "set",
  "vuit",
  "nou",
  "deu",
  "onze"
};

/**
 * The string "$1" will be replaced with the current hour (e.g., "three"
 * at 3:45).  The string "$2" will be replaced with the *next* hour
 * (e.g., "four" at 3:45).
 *
 * A "*" character before a word makes that word bold.
 */
const char* const RELS_CA[] = {
  "*$1 en punt",
  "*$1 y cinc",
  "*$1 y deu",
  "un quart de *$2",
  "un quart i cinc de *$2",
  "un quart i deu *$2",
  "dos quarts de *$2",
  "dos quarts i cinc de *$2",
  "dos quarts i deu de *$2",
  "tres quarts de *$2",
  "tres quarts i cinc de *$2",
  "tres quarts i deu de *$2"
};

