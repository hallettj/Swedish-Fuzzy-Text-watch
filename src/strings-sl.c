#include "strings-en_GB.h"

const char* const HOURS_SL[] = {
  // AM hours
  "polnoci",
  "ena",
  "dva",
  "tri",
  "stiri",
  "pet",
  "sest",
  "sedem",
  "osem",
  "devet",
  "deset",
  "enajst",

  // PM hours
  "poldne",
  "ena",
  "dva",
  "tri",
  "stiri",
  "pet",
  "sest",
  "sedem",
  "osem",
  "devet",
  "deset",
  "enajst",

  // special form for next hour
  "polnoci",
  "enih",
  "dveh",
  "treh",
  "stirih",
  "petih",
  "sestih",
  "sedmih",
  "osmih",
  "devetih",
  "desetih",
  "enajstih",

  "poldne",
  "enih",
  "dveh",
  "treh",
  "stirih",
  "petih",
  "sestih",
  "sedmih",
  "osmih",
  "devetih",
  "desetih",
  "enajstih",
};

/**
 * The string "$1" will be replaced with the current hour (e.g., "three"
 * at 3:45).  The string "$2" will be replaced with the *next* hour
 * (e.g., "four" at 3:45).
 *
 * A "*" character before a word makes that word bold.
 */
const char* const RELS_SL[] = {
  "*$1",
  "pet cez *$1",
  "deset cez *$1",
  "petnajst cez *$1",
  "dvajset cez *$1",
  "pet do pol *$2",
  "pol *$2",
  "pet cez pol *$2",
  "dvajset do *$2",
  "petnajst do *$2",
  "deset do *$2",
  "pet do *$2"
};

