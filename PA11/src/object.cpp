#include "object.h"

/*
  Constructor
  @param std::string objFileName: the name of the .obj file to load
  @param ShapeInfo&  newShape   : struct storing info to initialize
                                  this object for use w/ bullet
  @pre objFileName & ShapeInfo should be arguments that exist.
  @post Initialize Vertices, Indices, btCollisionObject
        Generates buffer for VB, IB
 */
Object::Object(const std::string& objFileName, const ShapeInfo& newShape,
	       const std::string& key, const std::string& texFileName)
{
  textured = false;
  keyname = key;

//////////////////// BULLET STUFF //////////////////////
  objTriMesh = nullptr; // Used to store btTriangleMesh if it will be used
  shape = nullptr;      // Will be used to store collision shape when initialized // btCollisionShape* shape
  switch(newShape.shapeName) {
    case box:
      shape = new btBoxShape(newShape.getBtVector3()); // 1 1 1
      physicsObject = new btCollisionObject();
      break;
    case sphere:
      shape = new btSphereShape(newShape.extents[0]);
      physicsObject = new btCollisionObject();
      break;
    case plane:
      shape = new btBoxShape(newShape.getBtVector3());
      physicsObject = new btCollisionObject();
      break;
    case cylind:
      shape = new btCylinderShape(newShape.getBtVector3());
      physicsObject = new btCollisionObject();
      break;
    case mesh:
      // if mesh, the object cannot be a dynamic object, and it has to be
      // initialized. See loadModel() for loading in the mesh
      // particularly the parts w/ if (objTriMesh != nullptr)
      objTriMesh = new btTriangleMesh();
      physicsObject = new btCollisionObject();
      physicsObject->setCollisionShape(shape);
      break;

      // Basically implementing the option of ghostObjects could make collisions really convenient if it gets working
      // See src/BulletCollision/CollisionDispatch/btGhostObject.h in bullet github for methods
      //     btGhostObject can be used to quickly detect if _anything_ is colliding with it through function 
      //     getNumOverlappingObjects() which returns the number of objects overlapping with the ghost object
    case ghostObject_mesh: // Added object to make this a ghostObject
      physicsObject = new btGhostObject();
      objTriMesh = new btTriangleMesh();
      shape = new btBoxShape(btVector3(2,2,2)); // filler so shape is not nullptr wIP
      physicsObject->setCollisionShape(shape); // shape should still be nullptr at this point
      break;
  }

  // LOAD MODEL
  if(!loadModel(objFileName)) {
    std::cout << "Model not loaded." << std::endl;
  }

  if(texFileName == NA) {
    std::cout << "No Texture to load." << std::endl;
    textured = false;
  }
  else {
    //std::cout << "Loading " << texFileName << std::endl;
    if(!loadTexture(texFileName)) {
      std::cout << "Texture failed to load." << std::endl;
    }
    else
      textured = true;
  }

  //if(newShape.shapeName == mesh)
  // physicsObject = new btCollisionObject();
  // physicsObject->setCollisionShape(shape);
  // physicsObject->setUserPointer((void*)this);

  // Set up vertices and indices for rendering this object
  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
  
  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);  
  //showMeshData(); // Used for debugging.
  return;
}

