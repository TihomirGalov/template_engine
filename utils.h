#ifndef TEMPLATE_ENGINE_UTILS_H
#define TEMPLATE_ENGINE_UTILS_H

#include <string>

std::string splitByIndex(std::string &data, int index);

void strip(std::string &s);

int isInArray(std::string names[], const std::string &substr);

bool isOpeningBracelet(char a);

bool isClosingBracelet(char a);

bool isLoopIn(std::string &s, size_t index);

#endif //TEMPLATE_ENGINE_UTILS_H
