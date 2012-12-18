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
	for (int32_t iii=0; iii<m_elementsStatic.Size() ; iii++) {
		if (NULL != m_elementsStatic[iii]) {
			delete(m_elementsStatic[iii]);
			m_elementsStatic[iii] = NULL;
		}
	}
	m_elementsStatic.Clear();
	for (int32_t iii=0; iii<m_elementsDynamic.Size() ; iii++) {
		if (NULL != m_elementsDynamic[iii]) {
			delete(m_elementsDynamic[iii]);
			m_elementsDynamic[iii] = NULL;
		}
	}
	m_elementsDynamic.Clear();
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
		for (int32_t iii=0; iii<m_elementsStatic.Size() ; iii++) {
			if (NULL != m_elementsStatic[iii]) {
				m_elementsStatic[iii]->ProcessGravity(deltaTime, m_gravity[jjj]);
			}
			m_elementsStatic[iii]->ProcessSpeed(deltaTime);
			m_elementsStatic[iii]->ProcessPosition(deltaTime);
		}
		for (int32_t iii=0; iii<m_elementsDynamic.Size() ; iii++) {
			if (NULL != m_elementsDynamic[iii]) {
				m_elementsDynamic[iii]->ProcessGravity(deltaTime, m_gravity[jjj]);
			}
			m_elementsDynamic[iii]->ProcessSpeed(deltaTime);
			m_elementsDynamic[iii]->ProcessPosition(deltaTime);
		}
	}
}

void game::Engine::ProcessIA(float deltaTime)
{
	//EWOL_DEBUG("Artificial Intelligence management");
	for (int32_t iii=0; iii<m_elementsStatic.Size() ; iii++) {
		if (NULL != m_elementsStatic[iii]) {
			m_elementsStatic[iii]->ArtificialIntelligence(deltaTime);
		}
	}
	for (int32_t iii=0; iii<m_elementsDynamic.Size() ; iii++) {
		if (NULL != m_elementsDynamic[iii]) {
			m_elementsDynamic[iii]->ArtificialIntelligence(deltaTime);
		}
	}
}

void game::Engine::ProcessCollision(float deltaTime)
{
	//EWOL_DEBUG("Collision management");
	
}

void game::Engine::Draw(ewol::DrawProperty& displayProp)
{
	//EWOL_DEBUG("Drawing the system");
	for (int32_t iii=0; iii<m_elementsStatic.Size() ; iii++) {
		if (NULL != m_elementsStatic[iii]) {
			m_elementsStatic[iii]->Draw();
		}
	}
	for (int32_t iii=0; iii<m_elementsDynamic.Size() ; iii++) {
		if (NULL != m_elementsDynamic[iii]) {
			m_elementsDynamic[iii]->Draw();
		}
	}
}


void game::Engine::AddElement(game::Element* newElement, bool dynamic)
{
	bool find=false;
	if (true == dynamic) {
		for (int32_t iii=0 ; iii<m_elementsDynamic.Size() ; iii++) {
			if (NULL == m_elementsDynamic[iii]) {
				m_elementsDynamic[iii] = newElement;
				find = true;
				break;
			}
		}
		if (false==find) {
			m_elementsDynamic.PushBack(newElement);
		}
	} else {
		for (int32_t iii=0 ; iii<m_elementsStatic.Size() ; iii++) {
			if (NULL == m_elementsStatic[iii]) {
				m_elementsStatic[iii] = newElement;
				find = true;
				break;
			}
		}
		if (false==find) {
			m_elementsStatic.PushBack(newElement);
		}
	}
}


void game::Engine::AddGravity(game::Gravity gravity)
{
	m_gravity.PushBack(gravity);
}

