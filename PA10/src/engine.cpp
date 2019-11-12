
#include "engine.h"
#include "camera.h"

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize(char **argv)
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("window failed to initialize.\n");
    return false;
  }
  std::cout << "Window loaded" << std::endl;
  
  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, argv)) //initialize new instance of Objects here
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }
  std::cout << "Graphics loaded" << std::endl;
 
// Start the physics
   m_physics = new Physics();
   if(!m_physics->Initialize()) //set the 5 components of a physics world here
   {
     printf("The physics failed to initialize.\n");
     return false;
   }
   std::cout << "Physics loaded" << std::endl;

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  ballsRemaining = MAX_BALLS;
  zCoordTrigger = 10.0f;


  // ============= Create the objects ===============================
  // note : Objects in obj array have graphics and physics components
  //        automatically updated and rendered
  //        in Engine::Run()
  // Format is as follows: 
  // create ShapeInfo struct > use struct to initialize object
  // push object into objs array  > add object to physics world  

  // add moveable cube
  struct ShapeInfo info(box, 1, 1, 1); // 1,1,1 represents size of collision shape
  Object* temp = new Object("cubeTest.obj", info, "block", "wood.jpg"); // temp holds next object to be stored
  objs.push_back(temp);
  m_physics->AddShape(temp,   // pass in pointer to the object you just created
		      0,2,0, // 0,10,0 represents starting position of object
		      1);  // this value is static vs dynamic. true - dynamic
  // Used in Keyboard() to refer to array index of object that will be moved
  dynamicCubeIndex = objs.size()-1;
  
