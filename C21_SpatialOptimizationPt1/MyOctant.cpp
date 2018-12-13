#include "MyOctant.h"
using namespace Simplex;
uint MyOctant::m_nCount = 0;

static std::map<int, MyOctant*> OctLookUpTable;

static const int MAXDEPTH = 1;
static int dimCounter = -1;

//  MyOctant
void MyOctant::Init(void)
{
	m_nData = 0;
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
	m_ContainedEnts = std::vector<MyEntity*>();
	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i] = nullptr;
	}
}

MyOctant::MyOctant(int a_iLim)
{
	Init();
	OctLookUpTable = std::map<int, MyOctant*>();
	m_iLim = a_iLim;
	m_iDim = 0;
	OctLookUpTable.insert(std::pair<int, MyOctant*>(m_iDim, this));
	std::vector<MyEntity*> l_Entity_List = m_pEntityMngr->GetEntityList();
	uint iEntityCount = l_Entity_List.size();
	std::vector<vector3> v3MaxMin_list;
	for (uint i = 0; i < iEntityCount; ++i)
	{
		

		MyRigidBody* pRG = l_Entity_List[i]->GetRigidBody();
		vector3 v3Min = pRG->GetMinGlobal();
		vector3 v3Max = pRG->GetMaxGlobal();
		v3MaxMin_list.push_back(v3Min);
		v3MaxMin_list.push_back(v3Max);

		m_ContainedEnts.push_back(l_Entity_List[i]);//hey. cool. nice.
	}

	m_pRigidBody = new MyRigidBody(v3MaxMin_list);
	m_pRigidBody->MakeCubic();
	m_pRigidBody->m_fRadius = -10000.0f;
	Subdivide();
	dimCounter++;
}

MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	Init();
	std::vector<vector3> v3MaxMin_list;
	v3MaxMin_list.push_back(a_v3Center - vector3(a_fSize));
	v3MaxMin_list.push_back(a_v3Center + vector3(a_fSize));
	m_pRigidBody = new MyRigidBody(v3MaxMin_list);
	m_pRigidBody->m_fRadius = -100000.0f;
	m_nCount++;
	dimCounter++;
}

void MyOctant::Subdivide()
{	
	//safety
	if (m_nLevel > MAXDEPTH) return;

	if (m_pChild[0]) //if children are already present, destroy them
	{
		for (size_t i = 0; i < 8; i++)
		{
			m_pChild[i]->Release();//this is a safety more than anything, intended behaviour will not allow for subdivision of any dim with children
		}
	}

	//make new children
	vector3 v3Center = m_pRigidBody->GetCenterLocal();
	vector3 v3HalfWidth = m_pRigidBody->GetHalfWidth();
	float fSize = (v3HalfWidth.x) / 2.0f;
	float fCenters = fSize;

	m_pChild[0] = new MyOctant(v3Center + vector3(fCenters, fCenters, fCenters), fSize);
	m_pChild[1] = new MyOctant(v3Center + vector3(-fCenters, fCenters, fCenters), fSize);
	m_pChild[2] = new MyOctant(v3Center + vector3(-fCenters, -fCenters, fCenters), fSize);
	m_pChild[3] = new MyOctant(v3Center + vector3(fCenters, -fCenters, fCenters), fSize);

	m_pChild[4] = new MyOctant(v3Center + vector3(fCenters, fCenters, -fCenters), fSize);
	m_pChild[5] = new MyOctant(v3Center + vector3(-fCenters, fCenters, -fCenters), fSize);
	m_pChild[6] = new MyOctant(v3Center + vector3(-fCenters, -fCenters, -fCenters), fSize);
	m_pChild[7] = new MyOctant(v3Center + vector3(fCenters, -fCenters, -fCenters), fSize);

	//automation variables i'm allocating here for efficiency
	uint iSubCollisions;
	std::vector<MyEntity*> vSubBodies;

	//main construction loop
	for (uint i = 0; i < 8; i++)
	{
		//certain inits that have to be done here
		m_pChild[i]->m_nLevel = m_nLevel + 1;
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_iLim = m_iLim;
		m_pChild[i]->m_iDim = dimCounter + i;
		OctLookUpTable.insert_or_assign(m_pChild[i]->m_iDim, m_pChild[i]);
		

		//reset automation
		iSubCollisions = 0;
		vSubBodies = std::vector<MyEntity*>();
		for (size_t j = 0; j < m_ContainedEnts.size(); j++)
		{
			//m_ContainedEnts[j]->SetColorNotColliding(C_BLUE);
			//if an entity from the parent is colliding with a child, add it to the child's contained entities
			if (m_pChild[i]->m_pRigidBody->IsColliding(m_ContainedEnts[j]->GetRigidBody()))
			{
				//std::cout << "found a collision on the octree.";
				iSubCollisions++;
				vSubBodies.push_back(m_ContainedEnts[j]);
				//m_ContainedEnts[j]->SetColorNotColliding(C_BLUE);//testing to see what the octree sees
			}
		}
		//we now know what's in this child's bounds, so let's set that
		m_pChild[i]->m_ContainedEnts = vSubBodies;

		//recursive "call core." this is where the octree call stack is recoursed
		//if we're over the limit, attempt to subdivide, otherwise...
		if (iSubCollisions > m_iLim) 
		{
			if (m_nLevel > MAXDEPTH)
			{
				//if we're in too deep, set the contained entities' dimensions and back out
				for (size_t j = 0; j < m_ContainedEnts.size(); j++)
				{
					m_pEntityMngr->AddDimension(m_pChild[i]->m_ContainedEnts[j]->GetUniqueID(), m_pChild[i]->m_iDim);
				}
				return;
			}
			else
				m_pChild[i]->Subdivide();
		}
		else if (m_pChild[i]->m_ContainedEnts.size() > 0)
		{
			//...not over the limit, so add dimensions and return
			for (size_t j = 0; j < m_pChild[i]->m_ContainedEnts.size(); j++)
			{
				m_pEntityMngr->AddDimension(m_pChild[i]->m_ContainedEnts[j]->GetUniqueID(), m_pChild[i]->m_iDim);
			}
		}
		else //if completely empty, set a dummy
		{
			m_pEntityMngr->AddDimension("CHUNGUS", m_pChild[i]->m_iDim);//if the dim is empty, it still needs to be a part of the map, so add a dummy ent
		}
	}
}

