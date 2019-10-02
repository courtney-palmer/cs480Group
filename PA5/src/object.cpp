#include "object.h"

Object::Object(bool moon, float baseSc, float baseOS, float baseSS, char** argv)
{  
  // LOAD MODEL
  ///////////// -- ADDING ASSIMP STUFF -- /////////////////
  std::string s;
  int i = 0;
  while(!(strcmp(argv[i], "-o") == 0)){ //go through arguments until you find -o flag
    i++;
  }
  i++; //next argument is the file name we want
  std::string fileName(argv[i]);
  std::cout << "Filename: " << fileName << std::endl;

  int faceNumber;
  aiMesh *mesh;


  scene = importer.ReadFile("../Assets/model/" + fileName, aiProcess_Triangulate);
  meshNumber = scene->mNumMeshes; //hold numberof meshes in the scene
  std::cout << "Number of meshes: " << meshNumber << std::endl;
  
  aiColor3D color (0.0f, 0.0f, 0.0f); //unsure how to utilize aiColor3d type. 

  // NOTES: The following for loop captures Vertices (position, color) and captures
  // indices. We still need to seperate out the 3 indices
  // The 3 indices seem to represent the 3 vertex for each face - ash

  // Retrieve Vertices(position & color) & Indices in each Mesh
  for(unsigned int meshNums = 0; meshNums < meshNumber; meshNums++){ //loop through each mesh found
    mesh = scene->mMeshes[meshNums]; //holds current mesh
    scene->mMaterials[meshNums +1]->Get(AI_MATKEY_COLOR_DIFFUSE, color); 

    // Get INDICES (and vertices) from MESH
    faceNumber = mesh->mNumFaces; //holds the number of faces in the current mesh
    std::cout << "Number of Faces for mesh " << meshNums << " is: " << faceNumber << std::endl;

    for(int f = 0; f < faceNumber; f++){ //traverse each face, save the 3 indices
      aiFace* face = &mesh->mFaces[f];  		// get the current face

      // Use index value to load vertex values from mVertices
      for(int i = 0; i < 3; i++) {
	Indices.push_back(face->mIndices[i]);  // push back face indices onto Indices
	// load vertexs for face using mesh indices
	aiVector3D vertVect = mesh->mVertices[Indices.back()]; // get vurrent vertice vector
	glm::vec3 tempPos = glm::vec3(vertVect.x, vertVect.y, vertVect.z); 
	glm::vec3 tempColor = {1.0f, 1.0f, 1.0f}; // eventually want to use the aiColor3D type
	Vertex *tempVertex = new Vertex(tempPos, tempColor); 
	Vertices.push_back(*tempVertex); // push back position and color vector into Vertices
      }

    } // End for : "Get INDICES from Mesh

  } // End for loop "Retrieve Info from Meshes"

  std::cout << "Total Vertices Stored in Object: " << Vertices.size() << std::endl
	    << "Total Indices Stored: " << Indices.size() << std::endl;
  ///////////// -- END OF  ASSIMP STUFF -- /////////////////

  isMoon = moon;
  angleOrbit = 0.0f;
  angleSelf = 0.0f;

  pausedOrbit = false;
  pausedSpin = false;
  reversedOrbit = false;
  reversedSpin = false;
  position = glm::mat4(1.0f);

  baseScale = baseSc;
  scaleMult = 1.0f; //scales up/down 0.25 w/ each keypress
  maxScale = 3.0f;
  minScale = 0.25f; 

  baseOrbitSpeed = baseOS;
  baseSpinSpeed = baseSS;
  orbitSpeedMult = 1.0f;  //scales up/down 0.25 w/ each keypress
  spinSpeedMult = 1.0f;  //scales up/down 0.25 w/ each keypress
  maxSpeed = 3.0f;
  minSpeed = 0.25f;

  //DisplayModelInfo();
  
  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  // Statement loads only the platform into buffer
  //glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6, &Vertices[300000], GL_STATIC_DRAW);
  

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

  // Statement loads only the platform into buffer
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, &Indices[300000], GL_STATIC_DRAW);
  

  
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt, glm::mat4 orbitOrigin)
{
  if(!pausedOrbit)
  {
    if(reversedOrbit)
      angleOrbit -= dt * M_PI/(baseOrbitSpeed / orbitSpeedMult); //the angle of the object's orbit
   else
      angleOrbit += dt * M_PI/(baseOrbitSpeed / orbitSpeedMult); //the angle of the object's orbit
  }
  if(!pausedSpin)
  {
    if(reversedSpin)
      angleSelf -= dt * M_PI/(baseSpinSpeed / spinSpeedMult); //the angle of the object's orbit
   else
      angleSelf += dt * M_PI/(baseSpinSpeed / spinSpeedMult); //the angle of the object's orbit
  }

  position = glm::translate(orbitOrigin, glm::vec3((5.0f * sin(angleOrbit)), 0.0f, (5.0f * cos(angleOrbit)))); //translates cube about the designated orbitOrigin
  glm::mat4 rotSelf = glm::rotate(glm::mat4(1.0f), (angleSelf), glm::vec3(0.0, 1.0, 0.0)); //sets the cube's rotation about its center y-axis
  glm::mat4 scaleMat = glm::scale(glm::vec3((scaleMult * baseScale), (scaleMult * baseScale), (scaleMult * baseScale))); //set the scale of the object

  model = position * rotSelf * scaleMat; //multiply matrices to apply effects to the model
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
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  // as far as we know, glBindBuffer initializes the buffer
  //glBindBuffer(GL_ARRAY_BUFFER, VB);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  //glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  // Draw mesh1?
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  // Draw mesh2?
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6, &Vertices[300000], GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, &Indices[300000], GL_STATIC_DRAW);
  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void Object::SetScale(bool scalar)
{
  if(scalar) //if increasing
  {
    if(scaleMult + 0.25f > maxScale) //be sure not to go over the max limit
      scaleMult = maxScale;
    else
      scaleMult += 0.25f;
  }
  else //if decreasing
  {
    if(scaleMult - 0.25f < minScale) //be sure not to go under the min limit
      scaleMult = minScale;
    else
      scaleMult -= 0.25f;
  }
}

void Object::SetOrbitSpeed(bool scalar)
{
  if(scalar) //if increasing
  {
    if(orbitSpeedMult + 0.25f > maxSpeed) //be sure not to go over the max limit
      orbitSpeedMult = maxSpeed;
    else
      orbitSpeedMult += 0.25f;
  }
  else //if decreasing
  {
    if(orbitSpeedMult - 0.25f < minSpeed) //be sure not to go under the min limit
      orbitSpeedMult = minSpeed;
    else
      orbitSpeedMult -= 0.25f;
  }
}

void Object::SetSpinSpeed(bool scalar)
{
  if(scalar) //if increasing
  {
    if(spinSpeedMult + 0.25f > maxSpeed) //be sure not to go over the max limit
      spinSpeedMult = maxSpeed;
    else
      spinSpeedMult += 0.25f;
  }
  else //if decreasing
  {
    if(spinSpeedMult - 0.25f < minSpeed) //be sure not to go under the min limit
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
	      << Vertices[i].vertex.z << std::endl
	      << "Color: " 
	      << Vertices[i].color.x << " "
	      << Vertices[i].color.y << " "
	      << Vertices[i].color.z << std::endl;
  }

  // Display Indices in groups of threes aka for each face
  for(int i = 0; i < displayLines; i++) {
    std::cout << i << ":" << Indices[i] << " ";
    if( (i%3) == 0 )
      std::cout << std::endl;
  }
}
