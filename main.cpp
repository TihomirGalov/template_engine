#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const string INPUT_FILE = "/home/tihomir/Documents/university/template_engine/media/database.txt";
const string TEMPLATE_FILE = "/home/tihomir/Documents/university/template_engine/media/template.txt";


void get_names(string *output) {
    ifstream file(INPUT_FILE);
    string raw_row;
    getline(file, raw_row);
    file.close();

    int row_length = raw_row.size();
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

void strip(string &s) {
    if(s[0] == ' ' && s[s.length()-1] == ' ') {
        s.erase(0, 1);
        s.erase(s.length() - 1, 1);
    }
}

bool isInArray(string names[], string substr) {
    int len = names->length();
    for(int i = 0; i < len; i++) {
        if(!names[i].compare(substr))
            return true;
    }
    return false;
}


bool isLoop(string &row, string &var_name, string &arr_name) {
    int row_length = row.length();
    bool inLoopBody = false, hasVariable = false;


    for (int i = 1; i < row_length; i++) {
        if (row[i] == ' ')
            continue;

        if(row[i - 1] == '{' && row[i] == '%') {
            inLoopBody = true;
            continue;
        }
        if (inLoopBody) {
            if (row[i] == '}' && row[i - 1] == '%'  && hasVariable)
                return true;
            if (row[i] == 'n' && row[i-1] == 'i' && var_name.length()) {
                hasVariable = true;
            }

            if (!hasVariable)
                var_name.push_back(row[i]);
            else
                arr_name.push_back(row[i]);

        }

    }
    return false;
}

void for_loop(string names[], int row, string *var_name, string *arr_name) {
    ifstream template_file(TEMPLATE_FILE);
    string template_row;
    int curr_row = 1;
    while (getline(template_file, template_row)) {
        if (curr_row++ < row)
            continue;
        int row_len = template_row.length() - 1;
        bool hasPercentage = false, hasBracelet = false;
        string local_var;
        int start_index = 0, end_index = 0;

        for (int i = 0; i < row_len; i++) {
            if(template_row[i] == '{') {
                if(template_row[i + 1] == '%')
                    hasPercentage = true;
                if(template_row[i + 1] ==  '{')
                    hasBracelet = true;
                continue;
            }


        }


    }
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
        int row = 1;
        while (getline(template_file, output_row)) {
            ofstream output(to_string(index) + ".txt");
            string var_name, arr_name;
            if(isLoop(output_row, var_name, arr_name) && isInArray(names, arr_name)) {
                for_loop(names, row, &var_name, &arr_name);
            }
            row++;

        }

    }
    file.close();

}

int main() {
    string names[100];
    get_names(names);

    write_file(names);

    return 0;
}

