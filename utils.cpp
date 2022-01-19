/**
*
* Solution to course project # 4
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2021/2022
*
* @author Tihomir Galov
* @idnumber 1MI0600118
* @compiler GCC
*
* <file with helper functions for working with strings>
*
*/
#include <string>
#include "utils.h"

std::string splitByIndex(std::string &data, int index) {
    int len = data.length();
    int comma_counter = 0;
    std::string output;
    for (int i = 0; i < len; i++) {
        if (data[i] == ',') {
            comma_counter++;
            continue;
        }
        if (comma_counter == index)
            output.push_back(data[i]);
        else if (comma_counter > index)
            break;
    }

    return output;

}

void strip(std::string &s) {
    if (s[0] == ' ' && s[s.length() - 1] == ' ') {
        s.erase(0, 1);
        s.erase(s.length() - 1, 1);
    }
}

int isInArray(std::string names[], const std::string &substr) {
    unsigned int len = names->length();
    for (int i = 0; i < len; i++) {
        if (names[i] == substr)
            return i;
    }
    return -1;
}

bool isOpeningBracelet(char a) {
    return a == '{';
}

bool isClosingBracelet(char a) {
    return a == '}';
}

bool isLoopIn(std::string &s, size_t index) {
    return (s[index] == 'i' && s[index + 1] == 'n' && s[index - 1] == ' ' &&
            s[index + 2] == ' ');
}