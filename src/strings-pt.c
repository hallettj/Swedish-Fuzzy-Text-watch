#include "strings-pt.h"

const char* const HOURS_PT[] = {
  // AM hours
  "meia noite",
  "uma",
  "duas",
  "três",
  "quatro",
  "cinco",
  "seis",
  "sete",
  "oito",
  "nove",
  "dez",
  "onze",

  // PM hours
  "meio dia",
  "uma",
  "duas",
  "três",
  "quatro",
  "cinco",
  "seis",
  "sete",
  "oito",
  "nove",
  "dez",
  "onze"
};

/**
 * The string "$1" will be replaced with the current hour (e.g., "three"
 * at 3:45).  The string "$2" will be replaced with the *next* hour
 * (e.g., "four" at 3:45).
 *
 * A "*" character before a word makes that word bold.
 */
const char* const RELS_PT[] = {
  "*$1 em ponto",
  "*$1 e cinco",
  "*$1 e dez",
  "*$1 e quinze",
  "*$1 e vinte",
  "*$1 e vinte e cinco",
  "*$1 e meia",
  "*$1 e trinta e cinco",
  "vinte pras *$2",
  "quinze pras *$2",
  "dez pras *$2",
  "cinco pras *$2"
};