#ifndef TASK1_COLOR_H_
#define TASK1_COLOR_H_

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define MIX "mix"
#define MIN_MAX "min_max"

using namespace std;

class Colors
{
public:
    Colors(){};
    ~Colors(){};

    void init(vector<string> &_cliColors, vector<string> &_fileColors)
    {
        strColors.reserve(_cliColors.size() + _fileColors.size());
        strColors.insert(strColors.end(), _fileColors.begin(), _fileColors.end());
        strColors.insert(strColors.end(), _cliColors.begin(), _cliColors.end());
    }

    void mix()
    {
        prepareColors(MIX);
        size_t colorsSize = colorsInt.size();
        int sumRGBA[4] = {0, 0, 0, 0};
        for (int i = 0; i < colorsSize; i++)
        {
            sumRGBA[0] += colorsInt[i][0];
            sumRGBA[1] += colorsInt[i][1];
            sumRGBA[2] += colorsInt[i][2];
            sumRGBA[3] += colorsInt[i][3];
        }
        for (int i = 0; i < 4; i++)
        {
            color[i] = sumRGBA[i] / colorsSize;
        }
        calculateHSL();
        cout << "New color after 'mix' method:" << endl;
        printColor();
    }

    void lowest() 
    {
        prepareColors(MIN_MAX);
        color[0] = *min_element(colorsR.begin(), colorsR.end());
        color[1] = *min_element(colorsG.begin(), colorsG.end());
        color[2] = *min_element(colorsB.begin(), colorsB.end());
        color[3] = *min_element(colorsA.begin(), colorsA.end());
        calculateHSL();
        cout << "New color after 'lowest' method:" << endl;
        printColor();
    }

    void highest() 
    {
        prepareColors(MIN_MAX);
        color[0] = *max_element(colorsR.begin(), colorsR.end());
        color[1] = *max_element(colorsG.begin(), colorsG.end());
        color[2] = *max_element(colorsB.begin(), colorsB.end());
        color[3] = *max_element(colorsA.begin(), colorsA.end());
        calculateHSL();
        cout << "New color after 'highest' method:" << endl;
        printColor();
    }

    void mixSaturate()
    {
        prepareColors(MIX);
        size_t colorsSize = colorsInt.size();
        array<int, 4> lastColor = colorsInt.back();
        double sumSat = 0;
        for (int i = 0; i < colorsSize-1; i++) {
            color = colorsInt[i];
            calculateHSL();
            sumSat += hsl[1];
        }
        color = lastColor;
        calculateHSL();
        cout << "sum sat" << sumSat << endl;
        hsl[1] = sumSat / static_cast<double>(colorsSize-1);
        printColor();
    }

private:
    vector<array<int, 4>> colorsInt;
    vector<int> colorsR;
    vector<int> colorsG;
    vector<int> colorsB;
    vector<int> colorsA;
    array<int, 4> color;
    array<double, 3> hsl;
    vector<string> strColors;

    void clearColor()
    {
        for (int i = 0; i < 4; i++)
        {
            if (i == 3)
            {
                color[i] = 0xff;
                continue;
            }
            color[i] = 0;
        }
    }

    void parseColor(string strColor)
    {
        cout << strColor<<endl;
        if (strColor.size() == 3)
        {
            for (int i = 0; i < strColor.size(); i++)
            {
                color[i] = stoi(string(2, strColor[i]), nullptr, 16);
            }
        }
        if (strColor.size() == 6 || strColor.size() == 8)
        {
            for (int i = 0; i < strColor.size(); i += 2)
            {
                color[i / 2] = stoi(strColor.substr(i, 2), nullptr, 16);
            }
        } else {
            stringstream strColorStream(strColor);
            char c;
            for(int i = 0; i < 4; i++) 
            {
                strColorStream >> color[i] >> c;
                cout << color[i] << endl;
            }
        }
    }

    void prepareColors(string mode)
    {
        clearColor();
        for (string strColor : strColors)
        {
            parseColor(strColor);
            if (mode == MIX) 
            {
                colorsInt.push_back(color);
            }
            if (mode == MIN_MAX) 
            {
                colorsR.push_back(color[0]);
                colorsG.push_back(color[1]);
                colorsB.push_back(color[2]);
                colorsA.push_back(color[3]);
            }
        }
    }

    void calculateHSL()
    {
        double r = color[0] / 255.0;
        double g = color[1] / 255.0;
        double b = color[2] / 255.0;
        double cmin = min({r, g, b});
        double cmax = max({r, g, b});
        double delta = cmax - cmin;
        double h, s, l;

        if (delta == 0)
        {
            h = 0;
        }
        else if (cmax == r)
        {
            h = fmod((g - b) / delta, 6);
        }
        else if (cmax == g)
        {
            h = ((b - r) / delta) + 2;
        }
        else
        {
            h = ((r - g) / delta) + 4;
        }
        h *= 60;
        if (h < 0)
        {
            h += 360;
        }
        l = (cmax + cmin) / 2;
        if (delta == 0)
        {
            s = 0;
        }
        else
        {
            s = delta / (1 - abs(2 * l - 1));
        }

        hsl = {h, s, l};
    }
    
    void printColor() 
    {
        cout << "Red: " << color[0] << endl;
        cout << "Green: " << color[1] << endl;
        cout << "Blue: " << color[2] << endl;
        cout << "Alpha: " << color[3] << endl;
        printf("Hex: #%02x%02x%02x%02x\n", color[0], color[1], color[2], color[3]);
        cout << "Hue: " << hsl[0] << endl;
        cout << "Saturation: " << hsl[1] << endl;
        cout << "Lightness: " << hsl[2] << endl;
    }
};
#endif