void Simplex::MyOctant::Update() //this must ONLY be called on the root octant
{
	
	//iterate through the entity list to check if they're still colliding with their dimensions
	std::vector<MyEntity*> dirtyEntities = std::vector<MyEntity*>();
	//iterating through the dim dictionary (map)
	for (auto const&x : m_pEntityMngr->m_DimMap)
	{
		for (size_t i = 0; i < x.second.size(); i++)
		{
			if (!x.second[i]->GetRigidBody()->IsColliding(OctLookUpTable[x.first]->m_pRigidBody)) //if the entity is not colliding with the octant it's supposedly a part of
			{
				if (std::find(dirtyEntities.begin(), dirtyEntities.end(), x.second[i]) == dirtyEntities.end());//only mark dirty once
					dirtyEntities.push_back(x.second[i]);

				//
				m_pEntityMngr->RemoveDimension(x.second[i]->GetUniqueID(), x.first);
			}
		}
	}

	// clean dirty entities by adding them to the dimensions they're actually a part of
	std::map<int, std::vector<MyEntity*>> dirtyDims = std::map<int, std::vector<MyEntity*>>();
	for (size_t i = 0; i < dirtyEntities.size(); i++)
	{
		for (const auto&x : m_pEntityMngr->m_DimMap)
		{
			if (!dirtyEntities[i]->IsInDimension(x.first)) //if the dirty entity is not in this dimension, check it
			{
				if (dirtyEntities[i]->GetRigidBody()->IsColliding(OctLookUpTable[x.first]->m_pRigidBody))
				{
					//add the entity to the dimension
					m_pEntityMngr->AddDimension(dirtyEntities[i]->GetUniqueID(), x.first);
					OctLookUpTable[x.first]->m_ContainedEnts.push_back(dirtyEntities[i]); //the amount of quote-enquote cache invalidation that could happen with this many lookup tables and unsynchronized data might super blow, who knows

					dirtyDims.insert(x);//check for subdivision
				}
			}
		}
		if ((dirtyEntities[i]->m_DimensionList.size() == 0))//dirty entities MUST be added to some dimension at the end of this
		{
			std::cout << "Entity "<< dirtyEntities[i]->GetUniqueID() <<" has no dimensions!\n";
		}
		//clean the dimensions that need subdivision
		for (const auto&x : dirtyDims)
		{
			if (m_pEntityMngr->m_DimMap[x.first].size() > OctLookUpTable[x.first]->m_iLim && OctLookUpTable[x.first]->m_nLevel < MAXDEPTH) //if the dimension is too full, subdivide
			{
				//destroy the dimension
				for (size_t j = 0; j < m_pEntityMngr->m_DimMap[x.first].size(); j++)
				{
					m_pEntityMngr->RemoveDimension(m_pEntityMngr->m_DimMap[x.first][j]->GetUniqueID(), x.first);//i think i might be in indexer hell
				}
				//std::vector<int, std::vector<MyEntity*>>::iterator it = 

				//and make the worlds anew

				m_pEntityMngr->m_DimMap.erase(x.first);
				OctLookUpTable[x.first]->Subdivide();
			}
		}
	}


}

