
#include "LevelData.h"
#include "ResourcesData.h"

using namespace std;

int main()
{
    WADStructure wad = WADStructure("../../tests/DOOM2.WAD");
    ResourcesData resources = ResourcesData(&wad);

    cout << "Path: \""<< wad.filePath << "\"" << endl;
    cout << endl << "HEADER:" << endl;
    cout << "Identification: " << wad.header.identification << endl;
    cout << "Numlumps: " << wad.header.numlumps << endl;
    cout << "Infotableofs: " << wad.header.infotableofs << endl;
    cout << endl << "INFO:" << endl;
    cout << "levels amount: " << wad.levelsAmount << endl;
    cout << "sprites amount: " << resources.getSpritesAmount() << endl;
    
    LevelData level = LevelData(wad.filePath, &wad.levelsList[0]);
    
    //resources.readSprite("BLUDA0");
    level.printLevelInfo();

    /*
    cout << endl << "ENTRIES:" << endl;
    cout << setw(5) << "[ENTRY]" << setw(10) << "[NAME]" << setw(10) << "[SIZE]" << setw(10) << "[FILEPOS]" << endl;
    for (int i = 0; i < wad.header.numlumps; ++i) {
        cout << setw(5) << i + 1;
        cout << setw(10) << wad.directory[i].name;
        cout << setw(10) << wad.directory[i].size;
        cout << setw(10) << wad.directory[i].filepos;
        cout << endl;
    }
    */


    /*
    for (size_t i = 0; i < wad.levelsAmount; ++i) {
        cout << wad.levelsList[i].name << endl;
    }
    */
   
    /*
    int num = wad.directory.filepos;

    unsigned char* bytePtr = reinterpret_cast<unsigned char*>(&num);

    // Output each byte in hexadecimal format
    for (int i = 0; i < sizeof(num); ++i) {
        std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(bytePtr[i]) << " ";
    }

    std::cout << std::endl;
    */

}