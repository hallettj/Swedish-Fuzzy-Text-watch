#include "strings-sv.h"

const char* const MONTHS_SV[] = {
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

const char* const HOURS_SV[] = {
  // AM hours
  "tolv",
  "ett",
  "två",
  "tre",
  "fyra",
  "fem",
  "sex",
  "sju",
  "åtta",
  "nio",
  "tio",
  "elva",

  // PM hours
  "tolv",
  "ett",
  "två",
  "tre",
  "fyra",
  "fem",
  "sex",
  "sju",
  "åtta",
  "nio",
  "tio",
  "elva"
};

/**
 * The string "$1" will be replaced with the current hour (e.g., "three"
 * at 3:45).  The string "$2" will be replaced with the *next* hour
 * (e.g., "four" at 3:45).
 *
 * A "*" character before a word makes that word bold.
 */
const char* const RELS_SV[] = {
  "$1",
  "fem över *$1",
  "tio över *$1",
  "kvart över *$1",
  "tjugo över *$1",
  "fem i halv *$2",
  "halv *$2",
  "fem över halv *$2",
  "tjugo i *$2",
  "kvart i *$2",
  "tio i *$2",
  "fem i *$2"
};
