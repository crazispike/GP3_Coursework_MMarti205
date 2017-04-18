#include "cEnemy.h"

cEnemy::cEnemy() : cModel()
{
	
}

void cEnemy::randomise()
{
	if (rand() % 1 == 1)
	{
		cModel::m_mdlPosition.x = 10.0f;
	}
	
	cModel::m_mdlPosition.z = 22.0f;
	cModel::m_mdlPosition.x = 0.0f;  
	cModel::m_mdlDirection.z = -0.3f; 
	cModel::m_mdlSpeed = m_EnemyMinSpeed + rand() / (float)RAND_MAX * m_EnemyMaxSpeed;
	cModel::m_IsActive = true;
}

void cEnemy::update(float elapsedTime)
{
	cModel::m_mdlPosition += cModel::m_mdlDirection * cModel::m_mdlSpeed * elapsedTime;
	if (cModel::m_mdlPosition.z > PLAYFIELDX)
		cModel::m_mdlPosition.z -= 2 * PLAYFIELDX;
	if (cModel::m_mdlPosition.z < -PLAYFIELDX)
		cModel::m_mdlPosition.z+= 2 * PLAYFIELDX;
	if (cModel::m_mdlPosition.z > PLAYFIELDZ)
		cModel::m_mdlPosition.z -= 2 * PLAYFIELDZ;
	if (cModel::m_mdlPosition.z < -PLAYFIELDZ)
		cModel::m_mdlPosition.z += 2 * PLAYFIELDZ;
}

cEnemy::~cEnemy()
{

}

