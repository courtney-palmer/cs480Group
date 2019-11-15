
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
  score = 0;

  // ============= Create the objects ===============================
  // note : Objects in obj array have graphics and physics components
  //        automatically updated and rendered
  //        in Engine::Run()
  // Format is as follows: 
  // create ShapeInfo struct > use struct to initialize object
  // push object into objs array  > add object to physics world  

  /*
  // add moveable cube
  struct ShapeInfo info(box, 1, 1, 1); // 1,1,1 represents size of collision shape
  Object* temp = new Object("cubeTest.obj", info, "block", "wood.jpg"); // temp holds next object to be stored
  objs.push_back(temp);
  m_physics->AddShape(temp,   // pass in pointer to the object you just created
		      0,2,0, // 0,10,0 represents starting position of object
		      1);  // this value is static vs dynamic. true - dynamic
  // Used in Keyboard() to refer to array index of object that will be moved
  dynamicCubeIndex = objs.size()-1;
  */
  
  // Add invisible wall on top
  struct ShapeInfo invWallInfo(mesh);
  Object* temp = new Object("boardwwalls.obj", invWallInfo, "pinballCover", "wood.jpg");
  objs.push_back(temp);
  m_physics->AddShape(temp,
	 	      0,1,0,
	 	      3);
  objs.back()->physicsObject->setUserPointer(objs[objs.size()-1]);

  // Add BOARD : Static
  struct ShapeInfo wallInfo(mesh);
  temp = new Object("pboard.obj", wallInfo, "board", "harris.jpg");
  objs.push_back(temp);
  m_physics->AddShape(temp,
		      0, 0, 0,
		      3);
  objs.back()->physicsObject->setUserPointer(objs[objs.size()-1]);

   struct ShapeInfo backInfo(mesh);
  temp = new Object("backBoard.obj", backInfo, "backboard", "harris.jpg");
  objs.push_back(temp);
  m_physics->AddShape(temp,
		      0, 0, 0,
		      3);
  objs.back()->physicsObject->setUserPointer(objs[objs.size()-1]);

  // Add cones to side
  struct ShapeInfo c1(mesh);
  temp = new Object("lCone.obj", c1, "cone", "wood.jpg");
  objs.push_back(temp);
  m_physics->AddShape(temp,
		      7, -0.5, 5,
		      3);
  objs.back()->physicsObject->setUserPointer(objs[objs.size()-1]);

  temp = new Object("rCone.obj", c1, "cone", "wood.jpg");
  objs.push_back(temp);
  m_physics->AddShape(temp,
		      -5, -0.5, 5,
		      3);
  objs.back()->physicsObject->setUserPointer(objs[objs.size()-1]);

  // adding a series of bumpers
  struct ShapeInfo bumperInfo1(mesh);
  temp = new Object("bumper.obj", bumperInfo1, "bumper", "wood.jpg");
  objs.push_back(temp);
  m_physics->AddShape(temp,
		      -1, -1, 0,
		      3);
  objs.back()->physicsObject->setUserPointer(objs[objs.size()-1]);
  //std::cout << (Object*)objs.back()->physicsObject->getUserPointer() << std::endl;

  struct ShapeInfo bumperInfo2(mesh);
  temp = new Object("bumper.obj", bumperInfo2, "bumper", "wood.jpg");
  objs.push_back(temp);
  m_physics->AddShape(temp,
		      2, -1, 0,
		      3);
  objs.back()->physicsObject->setUserPointer(objs[objs.size()-1]);

  struct ShapeInfo bumperInfo3(mesh);
  temp = new Object("bumper.obj", bumperInfo3, "bumper", "wood.jpg");
  objs.push_back(temp);
  m_physics->AddShape(temp,
		      1, -1, 5,
		      3);
  objs.back()->physicsObject->setUserPointer(objs[objs.size()-1]);

  // Add plunger
  struct ShapeInfo plungerInfo(mesh);
  temp = new Object("plunger.obj", plungerInfo, "plunger");
  objs.push_back(temp);
  m_physics->AddShape(temp,
		      0,0,5,
		      3);
  plungerIndex = objs.size()-1;
  objs.back()->physicsObject->setUserPointer(objs[objs.size()-1]);

  // Add ball
  struct ShapeInfo ballInfo(sphere, 0.4, 0.4, 0.4);
  temp = new Object("pinball.obj", ballInfo, "pinball", "wood.jpg");
  objs.push_back(temp);
  m_physics->AddShape(temp,
		     -6,0,5,
		     1);
  ballIndex = objs.size()-1;
  objs.back()->physicsObject->setUserPointer(objs[objs.size()-1]);

  // Add paddles
  struct ShapeInfo rPaddleInfo(mesh);
  temp = new Object("rPaddle.obj", rPaddleInfo, "rPaddle");
  objs.push_back(temp);
  m_physics->AddShape(temp,
		      -2.45, 0, -4.6,
		      2);
  rPaddleIndex = objs.size()-1;
  objs.back()->physicsObject->setUserPointer(objs[objs.size()-1]);

    struct ShapeInfo lPaddleInfo(mesh);
  temp = new Object("lPaddle.obj", lPaddleInfo, "lPaddle");
  objs.push_back(temp);
    m_physics->AddShape(temp,
			4.7, 0, -4.6,
			2);
  lPaddleIndex = objs.size()-1;
  objs.back()->physicsObject->setUserPointer(objs[objs.size()-1]);

  buffer = 0;
  bufferMax = 1;
  followBall = false;

  // ========================= End Object Creation :> =================

  // No errors
  return true;
}

