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
	ProcessIA(deltaTime);
	ProcessCollision(deltaTime);
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
			game::Bounding* bounding1 = m_elements[iii]->GetBounding();
			if (NULL != bounding1) {
				bounding1->SetContactMode(false);
			}
		}
	}
	
	for (int32_t iii=0; iii<m_elements.Size() ; iii++) {
		if (NULL != m_elements[iii]) {
			game::Bounding* bounding1 = m_elements[iii]->GetBounding();
			if (NULL != bounding1) {
				for (int32_t jjj=iii+1; jjj<m_elements.Size() ; jjj++) {
					if (NULL!=m_elements[jjj]) {
						game::Bounding* bounding2 = m_elements[jjj]->GetBounding();
						if (NULL != bounding2) {
							bool hasContactConfirmed = false;
							if (bounding1->GetType() < bounding2->GetType()) {
								//bounding2->GenerateContact(m_elements[jjj], bounding1, m_elements[iii], m_contacts);
								hasContactConfirmed = bounding2->HasContact(*bounding1);
							} else {
								//bounding1->GenerateContact(m_elements[iii], bounding2, m_elements[jjj], m_contacts);
								hasContactConfirmed = bounding1->HasContact(*bounding2);
							}
							if (true == hasContactConfirmed) {
								bounding2->SetContactMode(true);
								bounding1->SetContactMode(true);
							}
						}
					}
				}
			}
		}
	}
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
	for (int32_t iii=0 ; iii<m_elements.Size() ; iii++) {
		if (NULL == m_elements[iii]) {
			m_elements[iii] = newElement;
			find = true;
			break;
		}
	}
	if (false==find) {
		m_elements.PushBack(newElement);
	}
}


void game::Engine::AddGravity(game::Gravity gravity)
{
	m_gravity.PushBack(gravity);
}

