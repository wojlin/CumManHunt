#include "GameData/GameData.h"

using namespace std;


int main()
{
    GameData gameData = GameData();
    
    gameData.loadIWAD("../tests/iwad_doom2.WAD");
    gameData.loadPWAD("../tests/pwad_append.WAD");
    gameData.loadPWAD("../tests/pwad_replace.WAD");
    gameData.loadPWAD("../tests/pwad_replace_map.WAD");
    gameData.loadPWAD("../tests/pwad_append_map.WAD");
    //gameData.loadPWAD("../tests/not_wad.WAD");
    gameData.compile();

    WADStructure* wad = gameData.getResourceFromWAD<WADStructure>();
    EndoomData* endoom = gameData.getResourceFromWAD<EndoomData>();
    PlayPalData* playpal = gameData.getResourceFromWAD<PlayPalData>();
    ColorMapData* colormap = gameData.getResourceFromWAD<ColorMapData>();
    ResourcesData* resources = gameData.getResourceFromWAD<ResourcesData>();
    AudioInfoData* audio = gameData.getResourceFromWAD<AudioInfoData>();
    SoundData* sound = gameData.getResourceFromWAD<SoundData>();
    DemoData* demo = gameData.getResourceFromWAD<DemoData>();

    gameData.printInfo();

    unique_ptr<LevelData> level = gameData.getLevelData(0);
    level->printInfo();
    //level->printDetailedInfo();


    


    //MUS FORMAT TEST
    //Sound sound1 = sound.readSound("D_OPENIN");
    //sound1.printInfo();
    //sound1.play();

    //DOOM FORMAT TEST
    //Sound sound2 = sound.readSound("DSSGTDTH");
    //sound2.printInfo();
    //sound2.play();

    //PC SPEAKER TEST
    //Sound sound3 = sound->readSound("DPPESIT");
    //sound3.printInfo(); 
    //sound3.play();
   
    
    //SPRITE TEST
    //string name1 = "CHGGA0";
    //Image image1 = resources.readSprite(name1);
    //image1.printInfo();
    //image1.saveAsFile("/home/anon/PROJECTS/CumManHunt/" + name1 + ".bmp");


    //PATCH TEST
    //string name2 = "DOOR9_1";
    //Image image2 = resources.readPatch(name2);
    //image2.printInfo();
    //image2.saveAsFile("/home/anon/PROJECTS/CumManHunt/" + name2 + ".bmp");

    //FLAT TEST
    //string name3 = "CONS1_5";
    //Image image3 = resources.readFlat(name3);
    //image3.printInfo();
    //image3.saveAsFile("/home/anon/PROJECTS/CumManHunt/" + name3 + ".bmp");


    //endoom.printEndoom();
    //endoom.printInfo();
    //colormap.printInfo();
    //playpal.printInfo();
    //level.printLevelInfo();
    //audio.printInfo();
    //resources.printInfo();
    //demo.printInfo();

}

/*
#include <math.h>
#include <stdio.h>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;

int main()
{
    sf::Window window;
    sf::Event event;
    window.create(sf::VideoMode(800, 600), "My window");
    //window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
        
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                cout << "left" << endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                cout << "right" << endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                cout << "forward" << endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                cout << "back" << endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                cout << "close" << endl;
                window.close();
            }
        
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    cout << "xD";
    return 0;
}
*/