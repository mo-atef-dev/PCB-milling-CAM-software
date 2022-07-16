#ifndef MMG_PARSE_H_INCLUDED
#define MMG_PARSE_H_INCLUDED
#include<vector>
#include "cmds.h"

std::vector<CompressedCommand> MMG_PARSE(char* str);
/*this function gets the MMG file in array of char and returns vector of structs "CompressedCommand" the array of char is about MMG commands and
after each command a new line character "\n"  */

#endif // MMG_PARSE_H_INCLUDED
