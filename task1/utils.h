#ifndef TASK1_UTILS_H_
#define TASK1_UTILS_H_

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#define MIX "mix"
#define LOWEST "lowest"
#define HIGHEST "highest"
#define MIX_SATURATE "mix-saturate"

#define DEFAULT_MODE MIX

#define FILENAME "colors.txt"

using namespace std;

class Utils
{
public:
    Utils(){};
    ~Utils(){};

    void readFile(vector<string> &colors)
    {
        ifstream file(FILENAME);
        if (file.is_open())
        {
            string line;
            cout << "File opened" << endl;
            while (getline(file, line))
            {
                if (!isWordValid(line))
                {
                    if (!line.empty())
                    {
                        cout << "Skipping: " << line << " - invalid format." << endl;
                    }
                    continue;
                }
                colors.push_back(line);
            }
            file.close();
            cout << "File closed" << endl;
        }
        else
        {
            cout << "Cannot open the file. Please check if the file directory is correct." << endl;
        }
    }

    void readArguments(int argc, char *argv[], string &mode, vector<string> &cliColors)
    {
        for (int i = 1; i < argc; i++)
        {
            string arg = argv[i];
            if (arg == "--mode" || arg == "-m")
            {
                if (i + 1 < argc)
                {
                    mode = argv[i + 1];
                    i++;
                }
                continue;
            }
            if (!isWordValid(arg))
            {
                cout << "Skipping: " << arg << " - invalid format." << endl;
                continue;
            }
            cliColors.push_back(arg);
        }
    }

    void checkMode(string &mode)
    {
        if (!isCorrectMode(mode))
        {
            cout << "Given mode: " << mode << " is not defined" << endl;
            cout << "Correct modes are: ";
            printModes();
            mode = DEFAULT_MODE;
            cout << "Setting default mode: " << mode << endl;
        }
    }

private:
    string modes[4] = {MIX, LOWEST, HIGHEST, MIX_SATURATE};

    bool isWordValid(string word)
    {
        if (word.empty())
        {
            return false;
        }
        regex rgbaRegex("^([0-9]{1,3},){3}[0-9]{1,3}$");
        regex hexRegex("^([0-9a-f]{3}|[0-9a-f]{6}|[0-9a-f]{8})$", regex_constants::icase);
        return regex_match(word, rgbaRegex) || regex_match(word, hexRegex);
    }

    bool isCorrectMode(string mode)
    {
        auto it = find(begin(modes), end(modes), mode);
        if (it != end(modes))
        {
            return true;
        }
        return false;
    }

    void printModes()
    {
        for (string mode : modes)
        {
            cout << mode << " ";
        }
        cout << endl;
    }
};

#endif