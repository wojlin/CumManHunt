#include <string>
#include <cmath>

constexpr char H1_CHAR = '~';
constexpr int H1_LEN = 60;
constexpr char H2_CHAR = '-';
constexpr int H2_LEN = 60;
constexpr char H3_CHAR = '.';
constexpr int H3_LEN = 60;



void printInfoHeader(string size, string name = "")
{
    char usedChar;
    int usedSize;

    if(size == "H1")
    {
        usedChar = H1_CHAR;
        usedSize = H1_LEN;
    }
    else if(size == "H2")
    {
        usedChar = H2_CHAR;
        usedSize = H2_LEN;
    }
    else if (size == "H3")
    {
        usedChar = H3_CHAR;
        usedSize = H3_LEN;
    }
    else
    {
        cout << "ERROR: incorrect header size!" << endl;
        return;
    }

    cout << endl;
    if(name.size() == 0)
    {
        for(int x = 0; x < usedSize; x++)
        {
            cout << usedChar;
        }
    }else
    {
        int wordLen = name.size();
        int titleLen = 1 + 1+ wordLen + 1 + 1;
        int linesLen = usedSize - titleLen;
        double lineLen = std::floor((float) linesLen / (float) 2.0);
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