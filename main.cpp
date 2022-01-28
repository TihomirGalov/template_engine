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
* <main file of the project>
*
*/

#include <iostream>
#include <fstream>
#include <string>
#include "utils.h"

using namespace std;


void get_names(string *output, const string &INPUT_FILE) {
    ifstream file(INPUT_FILE);
    string raw_row;
    getline(file, raw_row);
    file.close();

    size_t row_length = raw_row.size();
    unsigned int index = 0;
    string word;

    bool insert = true;

    for (unsigned int i = 0; i < row_length; i++) {
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

void write_file(string *names, const string &INPUT_FILE, const string &TEMPLATE_FILE) {
    ifstream file(INPUT_FILE);
    int index = -1; //starting with index = -1, because we skip the first row containing the variable names
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
        unsigned int array_index = 0;

        while (getline(template_file, output_row)) {

            size_t row_length = output_row.length() - 1;
            int replacer_index = -1;
            bool endVar = false, declareFor = false, placedLoop = false;

            //for every element of the row in the template file
            for (unsigned int i = 0; i < row_length; i++) {
                if (output_row[i] == ' ')
                    continue;
                if (isOpeningBracelet(output_row[i])) { //check if element is a special symbol
                    //if it is replacer_index(the index of the string,
                    // from which we will start replacing with our custom data) gets the value of the first opening bracelet

                    if (output_row[i + 1] == '%') //this means we found the beginning of a for loop
                        declareFor = true;
                    replacer_index = i;
                    i++;
                    continue;
                }
                if (replacer_index > -1) { // we are in the body of the jinja2 and are reading the variable
                    if (isClosingBracelet(output_row[i + 1])) {
                        if (isClosingBracelet(output_row[i])) {
                            string name = output_row.substr(replacer_index + 2, i - replacer_index - 2);
                            strip(name);

                            int position = isInArray(names, name);
                            if (position != -1) //if the found variable is in the input file replace its value
                                output_row.replace(replacer_index, i - replacer_index + 2, splitByIndex(row, position));

                        } else if (output_row[i] == '%') {
                            if (declareFor) {
                                if (isFor) { // means that we are at the {% endfor %} statement
                                    for (unsigned int j = 0; j <= array_index; j++) {
                                        unsigned int loop_len = loop_output.length();
                                        replacer_index = -1;
                                        string edit_loop = loop_output;

                                        for (int t = 0; t < loop_len; t++) {
                                            if (isOpeningBracelet(loop_output[t]) &&
                                                isOpeningBracelet(
                                                        loop_output[t + 1])) { //check if elements are special symbols
                                                replacer_index = t;
                                                t++;
                                                continue;
                                            }
                                            if (replacer_index > -1 && isClosingBracelet(loop_output[t]) &&
                                                isClosingBracelet(loop_output[t + 1]))
                                                edit_loop.replace(replacer_index, t - replacer_index + 2,
                                                                  array_values[j]);
                                        }
                                        output << edit_loop; //place everything from the loop body with replaced variables into the output file
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
                    if (declareFor) { //we are at the end of the loop declaration
                        if (isLoopIn(output_row, i)) {
                            endVar = true;
                            i++;
                        } else if (endVar)
                            arr_name.push_back(output_row[i]); // assign the name of the array
                        var_name.push_back(output_row[i]); // and the name of the variable that will be replaced
                    }
                }

            }
            if (placedLoop)
                continue;
            else if (declareFor && isInArray(names, arr_name) != -1) { // loading the array values from the input file
                isFor = true;
                unsigned int len = row.length();
                int start = isInArray(names, arr_name), comma_counter = 0;
                bool hasBracelet = false;
                for (unsigned int j = 0; j < len; j++) {
                    if (!hasBracelet && row[j] == ',') {
                        comma_counter++;
                        continue;
                    }

                    if (isOpeningBracelet(row[j])) {
                        hasBracelet = true;
                        continue;
                    } else if (isClosingBracelet(row[j])) {
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
                output << output_row << endl; //if we are not in a loop we place the row in the output file
            else {
                loop_output.append(output_row); // the row doesn't contain any special symbols
                loop_output.push_back('\n');
            }

        }
        output.close();
        template_file.close();
    }
    file.close();

}

int main() {
    string INPUT_FILE;
    string TEMPLATE_FILE;

    cout << "Enter the directory of the input file: ";
    cin >> INPUT_FILE;
    cout << "Enter the directory of the template file: ";
    cin >> TEMPLATE_FILE;

    string names[100];

    get_names(names, INPUT_FILE);
    write_file(names, INPUT_FILE, TEMPLATE_FILE);

    return 0;
}
