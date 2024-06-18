# GUI for Ollama

**Requirements:**
- Qt6.7+ ([Install Qt on linux](https://doc.qt.io/qt-6/linux.html) | [Install Qt on MacOs](https://doc.qt.io/qt-6/macos.html) or ```brew install qt```)
- [Ollama](https://ollama.com/)  
- CMake

------ 

**Build and run:**

Make sure the Ollama server is running on the host machine, with ```ollama serve```. Then:

```
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
$ ./ollamagui
```  

The application will ask for the desired server instance to connect to on opening. The default is ```localhost``` on port ```11434```. If you wish to connect to a different host within you local network, note that the ```OLLAMA_HOST``` environmental variable has to be modified on the host machine.

------

So far tested platforms: 
- Linux x86_64 (X11 AwesomeWM Arch)
- MacOS 13.3.1 x86_64


**Preview**

https://github.com/akamlah/ollama-gui/assets/87025819/017ce4e6-87d3-4c57-bf7e-0941c2e4cf19

