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
	bounds = 31.0f;

	poolBallInfo = std::map<MyEntity*, PhysicsInfo*>();


		//Adds the balls to the scene
#pragma region balls

	for (int i = 1; i < 9; i++)
	{
		Simplex::String name = "Models\\";
		name += std::to_string(i);
		name += "Ball.obj";
			if (i != 8)
				m_pEntityMngr->AddEntity(name, name);//fuck it
		else if (i == 8)
			m_pEntityMngr->AddEntity(name, "8ball");
		//vector3 v3Position = vector3(glm::sphericalRand(25.0f));
		float v1 = glm::linearRand(-28.0f, 28.0f);
		float v2 = glm::linearRand(-28.0f, 28.0f);
		float v3 = glm::linearRand(-28.0f, 28.0f);
		vector3 v3Position = vector3(v1, v2, v3);
		matrix4 m4Position = glm::translate(v3Position);
		m_pEntityMngr->SetModelMatrix(m4Position);
		PhysicsInfo* info = new PhysicsInfo(1.0f, v3Position, vector3(0.0f), vector3(bounds));
		MyEntity* ball = m_pEntityMngr->GetEntity(-1);
		poolBallInfo.insert(std::pair<MyEntity*, PhysicsInfo*>(ball, info));
		if (i == 8) //if it is the eight ball, set the rigidbody to use for collision logic
		{
			eightBallRB = m_pEntityMngr->GetRigidBody("8ball");
		}
		//m_pEntityMngr->AddDimension(-1, uIndex);
		//uIndex++;
	}

#pragma endregion balls

	//Adds Pockets to the scene
#pragma region pockets

	//front left bottom
	m_pEntityMngr->AddEntity("Models\\pocket.obj", "pocket1");
	v3Position = vector3(-30.0, -30.0, 30.0);
	m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);
	pocketRBList[0] = m_pEntityMngr->GetRigidBody("pocket1");

	//front right bottom
	m_pEntityMngr->AddEntity("Models\\pocket.obj", "pocket2");
	v3Position = vector3(30.0, -30.0, 30.0);
	m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);
	pocketRBList[1] = m_pEntityMngr->GetRigidBody("pocket2");

	//front right top
	m_pEntityMngr->AddEntity("Models\\pocket.obj", "pocket3");
	v3Position = vector3(30.0, 30.0, 30.0);
	m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);
	pocketRBList[2] = m_pEntityMngr->GetRigidBody("pocket3");

	 //front left top
	m_pEntityMngr->AddEntity("Models\\pocket.obj", "pocket4");
	v3Position = vector3(-30.0, 30.0, 30.0);
	m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);
	pocketRBList[3] = m_pEntityMngr->GetRigidBody("pocket4");

	//back left bottom
	m_pEntityMngr->AddEntity("Models\\pocket.obj", "pocket5");
	v3Position = vector3(-30.0, -30.0, -30.0);
	m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);
	pocketRBList[4] = m_pEntityMngr->GetRigidBody("pocket5");

	//back right bottom
	m_pEntityMngr->AddEntity("Models\\pocket.obj", "pocket6");
	v3Position = vector3(30.0, -30.0, -30.0);
	m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);
	pocketRBList[5] = m_pEntityMngr->GetRigidBody("pocket6");

	//back right top
	m_pEntityMngr->AddEntity("Models\\pocket.obj", "pocket7");
	v3Position = vector3(30.0, 30.0, -30.0);
	m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);
	pocketRBList[6] = m_pEntityMngr->GetRigidBody("pocket7");

	//back left top
	m_pEntityMngr->AddEntity("Models\\pocket.obj", "pocket8");
	v3Position = vector3(-30.0, 30.0, -30.0);
	m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position);
	pocketRBList[7] = m_pEntityMngr->GetRigidBody("pocket8");

