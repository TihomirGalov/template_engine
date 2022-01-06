#include <iostream>
#include <fstream>
#include <string>
#include "utils.h"

using namespace std;

string INPUT_FILE;
string TEMPLATE_FILE;


void get_names(string *output) {
    ifstream file(INPUT_FILE);
    string raw_row;
    getline(file, raw_row);
    file.close();

    unsigned int row_length = raw_row.size();
    int index = 0;
    string word;

    bool insert = true;

    for (int i = 0; i < row_length; i++) {
        if (!insert && raw_row[i] != ',')
            continue;
        else if (raw_row[i] == ',')
            insert = true;

        else if (raw_row[i] == ':') {
            output[index++] = word;
            insert = false;
            word.clear();
        } else
            word.push_back(raw_row[i]);

    }

}

int isInArray(string names[], const string &substr) {
    unsigned int len = names->length();
    for (int i = 0; i < len; i++) {
        if (names[i] == substr)
            return i;
    }
    return -1;
}


void write_file(string *names) {
    ifstream file(INPUT_FILE);
    int index = -1;
    string row;
    while (getline(file, row)) {
        index++;
        if (index == 0)
            continue;

        ifstream template_file(TEMPLATE_FILE);
        string output_row;
        ofstream output(to_string(index) + ".txt");

        while (getline(template_file, output_row)) {

            string var_name, arr_name;

            unsigned int row_length = output_row.length() - 1;
            int replacer_index = -1;
            bool isFor = false, endVar = false;

            for (int i = 0; i < row_length; i++) {
                if (output_row[i] == ' ')
                    continue;
                if (output_row[i] == '{') {
                    if (output_row[i + 1] == '%') {
                        replacer_index = i;
                        isFor = true;
                    }
                    if (output_row[i + 1] == '{') {
                        replacer_index = i;
                    }
                    i++;
                    continue;
                }
                if (replacer_index > -1) {
                    if (output_row[i + 1] == '}') {
                        if (output_row[i] == '}') {
                            string name = output_row.substr(replacer_index + 2, i - replacer_index - 2);
                            strip(name);

                            int position = isInArray(names, name);
                            if (position != -1)
                                output_row.replace(replacer_index, i - replacer_index + 2, splitByIndex(row, position));


                        } else if (output_row[i] == '%') {
                            //@TODO for loop stuff
                            continue;
                        }

                    }

                    if (isFor) {
                        if (output_row[i] == 'i' && output_row[i + 1] == 'n' && output_row[i - 1] == ' ' &&
                            output_row[i + 2] == ' ') {
                            endVar = true;
                            i++;
                        } else if (endVar) {
                            arr_name.push_back(output_row[i]);
                        }
                        var_name.push_back(output_row[i]);
                        continue;
                    }
                }

            }
            output << output_row << endl;
        }
        output.close();
        template_file.close();
    }
    file.close();

}

int main() {
    cout << "Enter the directory of the input file: ";
    cin >> INPUT_FILE;
    cout << "Enter the directory of the template file: ";
    cin >> TEMPLATE_FILE;
    string names[100];

    get_names(names);
    write_file(names);

    return 0;
}

