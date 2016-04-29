/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
#pragma once

#include <etk/Color.h>
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Image.h>

namespace appl {
	class WidgetDisplay;
	using WidgetDisplayShared = ememory::SharedPtr<appl::WidgetDisplay>;
	using WidgetDisplayWeak = ememory::WeakPtr<appl::WidgetDisplay>;
	class WidgetDisplay : public ewol::Widget {
		private:
			class Element {
				public:
					Element();
					vec2 m_position;
					vec2 m_size;
					float m_angle;
					float m_lifeTime;
					float m_life;
					float m_angleAdd;
					float m_downSpeed;
					void regenerate(const vec2& _size);
					void move(float _deltaTime);
			};
		protected:
			WidgetDisplay();
			void init();
		public:
			DECLARE_FACTORY(WidgetDisplay);
			virtual ~WidgetDisplay();
			ewol::compositing::Image m_compositing;
			std::vector<Element> m_elements;
		public: // Derived function
			void onRegenerateDisplay();
			virtual void periodicCall(const ewol::event::Time& _event);
			virtual bool onEventInput(const ewol::event::Input& _event);
			virtual void onDraw();
	};
}


