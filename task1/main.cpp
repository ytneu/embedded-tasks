#include <iostream>
#include <string>
#include <vector>

#include "color.h"
#include "utils.h"

using namespace std;

int main(int argc, char *argv[])
{
    Utils utils;
    Colors colors;

    string mode;
    vector<string> cliColors;
    vector<string> fileColors;

    utils.readFile(fileColors);
    utils.readArguments(argc, argv, mode, cliColors);
    utils.checkMode(mode);

    colors.init(cliColors, fileColors);

    if (mode == MIX)
    {
        colors.mix();
    }
    if (mode == LOWEST)
    {
        colors.lowest();
    }
    if (mode == HIGHEST)
    {
        colors.highest();
    }
    if (mode == MIX_SATURATE)
    {
        colors.mixSaturate();
    }

    return 0;
}
