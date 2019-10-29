# PA8: Bullet
<!-- comment format for markdown-->
## Group Members
Ashanti Cudiamat<br/><br/>
Courtney Palmer<br/><br/>
Christine Vaughan

## Project Description
In Project 8, we created a simple 'board' with 3 objects: a sphere, a cube, and a cylinder. The cylinder is a static object while the sphere and cube are dynamic objects. Input is provided so that the the cube can hit the ball to make it move and bounce against the walls of the board and the cylinder. 

## Cmake Instructions
The building of the project is done using CMake, installation with apt-get or brew may be necessary. Later use with CMake and Shader files will be require the copy of a directory where those files are stored (ex. shaders). To do this in the ```add_custom_target``` function place 
```cmake
COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/shaders/ ${CMAKE_CURRENT_BINARY_DIR}/shaders
mkdir build
cd build
cmake ..
make
```

## Command Line Instructions
First, make sure that the files were built using standard cmake procedures [described in the CMake Instructions section].

To run the program, navigate to your executable build folder and open a terminal. Then, run
```
./PA8 -v shader-v.txt -f shader-f.txt
```
NOTES: 
<ul>
<li>shader-v.txt and shader-f.txt are provided files in the Assets->Shaders folder. You can also use any vertex shader and fragment shader file when running the program. If new shader files are used, they must be added within Assets->Shaders prior to running the program. The shader-v and shader-f may go in any order, however, the -v flag must immediately precede the specefied vertex shader file and the -f flag must immediately preced the fragment shader file.</li>
<li>There are several .obj and .mtl files provided within the Assets->Models folder. You may use any .obj and .mtl file as long as it has been placed inside the Assets->Models folder.The name of the desired .obj file must be added in the appropriate place in the config file to be added to the program.</li>
<li>There are several textures provided within the Assets->Textures folder. You may use any image file as long as it has been placed inside the Assets->Textures folder.The name of the desired texture must be added in the appropriate place in the config file to be added to the program.</li>
</ul>
<!--
A config file is stored in the PA8 folder. This file stores information about the various bodies, including textures, scale factors, orbit speed, and rotation speed.
-->

## Extra Credit
A triangle mesh was used to load in the board/tray that holds all the other objects. The implementation can be seen in:
object.cpp 
- Object(std::string objFileName, const ShapeInfo& newShape)
- bool loadModel(std::string objFileName)
engine.cpp
- bool Initialize(int argc, char** argv);

The tray is loaded as a triangle mesh into Bullet.

## Controls
'up'      | Move forward<br/>
'down'    | Move backward<br/>
'left'    | Move left<br/>
'right'   | Move right<br/>
'space'   | Jump

