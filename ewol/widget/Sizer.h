/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_SIZER_H__
#define __EWOL_SIZER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/ContainerN.h>
#include <ewol/widget/Manager.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Sizer : public ewol::widget::ContainerN {
			public:
				/**
				 * @brief Main call of recording the widget on the List of "widget named creator"
				 */
				static void init(ewol::widget::Manager& _widgetManager);
			public:
				enum displayMode {
					modeVert, //!< Vertical mode
					modeHori, //!< Horizontal mode
				};
			private:
				ewol::parameter::List<enum displayMode> m_mode; //!< Methode to display the widget list (vert/hory ...)
			protected:
				/**
				 * @brief Constructor
				 * @param[in] _mode The mode to display the elements
				 */
				Sizer();
				void init(enum displayMode _mode=ewol::widget::Sizer::modeHori);
			public:
				DECLARE_WIDGET_FACTORY(Sizer, "Sizer");
				/**
				 * @brief Desstructor
				 */
				virtual ~Sizer();
				/**
				 * @brief set the mode to display elements.
				 * @param[in] _mode The mode to display the elements.
				 */
				void setMode(enum displayMode _mode) {
					m_mode.set(_mode);
				}
				/**
				 * @brief get the mode to display elements.
				 * @return The current mode to display the elements.
				 */
				enum displayMode getMode() const {
					return m_mode;
				}
			private:
				ewol::parameter::Value<gale::Dimension> m_borderSize; //!< Border size needed for all the display
			public:
				/**
				 * @brief set the current border size of the current element:
				 * @param[in] _newBorderSize The border size to set (0 if not used)
				 */
				void setBorderSize(const gale::Dimension& _newBorderSize) {
					m_borderSize.set(_newBorderSize);
				}
				/**
				 * @brief get the current border size of the current element:
				 * @return the border size (0 if not used)
				 */
				const gale::Dimension& getBorderSize() const {
					return m_borderSize;
				};
			public:
				enum animation {
					animationNone, //!< No annimation
					animationTop, //!< element came from the top
					animationbuttom, //!< element came from the buttom
					animationLeft, //!< element came from the Left
					animationRight //!< element came from the right
					//animationZoom //!< element came from zooming
				};
			private:
				enum animation m_animation; //!< Methode add and remove element (animation)
			public:
				/**
				 * @brief set an animation mode for the new element set in the Widget container.
				 * @param[in] _animation The new animation mode.
				 */
				void setAnimationMode(enum animation _animation) {
					m_animation = _animation;
				};
				/**
				 * @brief get the current animation mode.
				 * @return The animation mode.
				 */
				enum animation getAnimationMode() {
					return m_animation;
				};
			private:
				float m_animationTime; //!< Time in second to generate animation
			public:
				/**
				 * @brief set the time to produce animation.
				 * @param[in] _time The new animation time.
				 */
				void setAnimationTime(float _time) {
					m_animationTime = _time;
				};
				/**
				 * @brief get the current animation time.
				 * @return The time to produce the animation.
				 */
				float getAnimationTime() {
					return m_animationTime;
				};
			public: // Derived function
				virtual void calculateSize(const vec2& _availlable);
				virtual void calculateMinMaxSize();
				// overwrite the set fuction to start annimations ...
				virtual int32_t subWidgetAdd(std::shared_ptr<ewol::Widget> _newWidget);
				virtual int32_t subWidgetAddStart(std::shared_ptr<ewol::Widget> _newWidget);
				virtual void subWidgetRemove(std::shared_ptr<ewol::Widget> _newWidget);
				virtual void subWidgetUnLink(std::shared_ptr<ewol::Widget> _newWidget);
				virtual void onParameterChangeValue(const ewol::parameter::Ref& _paramPointer);
		};
	};
};

#endif
