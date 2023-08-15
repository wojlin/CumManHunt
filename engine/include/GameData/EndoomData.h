#ifndef ENDOOMDATA_H
#define ENDOOMDATA_H

#include <map>
#include <math.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>

#include "Interfaces.h"
#include "WadStructure.h"

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

        EndoomData();

        EndoomData(WADStructure *wad);

        void printInfo();

        int getEndoomRows();

        int getEndoomCols();

        vector<charData_t> getChars();

        void printEndoom();


    private:
        string filePath;
        WADStructure::lumpInfo_t endoomLump;
        int ROWS = 25;
        int COLS = 80;

        vector<charData_t> charsData;

        struct charInfo_t
        {
            uint8_t value;
            uint8_t data;
        };

        int numberToColor(int);

        void readEndoom();
};

#endif