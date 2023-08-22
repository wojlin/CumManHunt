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
#include "include/Renderer/BSP.h"

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


    int WINDOW_WIDTH = 1800; // in pixels
    int WINDOW_HEIGHT = 1000; // in pixels
    int FRAMERATE_LIMIT = 60;  // frames per second
    bool VSYNC = false;

    int MINIMAP_SIZE = 100; // in percents
    int MINIMAP_CONTENT_PERCENTAGE_OFFSET = 5; // in percents
    int MINIMAP_BORFER_PERCENTAGE = 1; // in percents

    int FOV = 90;

    int MINIMAP_FOV_DISTANCE_PERCENT = 50;

    int PLAYER_SPEED = 300;

    float playerRotation = 0.0f;
     float rotationSpeed = 1.0f; // Adjust as needed
     int framesBeforeReset = 0;
    const int resetFrequency = 5; // Reset every 10 frames

    auto start = std::chrono::high_resolution_clock::now();
    

    LevelBuild::LevelBuild levelBuild = LevelBuild::LevelBuild(&level, wad);
    

    vector<Player> players;
    for(int i = 1; i < 5; i++)
    {
        players.push_back(Player(levelBuild.getPlayerInfo(i)));
    }
    
    BSP bsp = BSP(&level, &players[0], FOV);
    bsp.renderBsp();

    
    
    //vector<LevelData::Seg> segArray = 
    //for(LevelData::Seg seg: segArray)
    //{
    //    cout << seg.startVertex << " " << seg.endVertex << endl;
    //}


    MinimapRenderer::MinimapRenderer minimapRenderer = MinimapRenderer::MinimapRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, MINIMAP_SIZE, MINIMAP_CONTENT_PERCENTAGE_OFFSET, MINIMAP_BORFER_PERCENTAGE, FOV, MINIMAP_FOV_DISTANCE_PERCENT, &level, &levelBuild, &players);
    minimapRenderer.drawMinimap();
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;


    sf::RenderWindow window;
    sf::Event event;
    sf::Clock clock;
    sf::Clock clockTimer;
    sf::Time deltaTime;
    
    sf::Text fpsText;

    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        // Handle font loading error
        return 1;
    }

    fpsText.setFont(font);
    fpsText.setCharacterSize(34);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition(100.0f, 100.0f);
    

    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CumManHunt");
    window.setSize(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT));
    window.setVerticalSyncEnabled(VSYNC);
    window.setFramerateLimit(FRAMERATE_LIMIT);
    window.setMouseCursorVisible(false);

    sf::Vector2i prevMousePosition;
    sf::Vector2i windowCenter(window.getSize().x / 2, window.getSize().y / 2);

    sf::Mouse::setPosition(windowCenter, window);

    int currentNode = 0;
    clockTimer.restart();

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {     
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }    
        }  

        deltaTime = clock.restart();
        float dtSeconds = deltaTime.asSeconds();
        float fps = 1.0f / dtSeconds;

        std::ostringstream ss;
        ss << "FPS: " << static_cast<int>(fps);
        fpsText.setString(ss.str());
        

        // Calculate mouse movement
        sf::Vector2i currentMousePosition = sf::Mouse::getPosition(window);
        sf::Vector2i mouseMovement = currentMousePosition - windowCenter;
        // Reset the mouse position to the center of the screen periodically
        if (framesBeforeReset >= resetFrequency) {
            sf::Mouse::setPosition(windowCenter, window);
            framesBeforeReset = 0;
        } else {
            framesBeforeReset++;
        }
        // Update player rotation based on mouse movement
        playerRotation += static_cast<float>(mouseMovement.x) * rotationSpeed * deltaTime.asSeconds();
        sf::Vector2f movementDirection(std::cos(playerRotation * M_PI / 180.0f), std::sin(playerRotation * M_PI / 180.0f));



        sf::Vector2f forward(std::cos(playerRotation * (3.14159f / 180.0f)), std::sin(playerRotation * (3.14159f / 180.0f)));
        sf::Vector2f movement(0.0f, 0.0f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            movement += forward;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            movement -= forward;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            movement -= sf::Vector2f(-forward.y, forward.x); // Strafe left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            movement += sf::Vector2f(-forward.y, forward.x); // Strafe right


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            cout << "close" << endl;
            window.close();
        }



        
        if (movement != sf::Vector2f(0.0f, 0.0f)) 
        {
            float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
            movement /= length;
            players[0].setPosX(players[0].getPosX() + PLAYER_SPEED * dtSeconds * movement.x);
            players[0].setPosY(players[0].getPosY() + PLAYER_SPEED * dtSeconds * movement.y);
        }
    
        


        players[0].setAngle(playerRotation);
        


        window.clear(sf::Color::Black);

        LevelBuild::level_bounds_t* bounds = levelBuild.getLevelBounds();
        bsp.renderBsp();
        minimapRenderer.drawMinimap();
        vector<int> nodes = bsp.getNodesTree();
        vector<int> segs = bsp.getSegsTree();
        for(int i =0; i < segs.size(); i++)
        {
            minimapRenderer.drawSegById(segs[i]);
        }
        //for(int i =0; i < nodes.size(); i++)
        //{
        //    minimapRenderer.drawNodeById(nodes[i]);
        //}
        //minimapRenderer.drawNodeById(nodes[nodes.size() - 1]);

        /*
        minimapRenderer.drawNodeById(nodes[currentNode]);
        
        
        if(clockTimer.getElapsedTime().asSeconds() > 1)
        {   
        
            currentNode++;
            if(currentNode == nodes.size())
            {
                minimapRenderer.drawMinimap();
                currentNode = 0;
            }
            cout << "draw next node" << endl; 
            clockTimer.restart();
        }
        */
        minimapRenderer.update();
        window.draw(*minimapRenderer.getMinimap());

        window.draw(fpsText);

        window.display();

        
    }

    return 0;

}


