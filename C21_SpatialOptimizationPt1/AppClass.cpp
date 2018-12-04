#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		initialCameraPos, //Position
		initialCameraTar,	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)
	
	//Entity Manager
	m_pEntityMngr = MyEntityManager::GetInstance();

	int nSquare = 500;
	vector3 v3Position;
	matrix4 m4Position;
	uint uIndex = 0;

	poolBallInfo = std::map<MyEntity*, PhysicsInfo>();

	/*for (int i = 0; i < nSquare; i++)
	{
		
		m_pEntityMngr->AddEntity("Minecraft\\Cube.obj");
		vector3 v3Position = vector3(glm::sphericalRand(34.0f));
		matrix4 m4Position = glm::translate(v3Position);
		m_pEntityMngr->SetModelMatrix(m4Position);
		
		uIndex++;
	
	}*/

		//Adds the balls to the scene
#pragma region balls

	for (int i = 1; i < 9; i++)
	{
		Simplex::String name = "Models\\";
		name += std::to_string(i);
		name += "Ball.obj";
		m_pEntityMngr->AddEntity(name);
		vector3 v3Position = vector3(glm::sphericalRand(25.0f));
		matrix4 m4Position = glm::translate(v3Position);
		m_pEntityMngr->SetModelMatrix(m4Position);
		PhysicsInfo info = PhysicsInfo(1.0f, v3Position, vector3(0.0f), vector3(36.0f));
		MyEntity* ball = m_pEntityMngr->GetEntity(-1);
		poolBallInfo.insert(std::pair<MyEntity*, PhysicsInfo>(ball, info));
		//m_pEntityMngr->AddDimension(-1, uIndex);
		//uIndex++;
	}

#pragma endregion balls

	//Adds Pockets to the scene
#pragma region pockets

	//front left bottom
	m_pEntityMngr->AddEntity("Models\\pocket.obj");
	v3Position = vector3(-35.0f, -35.0f, 35.0f);
	m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);

	//front right bottom
	m_pEntityMngr->AddEntity("Models\\pocket.obj");
	v3Position = vector3(35.0f, -35.0f, 35.0f);
	m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);

	//front right top
	m_pEntityMngr->AddEntity("Models\\pocket.obj");
	v3Position = vector3(35.0f, 35.0f, 35.0f);
	m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);

	 //front left top
	m_pEntityMngr->AddEntity("Models\\pocket.obj");
	v3Position = vector3(-35.0f, 35.0f, 35.0f);
	m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);

	//back left bottom
	m_pEntityMngr->AddEntity("Models\\pocket.obj");
	v3Position = vector3(-35.0f, -35.0f, -35.0f);
	m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);

	//back right bottom
	m_pEntityMngr->AddEntity("Models\\pocket.obj");
	v3Position = vector3(35.0f, -35.0f, -35.0f);
	m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);

	//back right top
	m_pEntityMngr->AddEntity("Models\\pocket.obj");
	v3Position = vector3(35.0f, 35.0f, -35.0f);
	m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);

	//back left top
	m_pEntityMngr->AddEntity("Models\\pocket.obj");
	v3Position = vector3(-35.0f, 35.0f, -35.0f);
	m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);

