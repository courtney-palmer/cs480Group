#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

#include "engine.h"


int main(int argc, char **argv)
{
  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Disk Drop");

  if(!engine->Initialize(argv))
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }
  engine->Run();
  delete engine;
  engine = NULL;
  return 0;
}
