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