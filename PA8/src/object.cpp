#include "object.h"

Object::Object(std::string objFileName, Shape colliShape)
{  
  // LOAD MODEL
  ///////////// -- ADDING ASSIMP STUFF -- /////////////////
  const char *fileName;
  fileName = objFileName.c_str();
  std::string firstPath = "../Assets/Models/";
  std::string fullFilePath = firstPath + fileName;

  std::cout << "Filename: " << fileName << std::endl;

  aiMesh *mesh;
  scene = importer.ReadFile(fullFilePath, aiProcess_Triangulate);
  meshNumber = scene->mNumMeshes; //hold numberof meshes in the scene
  std::cout << "Number of meshes: " << meshNumber << std::endl;
  
  // Retrieve Vertices(position & color) & Indices in each Mesh
  for(unsigned int meshNums = 0; meshNums < meshNumber; meshNums++) //loop through each mesh found
  {
    mesh = scene->mMeshes[meshNums]; //holds current mesh
    meshData.push_back( meshInfo(mesh->mNumFaces*3, Indices.size())); // add 1 mesh to meshData vector & starting index

    aiColor4D colorVal (0.0f, 0.0f, 0.0f, 1.0f); //r, g, b, a, (a controls transparency)
    scene->mMaterials[meshNums +1]->Get(AI_MATKEY_COLOR_DIFFUSE, colorVal); 
    aiMaterial *mtrl; // define a material type (stores materials)
    mtrl = scene->mMaterials[mesh->mMaterialIndex]; //retrieve current mesh materials
    glm::vec3 colorVert (0.0f, 0.0f, 0.0f); // initialize a temporary color vertex

    if(mtrl != NULL)
    {
      if(AI_SUCCESS == aiGetMaterialColor(mtrl, AI_MATKEY_COLOR_DIFFUSE, &colorVal))
      {
        colorVert.x = colorVal.r;
        colorVert.y = colorVal.g;
        colorVert.z = colorVal.b;
      }
      std::cout << "colors for mesh " << meshNums << " is: " << colorVert.x << " "<< colorVert.y << " " << colorVert.z << std::endl;
    }  

    // Get INDICES (and vertices) from MESH
    int faceNumber = mesh->mNumFaces; //holds the number of faces in the current mesh
    std::cout << "Number of Faces for mesh " << meshNums << " is: " << faceNumber << std::endl;

    for(int f = 0; f < faceNumber; f++) //traverse each face, save the 3 indices
    {
      aiFace* face = &mesh->mFaces[f]; // get the current face
      // Use index value to load vertex values from mVertices
      for(int i = 0; i < 3; i++)
      {
        Indices.push_back(face->mIndices[i]);  // push back face indices onto Indices
        // load vertexs for face using mesh indices
        aiVector3D vertVect = mesh->mVertices[Indices.back()]; // get vurrent vertice vector
        glm::vec3 tempPos = glm::vec3(vertVect.x, vertVect.y, vertVect.z); 
        Vertex *tempVertex = new Vertex(tempPos, colorVert); 
        Vertices.push_back(*tempVertex); // push back position and color vector into Vertices
      }
    } // End for : "Get INDICES from Mesh
    std::cout << std::endl;
  } // End for loop "Retrieve Info from Meshes"

  std::cout << "Total Vertices Stored in Object: " << Vertices.size() << std::endl
	    << "Total Indices Stored: " << Indices.size() << std::endl;
  ///////////// -- END OF  ASSIMP STUFF -- /////////////////

  angle = 0.0f;
  
  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
  
  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
  
  // display mesh info for debugging purposes
  for(int i = 0; i < meshData.size(); i++) 
  {
    std::cout << "Mesh " << i << ": " << std::endl
	      << "Mesh Indices: " << meshData[i].meshSize << ", "
	      << "Mesh Start Index " << meshData[i].meshStartIndex << std::endl;
  }

  //////////////////// BULLET STUFF //////////////////////

  // Make collision object that corresponds to the given model
  // Add collision object to physics.dynamicsWorld

  // wip, collision object does not correspond to given model currently
  switch(colliShape) {
  case box:
    shape = new btBoxShape(btVector3(1,1,1));
    break;
  case sphere:
    break;
  case plane:
    break;
  case cylinder:
    break;
  }

  /*i = 0;
  while(!(strcmp(argv[i], "-s") == 0)) //go through arguments until you find -s flag
    i++;
  i++; //next argument is the name name we want
/// ^^^actually let's make a config file ^^^///


  if((strcmp(argv[i], "box") == 0)) //determine what type the shape is
    shapeType = Shape.box;
  if((strcmp(argv[i], "sphere") == 0)) //determine what type the shape is
    shapeType = Shape.sphere;
  if((strcmp(argv[i], "plane") == 0)) //determine what type the shape is
    shapeType = Shape.box;
  if((strcmp(argv[i], "cylinder") == 0)) //determine what type the shape is
    shapeType = Shape.sphere;

  AddShape(&shape, shapeType);*/

  std::cout << "Object fully created probably." << std::endl;
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt)
{
  //model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  // as far as we know, glBindBuffer initializes the buffer
  //glBindBuffer(GL_ARRAY_BUFFER, VB);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  //glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  
  // Draw Each Mesh
  for(int i = 0; i < meshData.size(); i++)
  {
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * meshData[i].meshSize, &Vertices[meshData[i].meshStartIndex], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshData[i].meshSize, &Indices[meshData[i].meshStartIndex], GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
  }
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
