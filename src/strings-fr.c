#include "strings-fr.h"

const char* const MONTHS_FR[] = {
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

const char* const HOURS_FR[] = {
  // AM hours
  "douze",
  "une",
  "deux",
  "trois",
  "quatre",
  "cinq",
  "six",
  "sept",
  "huit",
  "neuf",
  "dix",
  "onze",

  // PM hours
  "douze",
  "une",
  "deux",
  "trois",
  "quatre",
  "cinq",
  "six",
  "sept",
  "huit",
  "neuf",
  "dix",
  "onze"
};

/**
 * The string "$1" will be replaced with the current hour (e.g., "three"
 * at 3:45).  The string "$2" will be replaced with the *next* hour
 * (e.g., "four" at 3:45).
 *
 * A "*" character before a word makes that word bold.
 */
const char* const RELS_FR[] = {
  "*$1 heures",
  "*$1 heures cinq",
  "*$1 heures dix",
  "*$1 heures et quart",
  "*$1 heures vingt",
  "*$1 heures vingt- cinq",
  "*$1 heures et demie",
  "*$2 moins vingt- cinq",
  "*$2 heures moins vingt",
  "*$2 moins le quart",
  "*$2 heures moins dix",
  "*$2 heures moins cinq"
};

// TODO: In all of the above strings "heures" should "heure" (singular)
// when the printed hour is "une".

// TODO: A couple of strings are shortened because the idiomatic
// versions would not fit on the screen:
// - "*$2 heures moins vingt-cinq" -> "*$2 moins vingt-cinq"
// - "*$2 heures moins le quart" -> "*$2 moins le quart"