#pragma endregion pockets

	//Adds Cue ball to the scene
	m_pEntityMngr->AddEntity("Models\\cueBall.obj", "CueBall");
	v3Position = vector3(0.0f, 0.0f, 0.0f); //variable defined in above pragam region
	m4Position = glm::translate(v3Position); //variable defined in above pragam region
	m_pEntityMngr->SetModelMatrix(m4Position);

	cueBall = m_pEntityMngr->GetEntity(-1);
	cueBallRB = cueBall->GetRigidBody();
	PhysicsInfo* info = new PhysicsInfo(1.0f, v3Position, vector3(0.0f), vector3(bounds));
	poolBallInfo.insert(std::pair<MyEntity*, PhysicsInfo*>(cueBall, info));


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

	std::map<MyEntity*, PhysicsInfo*>::iterator it;

	// update the velocity and move all the balls
	for (it = poolBallInfo.begin(); it != poolBallInfo.end(); it++)
	{
		(it->second)->UpdateVelocity();
		vector3 pos = (it->second)->position;
		if (!isnan(pos.x) && !isnan(pos.y) && !isnan(pos.z))
		{
			(it->first)->SetModelMatrix(/*(it->first)->GetModelMatrix() * */glm::translate(pos));
		}
	}


	//std::map<MyEntity*, PhysicsInfo*>::iterator outer;
	//collide with all the things
	for (auto& x : poolBallInfo) 
	{
		//std::map<MyEntity*,PhysicsInfo>::iterator iter = std::find(x.first->GetRigidBody()->m_CollidingRBSet.begin(), x.first->GetRigidBody()->m_CollidingRBSet.end(), )
		for (const auto& y : x.first->GetRigidBody()->m_CollidingRBSet) 
		{
			float fDist = glm::distance(x.first->GetRigidBody()->GetCenterGlobal(), y->GetCenterGlobal());
			if ( fDist < x.first->GetRigidBody()->GetRadius() + y->GetRadius())//if the entities are sphere colliding, resolve
			{
				//offset
				vector3 v3Dir = PhysicsInfo::normalize(x.first->GetRigidBody()->GetCenterGlobal() - y->GetCenterGlobal());
				v3Dir = v3Dir * (2.05 * x.first->GetRigidBody()->GetRadius());

				PhysicsInfo* collideInfo = Find(y);

				collideInfo->position = x.second->position + v3Dir;

				if (x.second->GetMagnitude() > 0.0f && collideInfo->mass != 999.0f)
				{
					poolBallInfo[x.first]->Collision(collideInfo);
					//x.first->GetRigidBody()->RemoveCollisionWith(y);
					//y->RemoveCollisionWith(x.first->GetRigidBody());
				}
			}
		}
	}

	//ball vs pocket collision
	for (int i = 0; i < 8; i++) //pockets loop
	{
		for (const auto& other : pocketRBList[i]->m_CollidingRBSet)
		{
			for (int j = 0; j < m_pEntityMngr->GetEntityCount(); j++)
			{
				if (other == cueBallRB) //if cueball
				{
					gameScore--;
					vector3 tempVec = vector3(0.0f, 0.0f, 0.0f);
					matrix4 m4Position = glm::translate(tempVec);
					String id = cueBall->GetUniqueID();
					m_pEntityMngr->SetModelMatrix(m4Position, id);
					poolBallInfo.erase(cueBall);
					PhysicsInfo* info = new PhysicsInfo(1.0f, tempVec, vector3(0.0f), vector3(bounds));
					poolBallInfo.insert(std::pair<MyEntity*, PhysicsInfo*>(cueBall, info));
				}

				else if (other == eightBallRB) // if eight ball
				{
					ballCount = static_cast<int>(m_pEntityMngr->GetEntityCount()) - 9;
					if (ballCount == 1) //only 8 ball is left
					{
						//game won
						EightBallSunk = 1;
					}
					else
					{
						//game lost
						EightBallSunk = 2;
					}
					//update remove index to be the 8 ball
					removeIndex = m_pEntityMngr->GetEntityIndex("8ball"); 
				}

				else if (other == m_pEntityMngr->GetEntity(j)->GetRigidBody()) //if normal ball
				{
					gameScore++;
					removeIndex = j;
				}
			}
		}
		
		/*for (int j = 0; j < m_pEntityMngr->GetEntityCount(); j++) //loop entities UNOPTIMIZED
		{
			//ignore pockets, 8-ball, and cueball
			if (j < 7 || j > 16)
			{
				if (m_pEntityMngr->GetRigidBody(i)->IsColliding(m_pEntityMngr->GetRigidBody(j)))
				{
					gameScore++;
					RemoveBall(m_pEntityMngr->GetEntity(j));
				}
			}
			//if 8 ball hits pocket
			else if (j == 7)
			{
				if (m_pEntityMngr->GetRigidBody(i)->IsColliding(m_pEntityMngr->GetRigidBody(j)))
				{
					ballCount = static_cast<int>(m_pEntityMngr->GetEntityCount()) - 9;
					if (ballCount == 1) //only 8 ball is left
					{
						//game won
						EightBallSunk = 1;
					}
					else
					{
						//game lost
						EightBallSunk = 2;
					}
					RemoveBall(m_pEntityMngr->GetEntity(j));
				}
			}
		}*/
	}
	if (removeIndex != -1) //removes ball if index was set above
	{
		RemoveBall(m_pEntityMngr->GetEntity(removeIndex));
		removeIndex = -1;
	}
		

	//Cueball vs pocket collision
	/*for (int i = 8; i < 16; i++)
	{
		if (m_pEntityMngr->GetEntityCount() > 0)
		{
			if (cueBallRB->IsColliding(m_pEntityMngr->GetRigidBody(i)))
			{
				gameScore--;
				vector3 tempVec = vector3(0.0f, 0.0f, 0.0f);
				matrix4 m4Position = glm::translate(tempVec);
				String id = cueBall->GetUniqueID();
				m_pEntityMngr->SetModelMatrix(m4Position, id);
				poolBallInfo.erase(cueBall);
				PhysicsInfo* info = new PhysicsInfo(1.0f, tempVec, vector3(0.0f), vector3(bounds));
				poolBallInfo.insert(std::pair<MyEntity*, PhysicsInfo*>(cueBall, info));
			}
		}
		
	}*/

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

	for (auto&x : poolBallInfo) {

		SafeDelete(x.second);
	}
	//release GUI
	ShutdownGUI();
}

