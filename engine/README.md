# GAME ENGINE

### folder strcuture:

```tree

engine
│
├── include
│   ├── Utils
│   ├── GameData
│   ├── Renderer
│   └── Controller
│
├── src
│   ├── Utils
│   ├── GameData
│   ├── Renderer
│   └── Controller
│
│
├── engine.cpp
├── engine.h
└── README.md

```

### compilation and running:

compile & build:

> ⚠️ **You need to use g++ v11 to compile this project** (it wont work on older versions)


#### manual:

    ```bash
    g++ -O3 *.cpp src/GameData/*.cpp src/Controller/*.cpp src/Renderer/*.cpp src/Utils/*.cpp -o game -lm -lasound -lSDL2 -lSDL2main -lsfml-graphics -lsfml-window -lsfml-system
    ```

    run:
    ```bash
    ./game
    ```

#### vscode:
    1. install code runner extension
    2. edit settings.json
    3.
    ```json
    "code-runner.executorMap": {
        "cpp": "cd $dir && g++ -O3 *.cpp src/GameData/*.cpp src/Controller/*.cpp src/Renderer/*.cpp src/Utils/*.cpp -o $fileNameWithoutExt -lm -lasound -lSDL2 -lSDL2main -lsfml-graphics -lsfml-window -lsfml-system && $dir$fileNameWithoutExt"
    }
    ```

### external libs:

* [SDL2](https://www.libsdl.org/)
* [port audio](http://portaudio.com/docs/v19-doxydocs/compile_linux.html)
* [SFML](https://www.sfml-dev.org/)
* [google tests](https://github.com/google/googletest)
* [g++ v11](https://stackoverflow.com/questions/67298443/when-gcc-11-will-appear-in-ubuntu-repositories)


### instaliation of external libs:

#### google tests

```sh
sudo apt-get install libgtest-dev
cd /usr/src/gtest
sudo cmake -Bbuild
sudo cmake --build build
sudo cp ./build/lib/libgtest* /usr/lib
```

#### SFML
```sh
sudo apt-get install libsfml-dev
```