// // add a temporary lamp
//   struct ShapeInfo lampInfo(box, 1, 1, 1);
//   temp = new Object("cubeTest.obj", lampInfo);
//   objs.push_back(temp);
//   m_physics->AddShape(temp,
// 		      15, 15, 10,
// 		      false);

  std::cout << "Adding Board\n";
  // add board/platform : static
  // struct ShapeInfo boardInfo(mesh);
  // temp = new Object("board.obj", boardInfo);
  // objs.push_back(temp);
  // m_physics->AddShape(temp,
	// 	      0, 0, 0,
	// 	      false);

  // // Add invisible wall on top
  // temp = new Object("board.obj", boardInfo);
  // m_physics->AddShape(temp,
	// 	      0,0,0,
	// 	      false);

  // Add walls : Static
  struct ShapeInfo wallInfo(mesh);
  temp = new Object("board.obj", wallInfo, "board", "harris.jpg");
  objs.push_back(temp);
  m_physics->AddShape(temp,
		      0, 0, 0,
		      3);
  //add thin box to be ball loss trigger
  // struct ShapeInfo lossTrigInfo(mesh);
  // temp = new Object("collisionDetection.obj", lossTrigInfo);
  // objs.push_back(temp);
  // m_physics->AddShape(temp,
	// 	      0, 1, 0,
	// 	      false);
  // trigIndex = objs.size()-1;
  //objs[trigIndex]->physicsObject->setUserPointer(lossTag);

  // Add ball
  struct ShapeInfo ballInfo(sphere, 0.5, 0.5, 0.5);
  temp = new Object("pinball.obj", ballInfo, "pinball", "wood.jpg");
  objs.push_back(temp);
  m_physics->AddShape(temp,
		     7,0,3,
		     1);
  ballIndex = objs.size()-1;
  //objs[ballIndex]->physicsObject->setUserPointer(ballTag);

  // Add cylinder
  // struct ShapeInfo cylindInfo(cylind, 1, 1, 1);
  // temp = new Object("trueCyliner.obj", cylindInfo);
  // objs.push_back(temp);
  // m_physics->AddShape(temp,
	// 	     0,0,15,
	// 	     true);


  // std::cout << "adding paddles" << std::endl;
  // struct ShapeInfo lPaddleInfo(mesh);
  // temp = new Object("LeftPaddle.obj", lPaddleInfo);
  // objs.push_back(temp);
  //   m_physics->AddShape(temp,
	// 	      0, 0, 0,
	// 	      false);

  struct ShapeInfo rPaddleInfo(box, 1,1,1);
  temp = new Object("testLPaddle.obj", rPaddleInfo, "rPaddle");
  objs.push_back(temp);
    m_physics->AddShape(temp,
		      0, 0, 0,
		      2);
  rPaddleIndex = objs.size()-1;

  // ========================= End Object Creation :> =================

  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
    }

    // Update physics
    m_physics->Update();
    //m_physics->Update(objs, ballIndex, trigIndex);

    // Update Graphics, send in physics instance and each single object.
    for(int i = 0; i < objs.size(); i++) {
      m_graphics->Update(m_physics, objs[i]);
    }

    // Render, send in objs vector array
    m_graphics->Render(objs);

    // Check to see if a ball has been lost
    // if(m_physics->lostBall = true)
    //   LoseBall();

    // btCollisionObject* obj = objs[ballIndex]->physicsObject;
    // btRigidBody* body = btRigidBody::upcast(obj);
    // btTransform trans;

    // if(body && body->getMotionState()) {
    //   body->getMotionState()->getWorldTransform(trans);
    // }
    // else {
    //   trans = obj->getWorldTransform();
    // }

    // std::cout << "World position of object: " << float(trans.getOrigin().getX()) << " "
	  //     << trans.getOrigin().getY() << " "
	  //     << trans.getOrigin().getZ() << std::endl;

    // if(trans.getOrigin().getZ() >= zCoordTrigger)
    // {
    //   //lose a ball
    //   LoseBall();
    // }

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard()
{
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  else if (m_event.type == SDL_KEYDOWN)
  {
    vel = btVector3(objs[dynamicCubeIndex]->RBody->getLinearVelocity().getX(), 
                    objs[dynamicCubeIndex]->RBody->getLinearVelocity().getY(), 
                    objs[dynamicCubeIndex]->RBody->getLinearVelocity().getZ());
    // handle key down events here
    switch(m_event.key.keysym.sym)
    {
      case SDLK_ESCAPE:
      m_running = false; //quit the program
      break;

      //toggle to vert shader
      case SDLK_v:
        std::cout << "v" << std::endl;
        m_graphics->toggleShader(0);
        break;

      //toggle to frag shader
      case SDLK_f:
        std::cout << "f" << std::endl;
        m_graphics->toggleShader(1);
        break;

      // toggle flat shading
      case SDLK_c:
	std::cout << "flat" << std::endl;
	m_graphics->toggleShader(2);
	break;

      case SDLK_u: // increase ambient lighting
        std::cout << "Ambience: " << m_graphics->ambience.x << " " <<  m_graphics->ambience.y << " " <<  m_graphics->ambience.z << std::endl;
        m_graphics->ambience.x += 0.05f;
        m_graphics->ambience.y += 0.05f;
        m_graphics->ambience.z += 0.05f;
        break;
      case SDLK_y: //decrease ambient lighting
        std::cout <<  "Ambience: " << m_graphics->ambience.x << " " <<  m_graphics->ambience.y << " " <<  m_graphics->ambience.z << std::endl;
        m_graphics->ambience.x -= 0.05f;
        m_graphics->ambience.y -= 0.05f;
        m_graphics->ambience.z -= 0.05f;
        break;
      case SDLK_h: //increase cubes diffuse lighting
        std::cout << "Cube Diffuse: " <<  m_graphics->diffuse.x << " " <<  m_graphics->diffuse.y << " " <<  m_graphics->diffuse.z << std::endl;
        m_graphics->diffuse.x += 0.05f;
        m_graphics->diffuse.y += 0.05f;
        m_graphics->diffuse.z += 0.05f;
        break;
      case SDLK_j: //decrease cubes diffuse lighting
        std::cout << "Cube Diffuse: " <<  m_graphics->diffuse.x << " " <<  m_graphics->diffuse.y << " " <<  m_graphics->diffuse.z << std::endl;
        m_graphics->diffuse.x -= 0.05f;
        m_graphics->diffuse.y -= 0.05f;
        m_graphics->diffuse.z -= 0.05f;
        break;
      case SDLK_n: //increase cubes specular lighting
        std::cout << "Cube Specular: " <<  m_graphics->specular.x << " " <<  m_graphics->specular.y << " " <<  m_graphics->specular.z << std::endl;
        m_graphics->specular.x += 0.05f;
        m_graphics->specular.y += 0.05f;
        m_graphics->specular.z += 0.05f;
        break;
      case SDLK_m: //decrease cubes specular lighting
        std::cout << "Cube Specular: " <<  m_graphics->specular.x << " " <<  m_graphics->specular.y << " " <<  m_graphics->specular.z << std::endl;
        m_graphics->specular.x -= 0.05f;
        m_graphics->specular.y -= 0.05f;
        m_graphics->specular.z -= 0.05f;
        break;
      //////Input to move cube
      case SDLK_UP:
        // Example of original code :
        // m_graphics->cube->RBody->setActivationState(DISABLE_DEACTIVATION);
        // m_graphics->cube->RBody->setLinearVelocity(btVector3(0, 0, 10));
        objs[dynamicCubeIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
        objs[dynamicCubeIndex]->RBody->setLinearVelocity(btVector3(vel.getX(), vel.getY(), 10));
        break;

      case SDLK_DOWN:
        objs[dynamicCubeIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
        objs[dynamicCubeIndex]->RBody->setLinearVelocity(btVector3(vel.getX(), vel.getY(), -10));
        break;

      case SDLK_LEFT:
        objs[dynamicCubeIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
        objs[dynamicCubeIndex]->RBody->setLinearVelocity(btVector3(10, vel.getY(), vel.getZ()));
        break;

      case SDLK_RIGHT:
        objs[dynamicCubeIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
        objs[dynamicCubeIndex]->RBody->setLinearVelocity(btVector3(-10, vel.getY(), vel.getZ()));
        break;

      case SDLK_SPACE:
        objs[dynamicCubeIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
        objs[dynamicCubeIndex]->RBody->applyCentralImpulse(btVector3(vel.getX(), 10, vel.getZ()));
        break;

      case SDLK_p:
        std::cout << "p" << std::endl;
        objs[rPaddleIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
        m_physics->movePaddle(getDT(), "left", objs[rPaddleIndex]->RBody);
        break;

      default:
        break;
    }
  }
    else if (m_event.type == SDL_KEYUP)
  {
    // handle key down events here
    switch(m_event.key.keysym.sym)
    {
      ////// When an arrow key is released, make sure cube stops moving in the given direction
      ////// However, the cube's velocity in other directions should stay the same
      case SDLK_UP:
	      objs[dynamicCubeIndex]->RBody->setLinearVelocity(btVector3(vel.getX(), vel.getY(), 0));
        break;

      case SDLK_DOWN:
	      objs[dynamicCubeIndex]->RBody->setLinearVelocity(btVector3(vel.getX(), vel.getY(), 0));
        break;

      case SDLK_LEFT:
	      objs[dynamicCubeIndex]->RBody->setLinearVelocity(btVector3(0, vel.getY(), vel.getZ()));
        break;

      case SDLK_RIGHT:
	      objs[dynamicCubeIndex]->RBody->setLinearVelocity(btVector3(0, vel.getY(), vel.getZ()));
        break;

      default:
        break;
    }
  }
}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}

void Engine::LoseBall()
{
  std::cout << "Lost Ball" << std::endl;
  //m_physics->lostBall = false;
}

/* 
   Outputs indexes and keynames of all objects in objs
 */
void Engine::outputObjects() const {
  std::cout << "== Objects in engine ==" << std::endl;
  for(int i = 0; i < objs.size(); i++) {
    std::cout << i << ": " << objs[i]->getKeyname() << std::endl;
  }
  std::cout << "=======================" << std::endl;
}

int Engine::getIndexOf(const std::string& key) {
  for(int i = 0; i < objs.size(); i++) {
    if (objs[i]->getKeyname() == key)
      return i;
  }
  return -1; // Key not found 
}
