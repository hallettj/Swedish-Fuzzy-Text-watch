#include "strings-en.h"

const char* const HOURS_NO[] = {
  // AM hours
  "tolv",
  "ett",
  "to",
  "tre",
  "fire",
  "fem",
  "seks",
  "sju",
  "åtte",
  "ni",
  "ti",
  "elleve",

  // PM hours
  "tolv",
  "ett",
  "to",
  "tre",
  "fire",
  "fem",
  "seks",
  "sju",
  "åtte",
  "ni",
  "ti",
  "elleve"
};

/**
 * The string "$1" will be replaced with the current hour (e.g., "three"
 * at 3:45).  The string "$2" will be replaced with the *next* hour
 * (e.g., "four" at 3:45).
 *
 * A "*" character before a word makes that word bold.
 */
const char* const RELS_NO[] = {
  "klokka er *$1",
  "fem over *$1",
  "ti over *$1",
  "kvart over *$1",
  "ti på halv *$2",
  "fem på halv *$2",
  "halv *$2",
  "fem over halv *$2",
  "ti over halv *$2",
  "kvart på *$2",
  "ti på *$2",
  "fem på *$2"
};

