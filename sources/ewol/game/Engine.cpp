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
	
}

void game::Engine::Process(int64_t lastTime, int32_t deltaTime)
{
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

void game::Engine::Draw(ewol::DrawProperty& displayProp)
{
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
