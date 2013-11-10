/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_SIZER_H__
#define __EWOL_SIZER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/ContainerN.h>
#include <ewol/widget/WidgetManager.h>

namespace widget {
	/**
	 * @ingroup ewolWidgetGroup
	 */
	class Sizer : public widget::ContainerN {
		public:
			/**
			 * @brief Main call of recording the widget on the List of "widget named creator"
			 */
			static void init(ewol::WidgetManager& _widgetManager);
		public:
			enum displayMode {
				modeVert, //!< Vertical mode
				modeHori, //!< Horizontal mode
			};
		private:
			enum displayMode m_mode; //!< Methode to display the widget list (vert/hory ...)
		public:
			/**
			 * @brief Constructor
			 * @param[in] _mode The mode to display the elements
			 */
			Sizer(enum displayMode _mode=widget::Sizer::modeHori);
			/**
			 * @brief Desstructor
			 */
			virtual ~Sizer(void);
			/**
			 * @brief set the mode to display elements.
			 * @param[in] _mode The mode to display the elements.
			 */
			void setMode(enum displayMode _mode);
			/**
			 * @brief get the mode to display elements.
			 * @return The current mode to display the elements.
			 */
			enum displayMode getMode(void);
		private:
			ewol::Dimension m_borderSize; //!< Border size needed for all the display
		public:
			/**
			 * @brief set the current border size of the current element:
			 * @param[in] _newBorderSize The border size to set (0 if not used)
			 */
			void setBorderSize(const ewol::Dimension& _newBorderSize);
			/**
			 * @brief get the current border size of the current element:
			 * @return the border size (0 if not used)
			 */
			const ewol::Dimension& getBorderSize(void) {
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
			enum animation getAnimationMode(void) {
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
			float getAnimationTime(void) {
				return m_animationTime;
			};
		public: // Derived function
			virtual const char * const getObjectType(void) {
				return "widget::Sizer";
			};
			virtual void calculateSize(const vec2& _availlable);
			virtual void calculateMinMaxSize(void);
			virtual bool loadXML(exml::Element* _node);
			// overwrite the set fuction to start annimations ...
			virtual int32_t subWidgetAdd(ewol::Widget* _newWidget);
			virtual int32_t subWidgetAddStart(ewol::Widget* _newWidget);
			virtual void subWidgetRemove(ewol::Widget* _newWidget);
			virtual void subWidgetUnLink(ewol::Widget* _newWidget);
	};
	
};

#endif