void Engine::Run()
{
  std::cout << "running" << std::endl;
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
    m_physics->Update(objs, score);
    std::cout << "physics updated" << std::endl;

    // run 10x less than m_physics->update
    if(buffer >= bufferMax) {
      m_physics->updatePaddle(objs[rPaddleIndex]->RBody, true);
      m_physics->updatePaddle(objs[lPaddleIndex]->RBody, false);
      buffer = 0;
    }
    else
      buffer++;
    //m_physics->Update(objs, ballIndex, trigIndex);

    // Update Graphics, send in physics instance and each single object.
    for(int i = 0; i < objs.size(); i++) {
      m_graphics->Update(m_physics, objs[i]);
    }

    // Render, send in objs vector array
    m_graphics->Render(objs);

     //Check to see if a ball has been lost
    if(m_physics->lostBall == true) {
      LoseBall();
    }

    // Update camera to follow ball if necessary
    if(followBall) {
      m_graphics->m_camera->Update(objs[ballIndex]->x, objs[ballIndex]->y + 10, objs[ballIndex]->z,
				   objs[ballIndex]->x, objs[ballIndex]->y, objs[ballIndex]->z,
				   0,0,1);
    }
     
    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard()
{
   btTransform trans;
  //float ballForce = 0.0f;
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

      // case SDLK_p:
      //   objs[rPaddleIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
      //   m_physics->movePaddle(getDT(), "right", objs[rPaddleIndex]->RBody, true);
      // break;

     case SDLK_o:
        objs[ballIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
        objs[plungerIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);

        if( m_physics->plungerForce < 20 && m_physics->ballLaunched == false)
        { 
          std::cout << m_physics->plungerPull << std::endl;
          m_physics->plungerPull -= .2;
          if(m_physics->plungerPull < 0){
            m_physics->plungerPull = 0;
          }

          objs[plungerIndex]->RBody->getMotionState()->getWorldTransform(trans);
          trans.setOrigin(btVector3(0.0f, 0.0f, m_physics->plungerPull));
          objs[plungerIndex]->RBody->getMotionState()->setWorldTransform(trans);
          objs[plungerIndex]->RBody->setMotionState(objs[plungerIndex]->RBody->getMotionState());
          //plunger->model = glm::make_mat4(m);
          m_physics->plungerForce += 1;
          std::cout << m_physics->plungerPull << std::endl;
        }


        //m_physics->applyPlungerForce(objs[plungerIndex]->RBody, m_physics->plungerPull, m_physics->plungerForce);
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
        //pinball m_graphics->cube->RBody->setActivationState(DISABLE_DEACTIVATION);
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

    case SDLK_p: // Right paddle
        std::cout << "p" << std::endl;
        objs[rPaddleIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
        m_physics->movePaddle(getDT(), "right", objs[rPaddleIndex]->RBody);
        break;

    case SDLK_i: // Left Paddle
      std::cout << "i" << std::endl;
        objs[lPaddleIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
        m_physics->movePaddle(getDT(), "left", objs[lPaddleIndex]->RBody);
        break;

    case SDLK_r: // reset game
      m_physics->moveObject(objs, m_physics->getBallIndex(),
			    -6,0,5);
      ballsRemaining = MAX_BALLS;
      score = 0;
      break;

    case SDLK_t: // Set camera to top down view
      m_graphics->m_camera->Update(0,30,-10,
				   0,0,0,
				   0,0,1);
      break;
    case SDLK_z: // Set camera to normal view
      m_graphics->m_camera->Update(0, 15, -20,
				   0,0,0,
				   0,0,1);
      break;
    case SDLK_x: // Follow ball
      toggleFollowBall();
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
      
      // case SDLK_p:
      // std::cout << "p released" << std::endl;
      // objs[rPaddleIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
      //  m_physics->movePaddle(getDT(), "right", objs[rPaddleIndex]->RBody, false);
      // break;

      case SDLK_o:
        objs[ballIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
        objs[plungerIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
        
      if(m_physics->ballLaunched == false){
        btTransform trans;
        std::cout << "b" << std::endl;

        //apply physics to ball
        objs[ballIndex]->RBody->applyCentralImpulse(btVector3(0.0,0.0, m_physics->plungerForce));
        m_physics->plungerForce = 0;

        objs[ballIndex]->RBody->getMotionState()->getWorldTransform(trans);
        trans.setOrigin(btVector3(-6.0f, 0.0f, m_physics->plungerPull));
        objs[ballIndex]->RBody->getMotionState()->setWorldTransform(trans);
        objs[ballIndex]->RBody->setMotionState(objs[ballIndex]->RBody->getMotionState());

        //apply physics to plunger
        m_physics->plungerPull = 5;
        objs[plungerIndex]->RBody->getMotionState()->getWorldTransform(trans);
        objs[plungerIndex]->RBody->applyCentralImpulse(btVector3(0.0f, 0.0f, m_physics->plungerForce));
        m_physics->plungerForce = 0.0f;
        objs[plungerIndex]->RBody->getMotionState()->setWorldTransform(trans);
        trans.setOrigin(btVector3(0.0f, 0.0f, m_physics->plungerPull));
        objs[plungerIndex]->RBody->getMotionState()->setWorldTransform(trans);
        objs[plungerIndex]->RBody->setMotionState(objs[plungerIndex]->RBody->getMotionState());
        std::cout << "location at: " << "0,0, " << m_physics->plungerPull << std::endl;
      }
        
        
        break;

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
  std::cout << "-1 ball\n";
  ballsRemaining--;
  m_physics->lostBall = false;
  //move ball to starting position
  // if(ballsRemaining <= 0)
  // {
  //   //game over
  // }
  // else
    m_physics->moveObject(objs, m_physics->getBallIndex(),
			                    -6,0,5);
}

/* 
   Outputs indexes and keynames of all objects in objs
 */
void Engine::outputObjects() const {
  std::cout << "== Objects in engine ==" << std::endl;
  for(int i = 0; i < objs.size(); i++) {
    std::cout << i << ": " << objs[i]->getKeyname()
	      << " x" << objs[i]->x
	      << " y" << objs[i]->y
	      << " z" << objs[i]->z
	      << std::endl;
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

bool Engine::toggleFollowBall() {
  if(followBall)
    followBall = false;
  else
    followBall = true;
  return followBall;    
}
