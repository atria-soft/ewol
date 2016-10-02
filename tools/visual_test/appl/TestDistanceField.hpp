/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license BSD v3 (see license file)
 */
#pragma once

#include <appl/debug.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/compositing/Text.hpp>
#include <ewol/compositing/TextDF.hpp>
#include <gale/resource/Program.hpp>
#include <ewol/resource/DistanceFieldFont.hpp>

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

