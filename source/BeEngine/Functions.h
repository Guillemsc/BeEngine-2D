#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include <list>
#include <string>
#include "GeometryMath.h"

class ImVec2;
class ImVec4;
// -----------------------------------------
// -----------------------------------------

// USEFULL FUNCTIONS //

// -----------------------------------------
// -----------------------------------------

// Returns the angle between two points in degrees
float AngleFromTwoPoints(float x1, float y1, float x2, float y2);

// Returns the distance from two points(can be a negative distance)
float DistanceFromTwoPoints(float x1, float y1, float x2, float y2);

// Compares two char*, true if success
bool TextCmp(const char* text1, const char* text2);

// Copies astd::string to another
void TextCpy(char * destination, const char * origen);

// Gets size of astd::string
int TextLnght(char * txt);

// Separate astd::string into tokens using the indicated iterator
void Tokenize(std::string, const char separator, std::list<std::string>& tokens);

// Replaces part of a string with another part
std::string TextReplace(std::string main_str, std::string to_replace, std::string replace_with);

// Returns chars to upper case
std::string ToUpperCase(std::string str);

// Returns chars to lower case
std::string ToLowerCase(std::string str);

std::string StringFormatForSearch(const std::string& str);

bool StringContainsFormated(std::string str, std::string contains);

// Returns char to lower case
char ToLowerCase(char str);

// Returns a random int given a range;
int GetRandomValue(int range_1, int range_2);

// Returns a random float given a range;
float GetRandomValue(float range_1, float range_2);

// Returns a random float given a range;
double GetRandomValue(double range_1, double range_2);

double GetUIDRandomDouble();

float GetUIDRandomFloat();

int GetUIDRandomInt();

std::string IntToHexa(int num);

std::string GetUIDRandomHexadecimal();

// Returns true if a point is inide a rect
bool PointInRect(float2 point_xy, float4 rect_xywh);

// Returns true if a point is inide a rect
bool PointInRect(float2 point_xy, Rect rect_xywh);

std::wstring utf8_to_utf16(const std::string& utf8);

#endif //__FUNCTIONS_H__