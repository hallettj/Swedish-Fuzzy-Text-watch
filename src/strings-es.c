#include "strings-es.h"

const char* const MONTHS_ES[] = {
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

const char* const HOURS_ES[] = {
  // AM hours
  "doce",
  "una",
  "dos",
  "tres",
  "cuatro",
  "cinco",
  "seis",
  "siete",
  "ocho",
  "nueve",
  "diez",
  "once",

  // PM hours
  "doce",
  "una",
  "dos",
  "tres",
  "cuatro",
  "cinco",
  "seis",
  "siete",
  "ocho",
  "nueve",
  "diez",
  "once"
};

/**
 * The string "$1" will be replaced with the current hour (e.g., "three"
 * at 3:45).  The string "$2" will be replaced with the *next* hour
 * (e.g., "four" at 3:45).
 *
 * A "*" character before a word makes that word bold.
 */
const char* const RELS_ES[] = {
  "*$1 en punto",
  "*$1 y cinco",
  "*$1 y diez",
  "*$1 y cuarto",
  "*$1 y veinte",
  "*$1 y veinti- cinco",
  "*$1 y media",
  "*$2 menos veinti- cinco",
  "*$2 menos veinte",
  "*$2 menos cuarto",
  "*$2 menos diez",
  "*$2 menos cinco"
};

