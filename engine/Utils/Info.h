#ifndef INFO_H
#define INFO_H

#include <string>
#include <cmath>

constexpr char H1_CHAR = '~';
constexpr int H1_LEN = 60;
constexpr char H2_CHAR = '-';
constexpr int H2_LEN = 60;
constexpr char H3_CHAR = '.';
constexpr int H3_LEN = 60;

string printInfoTab(string size)
{
    int offset = 0;
    string tab = "";

    if(size == "H1")
    {
        offset = 0;
    }
    else if(size == "H2")
    {
        offset = 4;
    }
    else if (size == "H3")
    {
        offset = 8;
    }
    else
    {
        cout << "ERROR: incorrect header size!" << endl;
        return "";
    }

    for(int x = 0; x < offset; x++)
    {
        tab += " ";
    }

    return tab;
}

void printInfoHeader(string size, string name = "")
{
    char usedChar;
    int usedSize;
    int offset = 0;

    if(size == "H1")
    {
        usedChar = H1_CHAR;
        usedSize = H1_LEN;
        offset = 0;
    }
    else if(size == "H2")
    {
        usedChar = H2_CHAR;
        usedSize = H2_LEN;
        offset = 4;
    }
    else if (size == "H3")
    {
        usedChar = H3_CHAR;
        usedSize = H3_LEN;
        offset = 8;
    }
    else
    {
        cout << "ERROR: incorrect header size!" << endl;
        return;
    }

    cout << endl;
    if(name.size() == 0)
    {

        for(int x = 0; x < offset; x++)
        {
            cout << " ";
        }

        for(int x = 0; x < usedSize; x++)
        {
            cout << usedChar;
        }
    }else
    {
        int wordLen = name.size();
        int titleLen = 1 + 1+ wordLen + 1 + 1;
        int linesLen = usedSize - titleLen - offset;
        double lineLen = std::floor((float) linesLen / (float) 2.0);
        
        for(int x = 0; x < offset; x++)
        {
            cout << " ";
        }

        for(int x = 0; x < lineLen; x++)
        {
            cout << usedChar;
        }

        cout << "| ";
        cout << name;
        cout << " |";
        
        for(int x = 0; x < lineLen; x++)
        {
            cout << usedChar;
        }
    }

    cout << endl << endl;
}

#endif // INFO_H