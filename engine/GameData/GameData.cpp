
#include "LevelData.h"
#include "ColorMapData.h"
#include "PlayPalData.h"
#include "ResourcesData.h"
#include "EndoomData.h"

using namespace std;

int main()
{
    WADStructure wad = WADStructure("../../tests/DOOM2.WAD");
    EndoomData endoom = EndoomData(&wad);
    PlayPalData playpal = PlayPalData(&wad);
    ColorMapData colormap = ColorMapData(&wad);
    ResourcesData resources = ResourcesData(&wad, &playpal);

    cout << "Path: \""<< wad.filePath << "\"" << endl;
    cout << endl << "HEADER:" << endl;
    cout << "Identification: " << wad.header.identification << endl;
    cout << "Numlumps: " << wad.header.numlumps << endl;
    cout << "Infotableofs: " << wad.header.infotableofs << endl;
    cout << endl << "INFO:" << endl;
    cout << "levels amount: " << wad.levelsAmount << endl;
    cout << "sprites amount: " << resources.getSpritesAmount() << endl;
    cout << "flats amount: " << resources.getFlatsAmount() << endl;
    cout << "patches amount: " << resources.getPatchesAmount() << endl;
    cout << "pallets amount: " << playpal.getPalletesAmount() << endl;
    cout << "colormaps amount: " << colormap.getColorMapsAmount() << endl;
    cout << "pnames amount: " << resources.getPNamesAmount() << endl;
    cout << endl;
    LevelData level = LevelData(wad.filePath, &wad.levelsList[0]);
   
    
    //SPRITE TEST
    string name1 = "CHGGA0";
    Image image1 = resources.readSprite(name1);
    //image1.printInfo();
    //image1.saveAsFile("/home/anon/PROJECTS/CumManHunt/" + name1 + ".bmp");


    //PATCH TEST
    string name2 = "DOOR9_1";
    Image image2 = resources.readPatch(name2);
    //image2.printInfo();
    //image2.saveAsFile("/home/anon/PROJECTS/CumManHunt/" + name2 + ".bmp");

    //FLAT TEST
    string name3 = "CONS1_5";
    Image image3 = resources.readFlat(name3);
    //image3.printInfo();
    //image3.saveAsFile("/home/anon/PROJECTS/CumManHunt/" + name3 + ".bmp");


    //endoom.printEndoom();
    //endoom.printInfo();
    //colormap.printInfo();
    //playpal.printInfo();

    //level.printLevelInfo();



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