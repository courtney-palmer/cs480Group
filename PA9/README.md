# PA9 - Lighting
<!-- comment format for markdown-->
## Group Members
Ashanti Cudiamat<br/><br/>
Courtney Palmer<br/><br/>
Christine Vaughan

## Project Description
In Project 9, we added lighting effects to PA8. We added per-vertex lighting and per-fragment lighting, with the shaders swappable with a keystroke. There is also a spotlight that follows the sphere, and the parameters of the lighting can be adjusted.

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
./PA9 -v shader-v.txt -f shader-f.txt
./PA9 -v vLight-shader-v.txt -f vLight-shader-f.txt
./PA9 -v fLight-shader-v.txt -f fLight-shader-f.txt
```
NOTES: 
<ul>
<li>shader-v.txt and shader-f.txt are provided files in the Assets->Shaders folder. You can also use any vertex shader and fragment shader file when running the program. If new shader files are used, they must be added within Assets->Shaders prior to running the program. The shader-v and shader-f may go in any order, however, the -v flag must immediately precede the specefied vertex shader file and the -f flag must immediately preced the fragment shader file.</li>
<li>There are several .obj and .mtl files provided within the Assets->Models folder. You may use any .obj and .mtl file as long as it has been placed inside the Assets->Models folder.The name of the desired .obj file must be added in the appropriate place in the config file to be added to the program.</li>
<!--<li>There are several textures provided within the Assets->Textures folder. You may use any image file as long as it has been placed inside the Assets->Textures folder.The name of the desired texture must be added in the appropriate place in the config file to be added to the program.</li>
</ul>-->
<!--
A config file is stored in the PA8 folder. This file stores information about the various bodies, including textures, scale factors, orbit speed, and rotation speed.
-->

## Extra Credit

## Controls
### Cube
'up'      | Move forward<br/>
'down'    | Move backward<br/>
'left'    | Move left<br/>
'right'   | Move right<br/>
'space'   | Jump

### Lighting
'q'      | Fragment Lighting <br/>
'e'      | Vertex Lighting <br/>
'y'      | decrease ambient light <br/>
'u'      | increase ambient light<br/>