#pragma endregion pockets

	//Adds Cue ball to the scene
	m_pEntityMngr->AddEntity("Models\\cueBall.obj");
	v3Position = vector3(0.0f, 0.0f, 0.0f); //variable defined in above pragam region
	m4Position = glm::translate(v3Position); //variable defined in above pragam region
	m_pEntityMngr->SetModelMatrix(m4Position);

	cueBall = m_pEntityMngr->GetEntity(-1);
	cueBallRB = cueBall->GetRigidBody();
	PhysicsInfo info = PhysicsInfo(1.0f, v3Position, vector3(0.0f), vector3(36.0f));
	poolBallInfo.insert(std::pair<MyEntity*, PhysicsInfo>(cueBall, info));


	m_pEntityMngr->Update();
	m_pRoot = new MyOctant(2);

	// Initialize the position of the pointer to the middle of the screen
	centerX = m_pSystem->GetWindowX() + m_pSystem->GetWindowWidth() / 2;
	centerY = m_pSystem->GetWindowY() + m_pSystem->GetWindowHeight() / 2;

	
	//m_pRoot = new MyOctant(vector3(10.0f, 10.0f, 10.0f), 50.0f);
	//steve
	//m_pEntityMngr->AddEntity("Minecraft\\Steve.obj", "Steve");
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//update the octree
	m_pRoot->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	// get the force with which to hit the cue ball
	GetCueForce();

	if (cameraLerping) LerpCameraToCenter();

	std::map<MyEntity*, PhysicsInfo>::iterator it;

	// update the velocity and move all the balls
	for (it = poolBallInfo.begin(); it != poolBallInfo.end(); it++)
	{
		(it->second).UpdateVelocity();
		vector3 vel = (it->second).GetVelocity();
		(it->first)->SetModelMatrix((it->first)->GetModelMatrix() * glm::translate(vel));
	}
	std::map<MyEntity*, PhysicsInfo>::iterator outer;
	for (outer = poolBallInfo.begin(); outer != poolBallInfo.end(); outer++)
	{
		MyRigidBody* rig = outer->first->GetRigidBody();
		for (uint i = 0; i < rig->m_nCollidingSetSize; ++i)
		{
			// resolve collisions
		}
	}
	/*
	//crappy temp collision detection
	std::map<MyEntity*, PhysicsInfo>::iterator outer;
	std::map<MyEntity*, PhysicsInfo>::iterator inner;
	for (outer = poolBallInfo.begin(); outer != poolBallInfo.end(); outer++)
	{
		inner = outer;
		inner++;
		if (inner != poolBallInfo.end())
		{
			for (inner = inner; inner != poolBallInfo.end(); inner++)
			{
				if (outer->first != inner->first && outer->first->IsColliding(inner->first))
				{
					outer->second.Collision(inner->second);
				}
			}
		}
	}
	*/

	/*vector3 vel = poolBallInfo[cueBall].GetVelocity();
	poolBallInfo[cueBall].UpdateVelocity();
	std::cout << "Cue Ball Velocity: " << "( "<< vel.x  << ", " << vel.y << ", " << vel.z << ")" << std::endl;
	cueBall->SetModelMatrix(cueBall->GetModelMatrix() * glm::translate(vel));*/

	//Update Entity Manager
	m_pEntityMngr->Update();

	/*m_pMeshMngr->AddGridToRenderList(glm::rotate(IDENTITY_M4, 1.5708f, AXIS_Y));
	m_pMeshMngr->AddGridToRenderList(glm::translate(vector3(-17.0f, 0.0f, 0.0f)) * glm::rotate(IDENTITY_M4, 1.5708f, AXIS_Y));
	m_pMeshMngr->AddGridToRenderList(glm::translate(vector3(17.0f, 0.0f, 0.0f)) * glm::rotate(IDENTITY_M4, 1.5708f, AXIS_Y));*/
	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
	m_pRoot->Display();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	//debug object rendering to try and figure out what the hell my problem is
	/*for (size_t i = 0; i < m_pEntityMngr->GetEntityList().size(); i++)
	{
		m_pMeshMngr->AddMeshToRenderList(
			m_pMeshMngr->GetMesh(
				m_pMeshMngr->GenerateIcoSphere(1, 3)
			),
			m_pEntityMngr->GetEntity(i)->GetModelMatrix()
		);

	}*///this is a memory filler, only use to visualize possibly invisible objects
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{

	SafeDelete(m_pRoot);
	//release GUI
	ShutdownGUI();
}

MyRigidBody* Application::Find(MyRigidBody rigid)
{
	std::map<MyEntity*, PhysicsInfo>::iterator loopThrough;
	for (loopThrough = poolBallInfo.begin(); loopThrough != poolBallInfo.end(); loopThrough++)
	{
		if (rigid.IsEqual(*loopThrough->first->GetRigidBody()))
		{
			return loopThrough->first->GetRigidBody();
		}
	}
	return nullptr;
}

void Simplex::Application::RemoveBall(MyEntity* ball)
{
	
	// remove ball's physics info from map
	poolBallInfo.erase(ball);

	// remove ball from octree
	m_pRoot->RemoveEntity(ball);

	// remove ball from entity manager
	m_pEntityMngr->RemoveEntity(ball->GetUniqueID());

	// delete pointer
	//SafeDelete(ball);
}