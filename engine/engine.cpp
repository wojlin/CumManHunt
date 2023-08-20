#include "include/GameData/GameData.h"

#include "include/GameData/LevelData.h"
#include "include/GameData/ColorMapData.h"
#include "include/GameData/PlayPalData.h"
#include "include/GameData/ResourcesData.h"
#include "include/GameData/EndoomData.h"
#include "include/GameData/AudioInfoData.h"
#include "include/GameData/SoundData.h"
#include "include/GameData/DemoData.h"

#include "include/Renderer/LevelBuild.h"
#include "include/Renderer/MinimapRenderer.h"
#include "include/Controller/Player.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <chrono>

using namespace std;




int main()
{
    GameData gameData = GameData();
    
    gameData.loadIWAD("../tests/iwad_doom2.WAD");
    gameData.compile();

    WADStructure::WADStructure* wad = gameData.getResourceFromWAD<WADStructure::WADStructure>();
    EndoomData::EndoomData* endoom = gameData.getResourceFromWAD<EndoomData::EndoomData>();
    PlayPalData::PlayPalData* playpal = gameData.getResourceFromWAD<PlayPalData::PlayPalData>();
    ColorMapData::ColorMapData* colormap = gameData.getResourceFromWAD<ColorMapData::ColorMapData>();
    ResourcesData::ResourcesData* resources = gameData.getResourceFromWAD<ResourcesData::ResourcesData>();
    AudioInfoData::AudioInfoData* audio = gameData.getResourceFromWAD<AudioInfoData::AudioInfoData>();
    SoundData::SoundData* sound = gameData.getResourceFromWAD<SoundData::SoundData>();
    DemoData::DemoData* demo = gameData.getResourceFromWAD<DemoData::DemoData>();

    gameData.printInfo();

    unique_ptr<LevelData::LevelData> level = gameData.getLevelData(0);
    level->printInfo();


    int WINDOW_WIDTH = 800; // in pixels
    int WINDOW_HEIGHT = 600; // in pixels
    int FRAMERATE_LIMIT = 60;  // frames per second
    bool VSYNC = false;

    int MINIMAP_SIZE = 100; // in percents
    int MINIMAP_CONTENT_PERCENTAGE_OFFSET = 5; // in percents
    int MINIMAP_BORFER_PERCENTAGE = 1; // in percents

    
    auto start = std::chrono::high_resolution_clock::now();
    

    LevelBuild::LevelBuild levelBuild = LevelBuild::LevelBuild(&level, wad);

    vector<Player> players;
    for(int i = 1; i < 5; i++)
    {
        players.push_back(Player(levelBuild.getPlayerInfo(i)));
    }
    
    MinimapRenderer::MinimapRenderer minimapRenderer = MinimapRenderer::MinimapRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, MINIMAP_SIZE, MINIMAP_CONTENT_PERCENTAGE_OFFSET, MINIMAP_BORFER_PERCENTAGE, &level, &levelBuild, &players);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;


    sf::RenderWindow window;
    sf::Event event;
  
    

    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CumManHunt");
    window.setSize(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT));
    window.setVerticalSyncEnabled(VSYNC);
    window.setFramerateLimit(FRAMERATE_LIMIT);

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


            window.clear(sf::Color::Black);

            LevelBuild::level_bounds_t* bounds = levelBuild.getLevelBounds();

            

            
            window.draw(*minimapRenderer.getMinimap());

            window.display();
        }
    }

    return 0;

}


