#ifndef PASSPORT_DEFINITIONS_H
#define PASSPORT_DEFINITIONS_H

#define KEY_VALUE_PAIR_DELIMITERS " \n"
#define KEY_VALUE_DELIMITER ":"
#define NUM_PASSPORT_FIELDS 8

#define BIRTH_YEAR_INDEX 0
#define ISSUE_YEAR_INDEX 1
#define EXPIRATION_YEAR_INDEX 2
#define HEIGHT_INDEX 3
#define HAIR_COLOR_INDEX 4
#define EYE_COLOR_INDEX 5
#define PASSPORT_ID_INDEX 6
#define COUNTRY_ID_INDEX 7

#define BIRTH_YEAR_MAX 2002
#define BIRTH_YEAR_MIN 1920

#define ISSUE_YEAR_MAX 2020
#define ISSUE_YEAR_MIN 2010

#define EXPIRATION_YEAR_MAX 2030
#define EXPIRATION_YEAR_MIN 2020

#define HEIGHT_UNIT_STRING_LENGHT 2
#define HEIGHT_UNIT_CM "cm"
#define HEIGHT_UNIT_IN "in"
#define HEIGHT_VALUE_BUFFER 10
#define HEIGHT_CM_MAX 193
#define HEIGHT_CM_MIN 150
#define HEIGHT_IN_MAX 76
#define HEIGHT_IN_MIN 59

#define HAIR_COLOR_STRING_LENGTH 7
#define HAIR_COLOR_FIRST_CHAR '#'
#define HAIR_COLOR_VALID_CHARS "0123456789abcdef"

#define NUM_VALID_EYE_COLORS 7

#define PASSPORT_ID_LENGTH 9
#define PASSPORT_ID_VALID_CHARS "0123456789"

char* validEyeColors[NUM_VALID_EYE_COLORS] = {
    "amb", 
    "blu",
    "brn",
    "gry",
    "grn",
    "hzl",
    "oth"
};

char* passportFields[NUM_PASSPORT_FIELDS] = {
    "byr",
    "iyr",
    "eyr",
    "hgt",
    "hcl",
    "ecl",
    "pid",
    "cid"
};

#endif