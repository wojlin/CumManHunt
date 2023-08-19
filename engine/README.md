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
└── README.md

```

### compilation and running:

compile & build:

```bash
g++ engine.cpp src/GameData/*.cpp -o game -lm -lasound -lSDL2 -lSDL2main
```

run:
```
./game
```

### external libs:

* [SDL2](https://www.libsdl.org/)
* [port audio](http://portaudio.com/docs/v19-doxydocs/compile_linux.html)
* [SFML](https://www.sfml-dev.org/)
* [google tests](https://github.com/google/googletest)


### instaliation of external libs:

#### google tests

```sh
sudo apt-get install libgtest-dev
cd /usr/src/gtest
sudo cmake -Bbuild
sudo cmake --build build
sudo cp ./build/lib/libgtest* /usr/lib
```