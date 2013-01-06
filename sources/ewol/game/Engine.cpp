/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */



#include <ewol/game/Engine.h>




game::Engine::Engine(void)
{
	
}

game::Engine::~Engine(void)
{
	for (int32_t iii=0; iii<m_elements.Size() ; iii++) {
		if (NULL != m_elements[iii]) {
			delete(m_elements[iii]);
			m_elements[iii] = NULL;
		}
	}
	m_elements.Clear();
}

void game::Engine::Process(double lastTime, float deltaTime)
{
	ProcessGravity(deltaTime);
	ProcessCollision(deltaTime);
	ProcessIA(deltaTime);
}

void game::Engine::ProcessGravity(float deltaTime)
{
	//EWOL_DEBUG("Gravity management");
	for(int32_t jjj=0 ; jjj<m_gravity.Size() ; jjj++) {
		for (int32_t iii=0; iii<m_elements.Size() ; iii++) {
			if (NULL != m_elements[iii]) {
				m_elements[iii]->ProcessGravity(deltaTime, m_gravity[jjj]);
			}
			m_elements[iii]->ProcessPosition(deltaTime);
		}
	}
}

void game::Engine::ProcessIA(float deltaTime)
{
	//EWOL_DEBUG("Artificial Intelligence management");
	for (int32_t iii=0; iii<m_elements.Size() ; iii++) {
		if (NULL != m_elements[iii]) {
			m_elements[iii]->ArtificialIntelligence(deltaTime);
		}
	}
}

void game::Engine::ProcessCollision(float deltaTime)
{
	//EWOL_DEBUG("Collision management");
	m_contacts.Clear();
	for (int32_t iii=0; iii<m_elements.Size() ; iii++) {
		if (NULL != m_elements[iii]) {
			game::BoundingAABB& bounding1 = m_elements[iii]->GetBounding();
			bounding1.SetContactMode(false);
		}
	}
	// Brut force bounding detection :
	for (int32_t iii=0; iii<m_elements.Size() ; iii++) {
		if (NULL != m_elements[iii]) {
			// If the element is static, the bounding detection is not needed...
			if (true) {//false == m_elements[iii]->GetStaticMode()) {
				game::BoundingAABB& bounding1 = m_elements[iii]->GetBounding();
				for (int32_t jjj=iii+1; jjj<m_elements.Size() ; jjj++) {
					if (NULL!=m_elements[jjj]) {
						game::BoundingAABB& bounding2 = m_elements[jjj]->GetBounding();
						if (true == bounding2.HasContact(bounding1)) {
							bounding2.SetContactMode(true);
							bounding1.SetContactMode(true);
						}
					}
				}
			}
		}
	}
}


bool game::Engine::HasCollision(game::BoundingAABB& bounding, game::Element* currentElement)
{
	// Brut force bounding detection :
	for (int32_t iii=0; iii<m_elements.Size() ; iii++) {
		if (currentElement == m_elements[iii]) {
			continue;
		}
		if (NULL != m_elements[iii]) {
			if (true == bounding.HasContact(m_elements[iii]->GetBounding())) {
				return true;
			}
		}
	}
	return false;
}



void game::Engine::Draw(ewol::DrawProperty& displayProp)
{
	//EWOL_DEBUG("Drawing the system");
	for (int32_t iii=0; iii<m_elements.Size() ; iii++) {
		if (NULL != m_elements[iii]) {
			m_elements[iii]->Draw();
		}
	}
	#ifdef DEBUG
		for (int32_t iii=0; iii<m_elements.Size() ; iii++) {
			if (NULL != m_elements[iii]) {
				m_elements[iii]->DrawDebug();
			}
		}
	#endif
}


void game::Engine::AddElement(game::Element* newElement)
{
	bool find=false;
	if (NULL == newElement) {
		EWOL_ERROR("try to set a NULL pointer game::Element");
	}
	for (int32_t iii=0 ; iii<m_elements.Size() ; iii++) {
		if (NULL == m_elements[iii]) {
			m_elements[iii] = newElement;
			find = true;
			break;
		}
	}
	if (false==find) {
		m_elements.PushBack(newElement);
		newElement->SetEngine(this);
	}
}


void game::Engine::AddGravity(game::Gravity gravity)
{
	m_gravity.PushBack(gravity);
}

