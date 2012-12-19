/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__

#include <appl/Debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Scene.h>
#include <ewol/widget/SizerVert.h>
#include <ewol/game/Engine.h>

class TestScene : public widget::SizerVert
{
	private:
		game::Element* m_ground;
		//game::Element  m_sky;
		game::Engine   m_gameEngine;
		widget::Scene* m_testWidget;
	public:
		// Constructeur
		TestScene(void);
		virtual ~TestScene(void);
		// Derived function
		virtual const char * const GetObjectType(void) { return "TestButton"; };
		// Derived function
		virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
		// Derived function
		virtual void OnObjectRemove(ewol::EObject * removeObject);
};

#endif
