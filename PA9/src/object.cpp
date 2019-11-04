#include "object.h"

/*
  old constructor : does not allow you to specify shape of collision object at all
  See next constructor for updated version
 */
Object::Object(std::string objFileName, Shape colliShape)
{
//////////////////// BULLET STUFF //////////////////////
  switch(colliShape) {
  case box:
    shape = new btBoxShape(btVector3(1,1,1));
    break;
  case sphere:
    shape = new btSphereShape(btScalar(1));
    break;
  case plane:
  //shape = new btStaticPlaneShape(btVector3(0, 1, 0), -10);
    shape = new btBoxShape(btVector3(100, 1, 100)); // WIP CHANGE TO PLANE SPECIFIC SHAPE
    break;
  case cylind:
    shape = new btCylinderShape(btVector3(1,1,1));
    break;
  case mesh:
    // if mesh, the object cannot be a dynamic object, and it has to be
    // initialized 
    //shape = new btTriangleMesh();
    break;
  }

  // LOAD MODEL
  if(loadModel(objFileName)) {
    //std::cout << "Model loaded." << std::endl;
  }
  
  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
  
  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

  //velocity = btVector3(0.0f, 0.0f, 0.0f);
  
  showMeshData();

  /*i = 0;
  while(!(strcmp(argv[i], "-s") == 0)) //go through arguments until you find -s flag
    i++;
  i++; //next argument is the name name we want
/// ^^^actually let's make a config file ^^^///
*/
}

Object::Object(std::string objFileName, const ShapeInfo& newShape)
{
//////////////////// BULLET STUFF //////////////////////
  // Make collision object that corresponds to the given model
  // Add collision object to physics.dynamicsWorld
  // wip, collision object does not correspond to given model currently
  objTriMesh = nullptr;
  shape = nullptr;
  switch(newShape.shapeName) {
  case box:
    shape = new btBoxShape(newShape.getBtVector3()); // 1 1 1
    break;
  case sphere:
    shape = new btSphereShape(newShape.extents[0]);
    break;
  case plane:
    shape = new btBoxShape(newShape.getBtVector3());
    break;
  case cylind:
    shape = new btCylinderShape(newShape.getBtVector3());
    break;
  case mesh:
    // if mesh, the object cannot be a dynamic object, and it has to be
    // initialized. See loadModel() for loading in the mesh
    // particularly the parts w/ if (objTriMesh != nullptr)
    objTriMesh = new btTriangleMesh();
    break;
  }

  // LOAD MODEL
  if(loadModel(objFileName)) {
    //    std::cout << "Model loaded." << std::endl;
  }

  // Set up vertices and indices for rendering this object
  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
  
  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

  //velocity = btVector3(0.0f, 0.0f, 0.0f);
  
  //  showMeshData(); // Used for debugging.
  return;
}

/*
  This loads in .obj file information in order to render the object
  @param objFileName : std::string : file name of obj file
  @pre objFileName is assumed to be somewhere in ../Assets/Models/.
  @post Indices, Vertices(, objTriMesh if shape is mesh) will be filled w/ model info
 */
bool Object::loadModel(std::string objFileName) {
    ///////////// -- ADDING ASSIMP STUFF -- /////////////////
  const char *fileName;
  fileName = objFileName.c_str();
  std::string firstPath = "../Assets/Models/";
  std::string fullFilePath = firstPath + fileName;

  aiMesh *mesh;
  scene = importer.ReadFile(fullFilePath, aiProcess_Triangulate);
  meshNumber = scene->mNumMeshes; //hold numberof meshes in the scene
  //std::cout << "Number of meshes: " << meshNumber << std::endl;
  
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
    glm::vec3 normVert (0.0f, 0.0f, 0.0f); //initialize a temporary normal vertex

    if(mtrl != NULL)
    {
      if(AI_SUCCESS == aiGetMaterialColor(mtrl, AI_MATKEY_COLOR_DIFFUSE, &colorVal))
      {
        colorVert.x = colorVal.r;
        colorVert.y = colorVal.g;
        colorVert.z = colorVal.b;
      }
      // std::cout << "colors for mesh " << meshNums << " is: " << colorVert.x << " "<< colorVert.y << " " << colorVert.z << std::endl;
    }  

    // Get INDICES (and vertices) from MESH
    int faceNumber = mesh->mNumFaces; //holds the number of faces in the current mesh

    for(int f = 0; f < faceNumber; f++) //traverse each face, save the 3 indices
    {
      aiFace* face = &mesh->mFaces[f]; // get the current face

      btVector3 triArray[3]; // For use w/ bullet triangle mesh
      
      // Use index value to load vertex values from mVertices
      for(int i = 0; i < 3; i++)
      {
        Indices.push_back(face->mIndices[i]);  // push back face indices onto Indices
        // load vertexs for face using mesh indices
        aiVector3D vertVect = mesh->mVertices[Indices.back()]; // get current vertice vector
        if(mesh->HasNormals())
        {
          aiVector3D vert = mesh->mNormals[Indices.back()];
          normVert.x = vert.x;
          normVert.y = vert.y;
          normVert.z = vert.z;
        }
	// if collision shape is triangle mesh, load indices into there too
        if(objTriMesh != nullptr) {
	  // load model info into bullet triangle mesh
	  triArray[i] = btVector3(vertVect.x, vertVect.y, vertVect.z);
	}
	
        glm::vec3 tempPos = glm::vec3(vertVect.x, vertVect.y, vertVect.z); 
        Vertex *tempVertex = new Vertex(tempPos, colorVert, normVert); 
        Vertices.push_back(*tempVertex); // push back position and color vector into Vertices
      } // End for : "Process every triangle face and store into indices and vertices

      if(objTriMesh != nullptr) {
	objTriMesh->addTriangle(triArray[0], triArray[1], triArray[2]);
      }

    } // End for : "Get INDICES from Mesh

  } // End for loop "Retrieve Info from Meshes"

  ///////////// -- END OF  ASSIMP STUFF -- /////////////////

  if(objTriMesh != nullptr) {
    shape = new btBvhTriangleMeshShape(objTriMesh, true);
    std::cout << "Shape for triangle mesh succesfully assigned" << std::endl;
  }
  
  return true;
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
  delete scene;
  delete shape;
  delete RBody;
  delete physicsObject;
}

/*
  model is set to identity matrix every update so the same transformation
  is not consistently applied?
 */
void Object::Update()
{
  model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
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

void Object::showMeshData() const {
// display mesh info for debugging purposes
  for(int i = 0; i < meshData.size(); i++) 
  {
    std::cout << "Mesh " << i << ": " << std::endl
	      << "Mesh Indices: " << meshData[i].meshSize << ", "
	      << "Mesh Start Index " << meshData[i].meshStartIndex << std::endl;
  }
}
