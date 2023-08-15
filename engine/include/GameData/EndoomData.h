#include <iostream>

#ifndef ENDOOMDATA_H
#define ENDOOMDATA_H
#include "Interfaces.h"
#endif

struct charData_t
{
    uint8_t value;
    uint8_t foregroundColor;
    uint8_t backgroundColor;
    bool blinking;
};

class EndoomData: public baseResourceWAD
{
    public:

        EndoomData(){}

        EndoomData(WADStructure *wad)
        {
            filePath = wad->filePath;
            endoomLump = wad->findLump("ENDOOM");
            readEndoom();
        }

        void printInfo()
        {
            for(int x = 0; x < charsData.size(); x++)
            {   
                cout << x << ":  char unsigned: " << unsigned(charsData[x].value) << "    char:\"" << (char)charsData[x].value << "\",  foreground:" << unsigned(charsData[x].foregroundColor) << ",  background:" << unsigned(charsData[x].backgroundColor) << ",  blinking:" << std::boolalpha << charsData[x].blinking << endl;    
            }        
        }

        int getEndoomRows()
        {
            return ROWS;
        }

        int getEndoomCols()
        {
            return COLS;
        }


        vector<charData_t> getChars()
        {
            return charsData;
        }

        void printEndoom()
        {
            const std::string ANSI_ESCAPE = "\033[";

            // ANSI color codes
            const std::string ANSI_COLOR_RESET = "\033[0m";
            const std::string ANSI_COLOR_BLINK = "5m";
            const std::string ANSI_COLOR_BG_PREFIX = "48;5;";
            const std::string ANSI_COLOR_FG_PREFIX = "38;5;";
            int counter = 0;
            for(int y = 0; y < ROWS; y++)
            {
                for(int x = 0; x < COLS; x++)
                {
                    string ansiCode = "";

                    ansiCode += ANSI_ESCAPE;
                    ansiCode += "1m";
                    
                    if (charsData[counter].blinking) {
                        ansiCode += ANSI_ESCAPE;
                        ansiCode += ANSI_COLOR_BLINK;
                        ansiCode += "m";
                    }

                    ansiCode += ANSI_ESCAPE;
                    ansiCode += ANSI_COLOR_BG_PREFIX + to_string(numberToColor(charsData[counter].backgroundColor));
                    ansiCode += "m";
                    ansiCode += ANSI_ESCAPE;
                    ansiCode += ANSI_COLOR_FG_PREFIX + to_string(numberToColor(charsData[counter].foregroundColor));
                    ansiCode += "m";

                    
                
                    
                    cout << ansiCode << static_cast<char>(charsData[counter].value) << ANSI_ESCAPE + ANSI_COLOR_RESET;
                    counter ++;
                }
                cout << endl;
            }
        }



    private:
        string filePath;
        WADStructure::lumpInfo_t endoomLump;
        int ROWS = 25;
        int COLS = 80;

        struct charInfo_t
        {
            uint8_t value;
            uint8_t data;
        };

        
        

        vector<charData_t> charsData;

        int numberToColor(int number)
        {
            map<int, int> convert{{0, 0}, {1, 20}, {2, 2}, {3, 6}, {4, 1}, {5, 5}, {6, 208}, {7, 248}, {8, 237}, {9, 32}, {10, 46}, {11, 14}, {12, 160}, {13, 199}, {14, 11}, {15, 7}};
            return convert[number];
        }

        void readEndoom()
        {         
            std::ifstream file(filePath, std::ios::binary);
            file.seekg(endoomLump.filepos);
            for(int i = 0; i < endoomLump.size / 2; i++)
            {
                charInfo_t info;
                file.read(reinterpret_cast<char*>(&info), sizeof(charInfo_t));  
                
                charData_t charData;
                charData.value = info.value;
                charData.foregroundColor = info.data & 0b00001111;       // Extract foreground color (bits 0-3)
                charData.backgroundColor = (info.data >> 4) & 0b00000111; // Extract background color (bits 4-6)
                charData.blinking = (info.data >> 7) & 0b00000001;        // Extract blinking flag (bit 7)
                charsData.push_back(charData);
            }
            
            file.close();
        }
};