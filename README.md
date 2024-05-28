# GUI for Ollama

**Requirements:**
- Qt6.7+ ([Install Qt on linux](https://doc.qt.io/qt-6/linux.html) | [Install Qt on MacOs](https://doc.qt.io/qt-6/macos.html) or ```brew install qt```)
- [Ollama](https://ollama.com/)  
- CMake

------ 

**Build and run:**

Make sure the Ollama server is running, with ```ollama serve```. Then:

```
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
$ ./ollamagui
```  

The application tries to connect to an ollama server instance on ```127.0.0.1``` port ```11434``` as a default, this can be changed in the UI.

------

So far tested platforms: 
- Linux x86_64, X11
- MacOS 13.3.1, x86_64