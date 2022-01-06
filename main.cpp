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
    //first we open the file, containing the data we want to replace and iterate through its rows as every row is different output file
    while (getline(file, row)) {
        index++;
        if (index == 0)
            continue;

        ifstream template_file(TEMPLATE_FILE);
        string output_row;
        ofstream output(to_string(index) + ".txt"); // the output file is being created
        bool isFor = false;
        string loop_output, array_values[100];
        string var_name, arr_name;
        int array_index = 0;
        while (getline(template_file, output_row)) {

            unsigned int row_length = output_row.length() - 1;
            int replacer_index = -1;
            bool endVar = false, declareFor = false, placedLoop = false;
            //for every element of the row in the template file
            for (int i = 0; i < row_length; i++) {
                if (output_row[i] == ' ')
                    continue;
                if (output_row[i] == '{') { //check if elements a special symbol
                    //if it is replacer_index(the index of the string,
                    // from which we will start replacing with our custom data) gets the value of the first opening bracelet

                    if (output_row[i + 1] == '%') {
                        replacer_index = i;
                        declareFor = true;
                    }
                    if (output_row[i + 1] == '{') {
                        replacer_index = i;
                    }
                    i++;
                    continue;
                }
                if (replacer_index > -1) { // we are in the body of the jinja2 and are reading the variable
                    if (output_row[i + 1] == '}') {
                        if (output_row[i] == '}') {
                            string name = output_row.substr(replacer_index + 2, i - replacer_index - 2);
                            strip(name);

                            int position = isInArray(names, name);
                            if (position != -1) //if the found variable is in the input file replace its value
                                output_row.replace(replacer_index, i - replacer_index + 2, splitByIndex(row, position));

                        } else if (output_row[i] == '%') {
                            if (declareFor) {
                                if (isFor) {
                                    for (int j = 0; j <= array_index; j++) {
                                        unsigned int loop_len = loop_output.length();
                                        replacer_index = -1;
                                        string edit_loop = loop_output;

                                        for (int t = 0; t < loop_len; t++) {
                                            if (loop_output[t] == '{' &&
                                                loop_output[t + 1] == '{') { //check if elements a special symbol
                                                replacer_index = t;
                                                t++;
                                                continue;
                                            }
                                            if (replacer_index > -1 && loop_output[t] == '}' &&
                                                loop_output[t + 1] == '}')
                                                edit_loop.replace(replacer_index, t - replacer_index + 2,
                                                                  array_values[j]);
                                        }
                                        output << edit_loop << endl;
                                    }
                                    declareFor = false;
                                    isFor = false;
                                    loop_output.clear();
                                    placedLoop = true;
                                }
                                break;
                            }
                        }
                    }
                    if (declareFor) {
                        if (output_row[i] == 'i' && output_row[i + 1] == 'n' && output_row[i - 1] == ' ' &&
                            output_row[i + 2] == ' ') {
                            endVar = true;
                            i++;
                        } else if (endVar)
                            arr_name.push_back(output_row[i]);
                        var_name.push_back(output_row[i]);
                    }
                }

            }
            if (placedLoop)
                continue;
            else if (declareFor && isInArray(names, arr_name) != -1) {
                isFor = true;
                unsigned int len = row.length();
                int start = isInArray(names, arr_name), comma_counter = 0;
                bool hasBracelet = false;
                for (int j = 0; j < len; j++) {
                    if (!hasBracelet && row[j] == ',') {
                        comma_counter++;
                        continue;
                    }

                    if (row[j] == '{') {
                        hasBracelet = true;
                        continue;
                    } else if (row[j] == '}') {
                        hasBracelet = false;
                        continue;
                    }

                    if (comma_counter == start) {
                        if (row[j] == ',')
                            array_index++;
                        else
                            array_values[array_index].push_back(row[j]);
                    }

                }

            } else if (!isFor)
                output << output_row << endl;
            else
                loop_output.append(output_row);
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
