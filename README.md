# 1GAM14-1
*Just something I'm using to motivate myself with.*


## Dependencies
*I might provide commands for linux to set up the required dependencies in future.*
*Same goes for Win32 libs and DLLs.*

- OpenGL
- GLEW
- GLFW3
- libPNG
- FreeType 2.5.2


## Compiling

### Windows
- After getting and installing the dependencies, open up the Visual Studio Solution file at **vs2012/1GAM14-1.sln**.
- You may want to go change the working directory to "**..\\..\\**".
 - **Project -> Properties -> Configuration properties -> Debugging ->Working Directory**
- Compile!

### Linux
- Go to the project root directory and punch in command:
  - ```Shell
    make -C linux```
- The resulting binary can be found from the bin-directory.
- No installation script is provided. Yet.

