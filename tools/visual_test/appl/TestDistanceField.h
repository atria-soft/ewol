/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */
#pragma once

#include <appl/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Text.h>
#include <ewol/compositing/TextDF.h>
#include <gale/resource/Program.h>
#include <ewol/resource/DistanceFieldFont.h>

namespace appl {
	class TestDistanceField : public ewol::Widget {
			ewol::compositing::Text   m_text1;
			ewol::compositing::TextDF m_text2;
		public:
			// Constructeur
			TestDistanceField();
			void init();
		public:
			DECLARE_FACTORY(TestDistanceField);
			virtual ~TestDistanceField() {};
		public: // Derived function
			virtual void onDraw();
			virtual void calculateMinMaxSize();
			virtual void calculateSize(const vec2& _availlable);
			virtual void onRegenerateDisplay();
			virtual bool onEventInput(const ewol::event::Input& _event);
	};
}

