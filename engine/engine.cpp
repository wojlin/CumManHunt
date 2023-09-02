#include "engine.h"

//public

Engine::Engine()
{

}

vertexs_bounds_t Engine::getVertexsBounds()
{
    return vertexsBounds;
}

level_bounds_t Engine::getLevelBounds()
{
    return levelBounds;
}

LevelData::LevelData* Engine::getLevelData()
{
    return &level;
}

int* Engine::getMinimapFovDistancePercent()
{
    return &MINIMAP_FOV_DISTANCE_PERCENT;
}

int* Engine::getMinimapBorderPercentage()
{
    return &MINIMAP_BORFER_PERCENTAGE;
}

int* Engine::getMinimapContentPercentageOffset()
{
    return &MINIMAP_CONTENT_PERCENTAGE_OFFSET;
}

int* Engine::getMinimapSize()
{
    return &MINIMAP_SIZE;
}

int* Engine::getWindowWidth()
{
    return &WINDOW_WIDTH;
}

int* Engine::getWindowHeight()
{
    return &WINDOW_HEIGHT;
}

int* Engine::getPlayerFOV()
{
    return &FOV;
}

int* Engine::getPlayerHalfFOV()
{
    return &H_FOV;
}

float* Engine::getDeltaSeconds()
{
    return &deltaSeconds;
}

int* Engine::getPlayerSpeed()
{
    return &PLAYER_SPEED;
}

int* Engine::getPlayerHeight()
{
    return &PLAYER_HEIGHT;
}

int* Engine::getPlayerRotationSpeed()
{
    return &PLAYER_ROTATION_SPEED;
}

int* Engine::getScreenDist()
{
    return &SCREEN_DIST;
}

sf::RenderWindow* Engine::getWindow()
{
    return &window;
}

void Engine::run()
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

    setupWindow();
    setupFPS();

    level = gameData.getLevelData(0);
    level.printInfo();
    //level.printDetailedInfo();

    LevelBuild levelBuild = LevelBuild(&level, wad);
    levelBounds = levelBuild.getLevelBounds();
    vertexsBounds = levelBuild.getVertexsBounds();
    
    int currentPlayer = 0;

    vector<Player> players;
    for(int i = 1; i < 5; i++)
    {
        players.push_back(Player(*this, levelBuild.getPlayerInfo(i)));
    }
    
    
    BSP bsp = BSP(*this, &players[currentPlayer]);
    MinimapRenderer minimapRenderer = MinimapRenderer(*this, &players[currentPlayer]);
    LevelRenderer levelRenderer = LevelRenderer(*this, &players[currentPlayer]);
    Input input = Input(*this);

    while (window.isOpen())
    {
        cout << " ||| NEW FRAME |||" << endl;

        calculateFPS();                          

        window.clear(sf::Color::Black);

        input.manageInputs(&players[currentPlayer]);

        bsp.renderBsp();
        vector<int>* nodes = bsp.getNodesTree();
        vector<int>* segs = bsp.getSegsTree();
        vector<segmentDrawData>* drawData = bsp.getDrawData();
        
        

        levelRenderer.clearDrawingBoard();
        //levelRenderer.drawSegmentsById(segs);
        levelRenderer.drawData(drawData);
        levelRenderer.update();

        minimapRenderer.drawMinimap();

        
        for(int i =0; i < segs->size(); i++)
        {
            minimapRenderer.drawSegById((*segs)[i]);
        }
        //for(int i =0; i < nodes.size(); i++)
        //{
        //    minimapRenderer.drawNodeById(nodes[i]);
        //}
    
        minimapRenderer.update();

        window.draw(*levelRenderer.getLevel());
        window.draw(*minimapRenderer.getMinimap());
        window.draw(fpsText);
        window.display();       
    }
}



//private

void Engine::setupWindow()
{
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), TITLE);
    window.setSize(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT));
    window.setVerticalSyncEnabled(VSYNC);
    window.setFramerateLimit(FRAMERATE_LIMIT);
    window.setMouseCursorVisible(false);
}

void Engine::setupFPS()
{      
    
    if (!fpsFont.loadFromFile("font.ttf")) {
        // Handle font loading error
        return;
    }

    fpsText.setFont(fpsFont);
    fpsText.setCharacterSize(34);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition(100.0f, 100.0f);
}

void Engine::calculateFPS()
{
    deltaTime = clock.restart();
    deltaSeconds = deltaTime.asSeconds();
    fpsMeasure = 1.0f / deltaSeconds;

    std::ostringstream ss;
    ss << "FPS: " << static_cast<int>(fpsMeasure);
    fpsText.setString(ss.str());
}



// main code

int main()
{
    Engine engine = Engine();
    engine.run();

    return 0;
}





