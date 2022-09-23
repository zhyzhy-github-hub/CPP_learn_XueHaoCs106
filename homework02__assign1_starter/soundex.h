/**
 * File: soundex.h
 *
 * This is just here to communicate the prototype 
 * information for the functions in soundex.cpp that 
 * will be called from main.cpp
 */
#pragma once
#include <string>

void soundexSearch(std::string filepath);
std::string soundex(std::string s);
std::string removeNonLetters(std::string s);

std::string inputName(std::string input);
int getDigit(char inputSingleChar);
std::string getOriginalDigits(std::string input);
std::string removeDuplicationAndAdjacent(std::string inputSurname);
std::string substituteFirstDigitToUpperLetter(std::string inputSurname, std::string inputDigits);
std::string deleteZeroInDigits(std::string inputSurname);
std::string getLength4(std::string inputSurname);