/*
  This loads in .obj file information in order to render the object
  @param objFileName : std::string : file name of obj file
  @pre objFileName is assumed to be somewhere in ../Assets/Models/.
  @post Indices, Vertices(, objTriMesh if shape is mesh) will be filled w/ model info
  @UsedIn Object::Object(...)
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
  //  std::cout << "Number of meshes: " << meshNumber << std::endl; // DEBUG
  
  // === Retrieve Vertices(position & color) & Indices in each Mesh ===
  for(unsigned int meshNums = 0; meshNums < meshNumber; meshNums++)
  {
    mesh = scene->mMeshes[meshNums]; // retrieve current mesh from scene
    // Store mesh size and the mesh starting index (as stored in Vertices)
    meshData.push_back( meshInfo(mesh->mNumFaces*3, Indices.size())); 

    //    std::cout << "Mesh " << meshNums << " num vertices: " << mesh->mNumFaces*3
    //	      << " & Starting index: " << Indices.size() << std::endl; // DEBUG

    // Temporary vertexes are buffers between assimp input to object
    glm::vec3 colorVert (0.0f, 0.0f, 0.0f); // initialize a temporary color vertex
    glm::vec3 normVert (0.0f, 0.0f, 0.0f); //initialize a temporary normal vertex
    glm::vec2 uvVert (0,0); // initialize temporary uv vertex

    // === Obtain Color values from .mtl if relevant ===
    aiColor4D colorVal (0.0f, 0.0f, 0.0f, 1.0f); //r, g, b, a, (a controls transparency)
    scene->mMaterials[meshNums +1]->Get(AI_MATKEY_COLOR_DIFFUSE, colorVal); 
    aiMaterial *mtrl; // define a material type (stores materials)
    mtrl = scene->mMaterials[mesh->mMaterialIndex]; //retrieve current mesh materials

    if(mtrl != NULL)
    {
      if(AI_SUCCESS == aiGetMaterialColor(mtrl, AI_MATKEY_COLOR_DIFFUSE, &colorVal))
      {
        colorVert.x = colorVal.r;
        colorVert.y = colorVal.g;
        colorVert.z = colorVal.b;
      }
    }  

    // === Get INDICES (and vertices) from MESH ===
    int faceNumber = mesh->mNumFaces; //holds the number of faces in the current mesh

    for(int f = 0; f < faceNumber; f++) //traverse each face, save the 3 indices
    {
      aiFace* face = &mesh->mFaces[f]; // store current face from assimp mesh
      btVector3 triArray[3]; // For use w/ bullet triangle mesh
      
      // Use index value to load vertex values from mVertices
      for(int i = 0; i < 3; i++)
      {

        Indices.push_back(face->mIndices[i]);  // push back face indices onto Indices
        // load vertexs for face using mesh indices
        aiVector3D vertVect = mesh->mVertices[Indices.back()]; // get current vertice vector

	// load UVs
	if(mesh->HasTextureCoords(0)){
          aiVector3D vert = mesh->mTextureCoords[0][face->mIndices[i]];
          uvVert.x = vert.x;
          uvVert.y = vert.y;
	}
	
	// load Normals
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
        Vertex *tempVertex = new Vertex(tempPos, colorVert, normVert, uvVert); 
        Vertices.push_back(*tempVertex); // push back position and color vector into Vertices

      } // End for : "Process every triangle face and store into indices and vertices

      // If the object type was a mesh then triArray[3] should have been filled, so store new triangle in the obj mesh
      if(objTriMesh != nullptr) {
	objTriMesh->addTriangle(triArray[0], triArray[1], triArray[2]);
      }

    } // End for : "Get INDICES from Mesh
  } // End for loop "Retrieve Info from Meshes"

  ///////////// -- END OF  ASSIMP STUFF -- /////////////////

  if(objTriMesh != nullptr) {
    shape = new btBvhTriangleMeshShape(objTriMesh, true);
    //std::cout << "Shape for triangle mesh succesfully assigned" << std::endl;
  }
  
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

  //generate texture in OpenGL
  glGenTextures(1, &texture[index]);
  glBindTexture(GL_TEXTURE_2D, texture[index]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->columns(), image->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  delete image;

  return true;
}

Object::~Object()
{
  meshData.clear();
  Vertices.clear();
  Indices.clear();
  delete scene;

  delete texture;

  delete physicsObject;
  delete shape;
  delete objTriMesh;
  delete RBody;
}

/*
  Obsolete
 */
void Object::Update()
{
  //model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
}

/*
  Returns model in 4x4 matrix form
 */
glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render(int numOfElements)
{
  glEnableVertexAttribArray(0); // position attribute
  glEnableVertexAttribArray(1); // color attribute
  glEnableVertexAttribArray(2); // normal
  glEnableVertexAttribArray(3); // texture
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); //position
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color)); //color
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal)); //normal
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture)); // texture
  
  // Draw Each Mesh
  for(int i = 0; i < meshData.size(); i++)
  {
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * meshData[i].meshSize, &Vertices[meshData[i].meshStartIndex], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshData[i].meshSize, &Indices[meshData[i].meshStartIndex], GL_STATIC_DRAW);

    // Apply texture
    if(textured) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture[i]);
    }
    
    //glenum mode, glsizei count, glenum type, const void* indices, number of objects
    glDrawElementsInstanced(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0, numOfElements);
  }

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
}

/*
  display mesh info for debugging purposes
 */
void Object::showMeshData() const {
  for(int i = 0; i < meshData.size(); i++) 
  {
    std::cout << "Mesh " << i << ": " << std::endl
	      << "Mesh Indices: " << meshData[i].meshSize << ", "
	      << "Mesh Start Index " << meshData[i].meshStartIndex << std::endl;
  }
}
