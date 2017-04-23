#include "strings-de.h"

const char* const HOURS_DE[] = {
  // AM hours
  "twölf",
  "een",
  "twee",
  "dree",
  "veer",
  "fief",
  "söss",
  "söven",
  "acht",
  "negen",
  "teihn",
  "ölven",

  // PM hours
  "twölf",
  "een",
  "twee",
  "dree",
  "veer",
  "fief",
  "söss",
  "söven",
  "acht",
  "negen",
  "teihn",
  "ölven"
};

/**
 * The string "$1" will be replaced with the current hour (e.g., "three"
 * at 3:45).  The string "$2" will be replaced with the *next* hour
 * (e.g., "four" at 3:45).
 *
 * A "*" character before a word makes that word bold.
 */
const char* const RELS_DE[] = {
  "Klock *$1",
  "fief na *$1",
  "teihn na *$1",
  "viddel na *$1",
  "twintig na *$1",
  "fief vör halfig *$2",
  "halvig *$2",
  "fief na halvig *$2",
  "twintig vör *$2",
  "viddel vör *$2",
  "teihn vör *$2",
  "fief vör *$2"
};

