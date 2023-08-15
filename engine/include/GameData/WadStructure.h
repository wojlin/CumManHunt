#ifndef WADSTRUCTURE_H
#define WADSTRUCTURE_H

#include <iostream>
#include <fstream>
#include <bitset>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>


#include "Interfaces.h"

namespace WADStructure
{
    struct header_t
    {             
        string identification;  //The ASCII characters "IWAD" or "PWAD". 
        int numlumps;           //An integer specifying the number of lumps in the WAD. 
        int infotableofs;       //An integer holding a pointer to the location of the directory. 
    };

    struct lumpInfo_t 
    {
        string path;
        string name;
        int filepos;
        int size; 
    };


    struct levelInfo_t {
        string name;
        lumpInfo_t THINGS;
        lumpInfo_t LINEDEFS;
        lumpInfo_t SIDEDEFS;
        lumpInfo_t VERTEXES;
        lumpInfo_t SEGS;
        lumpInfo_t SSECTORS;
        lumpInfo_t NODES;
        lumpInfo_t SECTORS;
        lumpInfo_t REJECT;
        lumpInfo_t BLOCKMAP;
    };

    struct pwad_t
    {
        string path;
        header_t header;
        vector<lumpInfo_t> directory;
        vector<levelInfo_t> levelsList; 
    };



    class WADStructure: public baseResourceWAD {      
        public: 

            string filePath = "";
            header_t header;
            vector<lumpInfo_t> directory;
            vector<levelInfo_t> levelsList; 
            
            bool iWadFound = false;
            bool compiled = false;

            WADStructure();

            WADStructure(string path);

            ~WADStructure();

            void compile();

            int getlevelsAmount();

            int getDirectoryCount();

            void loadPWAD(string path);

            lumpInfo_t findLump(string lumpName);

            vector<lumpInfo_t> findLumps(string lumpName);

            void printInfo();

            header_t getHeader();

            vector<pwad_t> getPwads();

            vector<string> getPwadsPaths();

            int getPwadsAmount();
        
        private:

            vector<pwad_t> pwads;

            string charsToString(char* chars, int size);

            void readFile(string path);

            header_t readHeader(ifstream *file);     

            vector<lumpInfo_t> readDirectory(ifstream *file, header_t lHeader, string path);

            vector<levelInfo_t> readLevelsList(header_t lHeader, vector<lumpInfo_t> lDirectory);
    };

}

#endif