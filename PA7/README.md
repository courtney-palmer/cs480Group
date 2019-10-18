# PA7: Solar System

## Group Members
Ashanti Cudiamat<br/><br/>
Courtney Palmer<br/><br/>
Christine Vaughan

## Project Description
In Project 7, we created a model solar system. The origin hosts the sun, and all nine planets orbit it and rotate at each planets' respective speeds. The planets have the appropriate number of moons, which also rotate and orbit their planets.

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
./PA7 -v shader-v.txt -f shader-f.txt
```
NOTE: <br/><br/>
       shader-v.txt and shader-f.txt are provided files in the Assets->Shaders folder. You can also use any vertex shader and fragment shader file when running the program. If new shader files are used, they must be added within Assets->Shaders prior to running the program. The shader-v and shader-f may go in any order, however, the -v flag must immediately precede the specefied vertex shader file and the -f flag must immediately preced the fragment shader file. <br/><br/>
       
       There are several .obj and .mtl files provided within the Assets->Models folder. You may use any .obj and .mtl file as long as it has been placed inside the Assets->Models folder.The name of the desired .obj file must be added in the appropriate place in the config file to be added to the program.<br/><br/>

       There are several textures provided within the Assets->Textures folder. You may use any image file as long as it has been placed inside the Assets->Textures folder.The name of the desired texture must be added in the appropriate place in the config file to be added to the program.<br/><br/>
       
A config file is stored in the PA7 folder. This file stores information about the various bodies, including textures, scale factors, orbit speed, and rotation speed.

## Extra Credit
Simulation speed can be adjusted with up and down arrow keys.
Program uses a configuration file, located in the PA7 folder.
Rings are on Jupiter, Uranus, and Neptune.

## Controls
'0' - '9' | Switch Planet Focus<br/>
'q'       | Solar System Zoomed in<br/>
'w'       | Solar System Reset<br/>
'e'       | Solar System Zoom out<br/>
'up'      | Speed up simulation<br/>
'down'    | Slow down simulation

## References
All textures were provided by http://planetpixelemporium.com/planets.html <br/>
Planet info was provided by https://nssdc.gsfc.nasa.gov/planetary/factsheet/planet_table_ratio.html and altered for use in this program.
Moon info provided by https://www.windows2universe.org/our_solar_system/moons_table.html  and altered for use in this program.

