#include "object.h"

Object::Object(float baseSc, float baseOS, float baseSS, char** argv)
{
  // LOAD MODEL :: ASSIMP STUFF
  int i = 0;
  while(!(strcmp(argv[i], "-o") == 0)){ //go through arguments until you find -o flag
    i++;
  }
  i++; //next aurgument is the file name we want
  std::string fileName(argv[i]);
  std::cout << "Filename: " << fileName << std::endl;

  // LOAD MODEL
  if( loadModel(fileName) ) { // if loaded successfully
    std::cout << fileName << " model Loaded." << std::endl;
  }

  // LOAD TEXTURES
  i = 0;
  while(!(strcmp(argv[i], "-t") == 0)){ //go through arguments until you find -t flag
    i++;
  }
 
  for(int j = 0; j < meshNumber; j++)
  {
    i++; //next argument is the file name we want
    if ( loadTexture(argv[i]) ) {
      std::cout << argv[i] << " texture Loaded." << std::endl;
    }   
  }

  // Buffer for vertexes
  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  // Buffer for indices
  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
  
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

/**
    This function loads in the .obj file information
    @param std::string objFileName: File name of the .obj file
    @pre none
    @post Indices and Vertices will be filled
    @return bool that indicates success of model loading
*/
bool Object::loadModel(std::string objFileName) {

  // reload scene and meshNumber if needed
  scene = importer.ReadFile("../Assets/Models/" + objFileName, aiProcess_Triangulate);
  meshNumber = scene->mNumMeshes; //hold number of meshes in the scene

  aiMesh *mesh; // holds meshes temporarily for use
  
  // Retrieve Vertices(position & color) & Indices in each Mesh
  for(unsigned int meshNums = 0; meshNums < meshNumber; meshNums++){ //loop through each mesh found

    mesh = scene->mMeshes[meshNums]; //holds current mesh
    meshData.push_back( meshInfo(mesh->mNumFaces*3, Indices.size())); // add 1 mesh to meshData vector & starting index

    aiColor4D colorVal (0.0f, 0.0f, 0.0f, 1.0f); //r, g, b, a, (a controls transparency)
    scene->mMaterials[meshNums +1]->Get(AI_MATKEY_COLOR_DIFFUSE, colorVal); 
    aiMaterial *mtrl; // define a material type (stores materials)
    mtrl = scene->mMaterials[mesh->mMaterialIndex]; //retrieve current mesh materials
    glm::vec3 colorVert (0.0f, 0.0f, 0.0f); // initialize a temporary color vertex
    glm::vec2 textureVert (0.0f, 0.0f); //initialize a temporary texture vertex

    if(mtrl != NULL){
      if(AI_SUCCESS == aiGetMaterialColor(mtrl, AI_MATKEY_COLOR_DIFFUSE, &colorVal)){
        colorVert.x = colorVal.r;
        colorVert.y = colorVal.g;
        colorVert.z = colorVal.b;
      }
    }

    // Get INDICES (and vertices) from MESH
    int faceNumber = mesh->mNumFaces; //holds the number of faces in the current mesh

    for(int f = 0; f < faceNumber; f++){ //traverse each face, save the 3 indices
      aiFace* face = &mesh->mFaces[f]; // get the current face
      // Use index value to load vertex values from mVertices
      for(int i = 0; i < 3; i++) {
        Indices.push_back(face->mIndices[i]);  // push back face indices onto Indices
        if(mesh->HasTextureCoords(0)){
          aiVector3D vert = mesh->mTextureCoords[0][face->mIndices[i]];
          textureVert.x = vert.x;
          textureVert.y = vert.y;
      }
        // load vertexs for face using mesh indices
        aiVector3D vertVect = mesh->mVertices[Indices.back()]; // get current vertices vector
        glm::vec3 tempPos = glm::vec3(vertVect.x, vertVect.y, vertVect.z);

        Vertex *tempVertex = new Vertex(tempPos, colorVert, textureVert); 
        Vertices.push_back(*tempVertex); // push back position and color vector into Vertices
      }

    } // End for : "Get INDICES from Mesh
  } // End for loop "Retrieve Info from Meshes"
    
  return true;
}

/**
    This function loads a texture from a .jpg file using imageMagick
    @param std::string textFileName: File name of the .mtl file
    @pre none
    @post Texture will be bound to the model
    @return bool that indicates success of texture loading
*/
bool Object::loadTexture(std::string textFileName) {

  if(textFileName == std::string("NULL"))
    return false;

  // ADD TEXTURES
  ///////////// -- IMAGE MAGICK -- /////////////////
  //texture = new GLuint[meshNumber];
  textureNames.push_back(textFileName);
  texture = new GLuint[meshNumber];
  unsigned int index = textureNames.size() - 1;

  //load textures from images
  Magick::Blob blob;
  Magick::Image *image = NULL;
  image = new Magick::Image("../Assets/Textures/" + textFileName);
  image->write(&blob, "RGBA");

  if(textFileName.find("Ring") != std::string::npos ||
     textFileName.find("ring") != std::string::npos) {
    
    // Insert custom texture or something?
    
    glGenTextures(1, &texture[index]);
    glBindTexture(GL_TEXTURE_2D, texture[index]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->columns(), image->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
  } else {
    //generate texture in OpenGL
    glGenTextures(1, &texture[index]);
    glBindTexture(GL_TEXTURE_2D, texture[index]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->columns(), image->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    delete image;
  }
  return true;
}

void Object::Update(unsigned int dt, glm::mat4 orbitOrigin)
{
  return;
}

glm::mat4 Object::GetModel()
{
  return model;
}

glm::mat4 Object::GetPosition()
{
  return position;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture));

  // as far as we know, glBindBuffer initializes the buffer
  //glBindBuffer(GL_ARRAY_BUFFER, VB);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  //glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
  
  // Draw Each Mesh
  for(int i = 0; i < meshData.size(); i++) {    
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * meshData[i].meshSize, &Vertices[meshData[i].meshStartIndex], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshData[i].meshSize, &Indices[meshData[i].meshStartIndex], GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
    //bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[i]);
  }

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}

void Object::SetScale(bool scalar)
{
  if(scalar) //if increasing
  {
    if(scaleMult + 0.25f > maxScale) //ensures we don't go over the max limit
      scaleMult = maxScale;
    else
      scaleMult += 0.25f;
  }
  else //if decreasing
  {
    if(scaleMult - 0.25f < minScale) //ensures we don't go under the min limit
      scaleMult = minScale;
    else
      scaleMult -= 0.25f;
  }
}

void Object::SetOrbitSpeed(bool scalar)
{
  if(scalar) //if increasing
  {
    if(orbitSpeedMult + 0.25f > maxSpeed) //ensures we don't go over the max limit
      orbitSpeedMult = maxSpeed;
    else
      orbitSpeedMult += 0.25f;
  }
  else //if decreasing
  {
    if(orbitSpeedMult - 0.25f < minSpeed) //ensures we don't go under the min limit
      orbitSpeedMult = minSpeed;
    else
      orbitSpeedMult -= 0.25f;
  }
}

void Object::SetSpinSpeed(bool scalar)
{
  if(scalar) //if increasing
  {
    if(spinSpeedMult + 0.25f > maxSpeed) //ensures we don't go over the max limit
      spinSpeedMult = maxSpeed;
    else
      spinSpeedMult += 0.25f;
  }
  else //if decreasing
  {
    if(spinSpeedMult - 0.25f < minSpeed) //ensures we don't go under the min limit
      spinSpeedMult = minSpeed;
    else
      spinSpeedMult -= 0.25f;
  }
}

void Object::DisplayModelInfo(const unsigned int maxDisplayLines) {
  unsigned int displayLines = (Indices.size() < maxDisplayLines) ? Indices.size() : maxDisplayLines;
  // Display Vertex position and color information
  for(int i = 0; i < displayLines; i++) {
    std::cout << "Vertex " << i << ": "
	      << Vertices[i].vertex.x << " "
	      << Vertices[i].vertex.y << " "
	      << Vertices[i].vertex.z << std::endl;
	      // << "Color: " 
	      // << Vertices[i].color.x << " "
	      // << Vertices[i].color.y << " "
	      // << Vertices[i].color.z << std::endl;
  }

  // Display Indices in groups of threes aka for each face
  for(int i = 0; i < displayLines; i++) {
    std::cout << i << ":" << Indices[i] << " ";
    if( (i%3) == 0 )
      std::cout << std::endl;
  }
}