PhysicsInfo* Application::Find(MyRigidBody* rigid)
{
	std::map<MyEntity*, PhysicsInfo*>::iterator loopThrough;
	//loopThrough = std::find(poolBallInfo.begin(), poolBallInfo.end(), loopThrough->first->GetRigidBody());
	for (loopThrough = poolBallInfo.begin(); loopThrough != poolBallInfo.end(); loopThrough++)
	{
		if (rigid->IsEqual(*loopThrough->first->GetRigidBody()))
		{
			return loopThrough->second;
		}
	}
	return new PhysicsInfo(999);
}

void Simplex::Application::RemoveBall(MyEntity* ball)
{
	// remove ball's physics info from map
	poolBallInfo.erase(ball);

	// remove ball from octree
	m_pRoot->RemoveEntity(ball);

	// remove ball from entity manager (also deletes ball pointer)
	m_pEntityMngr->RemoveEntity(ball->GetUniqueID());
}

void Simplex::Application::AddBalls(void)
{
	for (int i = 1; i < 8; i++)
	{
		Simplex::String name = "Models\\";
		name += std::to_string(i);
		name += "Ball.obj";
		m_pEntityMngr->AddEntity(name);
		//vector3 v3Position = vector3(glm::sphericalRand(25.0f));
		float v1 = glm::linearRand(-28.0f, 28.0f);
		float v2 = glm::linearRand(-28.0f, 28.0f);
		float v3 = glm::linearRand(-28.0f, 28.0f);
		vector3 v3Position = vector3(v1, v2, v3);
		matrix4 m4Position = glm::translate(v3Position);
		m_pEntityMngr->SetModelMatrix(m4Position);
		PhysicsInfo* info = new PhysicsInfo(1.0f, v3Position, vector3(0.0f), vector3(bounds));
		MyEntity* ball = m_pEntityMngr->GetEntity(-1);
		poolBallInfo.insert(std::pair<MyEntity*, PhysicsInfo*>(ball, info));
		m_pRoot->AddEntity(ball);

		m_pEntityMngr->Update();
	}
}