void Simplex::MyOctant::AddEntity(MyEntity * toAdd)
{
std::map<int, std::vector<MyEntity*>> dirtyDims = std::map<int, std::vector<MyEntity*>>();

	for (const auto&x : m_pEntityMngr->m_DimMap)
	{
		if (toAdd->GetRigidBody()->IsColliding(OctLookUpTable[x.first]->m_pRigidBody))
		{
			//add the entity to the dimension
			m_pEntityMngr->AddDimension(toAdd->GetUniqueID(), x.first);
			OctLookUpTable[x.first]->m_ContainedEnts.push_back(toAdd); //the amount of quote-enquote cache invalidation that could happen with this many lookup tables and unsynchronized data might super blow, who knows

			dirtyDims.insert(x);
		}
		
	}
	//clean the dimensions that need subdivision
	for (const auto&x : dirtyDims)
	{
		if (m_pEntityMngr->m_DimMap[x.first].size() > OctLookUpTable[x.first]->m_iLim && OctLookUpTable[x.first]->m_nLevel < MAXDEPTH) //if the dimension is too full, subdivide
		{
			//destroy the dimension
			for (size_t j = 0; j < m_pEntityMngr->m_DimMap[x.first].size(); j++)
			{
				m_pEntityMngr->RemoveDimension(m_pEntityMngr->m_DimMap[x.first][j]->GetUniqueID(), x.first);//i think i might be in indexer hell
			}
			//std::vector<int, std::vector<MyEntity*>>::iterator it = 

			//and make the worlds anew

			m_pEntityMngr->m_DimMap.erase(x.first);
			OctLookUpTable[x.first]->Subdivide();
		}
	}
	
}

void Simplex::MyOctant::RemoveEntity(MyEntity * toRemove)
{
	String i = toRemove->GetUniqueID();
	for (const auto&x : toRemove->m_DimensionList) 
	{
		//cleaning the entity from all the various registries it exists within
		m_pEntityMngr->RemoveDimension(i, x);
		std::vector<MyEntity*>::iterator it = std::find(OctLookUpTable[x]->m_ContainedEnts.begin(), OctLookUpTable[x]->m_ContainedEnts.end(), toRemove);
		//assert(it != OctLookUpTable[x]->m_ContainedEnts.end());//safety
		OctLookUpTable[x]->m_ContainedEnts.erase(it);

	}
}

void MyOctant::Swap(MyOctant& other)
{
	std::swap(m_nData, other.m_nData);
	std::swap(m_lData, other.m_lData);
}
void MyOctant::Release(void)
{
	//m_lData.clear(); 
		
	if (m_pChild[0])//if children exist, delete those too
	{
		for (size_t i = 0; i < 8; i++)
		{
			//assert(m_pEntityMngr->m_DimMap.count(m_pChild[i]->m_iDim) != 0);
			OctLookUpTable.erase(m_pChild[i]->m_iDim);
			delete m_pChild[i];
		}
	}
}
void Simplex::MyOctant::Display(void)
{
	//m_pRigidBody->AddToRenderList();
	for (uint i = 0; i < 8; i++)
	{
		if (m_pChild[i])
			m_pChild[i]->Display();
		else
			m_pRigidBody->AddToRenderList();
	}
	//m_pMeshMngr->AddWireCubeToRenderList(glm::scale(vector3(70)), C_BLUE);
}

MyOctant::MyOctant(MyOctant const& other)
{
	m_nData = other.m_nData;
	m_lData = other.m_lData;
}
MyOctant& MyOctant::operator=(MyOctant const& other)
{
	if(this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}
MyOctant::~MyOctant(){Release();};
//Accessors
void MyOctant::SetData(int a_nData){ m_nData = a_nData; }
int MyOctant::GetData(void){ return m_nData; }
void MyOctant::SetDataOnVector(int a_nData){ m_lData.push_back(a_nData);}
int& MyOctant::GetDataOnVector(int a_nIndex)
{
	int nIndex = static_cast<int>(m_lData.size());
	//assert(a_nIndex >= 0 && a_nIndex < nIndex);
	return m_lData[a_nIndex];
}
//--- Non Standard Singleton Methods

