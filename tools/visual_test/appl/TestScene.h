/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__

#include <appl/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Scene.h>
#include <ewol/widget/Sizer.h>

class TestScene : public ewol::widget::Sizer {
	private:
		std::shared_ptr<ewol::widget::Scene> m_testWidget;
	public:
		// Constructeur
		TestScene();
		virtual ~TestScene();
	public: // Derived function
		virtual void onReceiveMessage(const ewol::object::Message& _msg);
};

#endif
