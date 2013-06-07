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

namespace widget {
	class Sizer : public widget::ContainerN
	{
		public:
			/**
			 * @brief Main call of recording the widget on the List of "widget named creator"
			 */
			static void Init(void);
			/**
			 * @brief Main call to unrecord the widget from the list of "widget named creator"
			 */
			static void UnInit(void);
		public:
			typedef enum {
				modeVert, //!< Vertical mode
				modeHori, //!< Horizontal mode
			} displayMode_te;
		private:
			displayMode_te m_mode; //!< Methode to display the widget list (vert/hory ...)
		public:
			/**
			 * @brief Constructor
			 * @param[in] _mode The mode to display the elements
			 */
			Sizer(displayMode_te _mode=widget::Sizer::modeHori);
			/**
			 * @brief Desstructor
			 */
			virtual ~Sizer(void);
			/**
			 * @brief Set the mode to display elements.
			 * @param[in] _mode The mode to display the elements.
			 */
			void SetMode(displayMode_te _mode);
			/**
			 * @brief Get the mode to display elements.
			 * @return The current mode to display the elements.
			 */
			displayMode_te GetMode(void);
		private:
			ewol::Dimension m_borderSize; //!< Border size needed for all the display
		public:
			/**
			 * @brief Set the current border size of the current element:
			 * @param[in] _newBorderSize The border size to set (0 if not used)
			 */
			void SetBorderSize(const ewol::Dimension& _newBorderSize);
			/**
			 * @brief Get the current border size of the current element:
			 * @return the border size (0 if not used)
			 */
			const ewol::Dimension& GetBorderSize(void) { return m_borderSize; };
		public:
			typedef enum {
				animationNone, //!< No annimation
				animationTop, //!< element came from the top
				animationbuttom, //!< element came from the buttom
				animationLeft, //!< element came from the Left
				animationRight //!< element came from the right
				//animationZoom //!< element came from zooming
			} animation_te;
		private:
			animation_te m_animation; //!< Methode add and remove element (animation)
		public:
			/**
			 * @brief Set an animation mode for the new element set in the Widget container.
			 * @param[in] _animation The new animation mode.
			 */
			void SetAnimationMode(animation_te _animation) { m_animation = _animation; };
			/**
			 * @brief Get the current animation mode.
			 * @return The animation mode.
			 */
			animation_te GetAnimationMode(void) { return m_animation;};
		private:
			float m_animationTime; //!< Time in second to generate animation
		public:
			/**
			 * @brief Set the time to produce animation.
			 * @param[in] _time The new animation time.
			 */
			void SetAnimationTime(float _time) { m_animationTime = _time; };
			/**
			 * @brief Get the current animation time.
			 * @return The time to produce the animation.
			 */
			float GetAnimationTime(void) { return m_animationTime;};
		public: // Derived function
			virtual const char * const GetObjectType(void) { return "ewol::widget::sizer"; };
			virtual void CalculateSize(const vec2& _availlable);
			virtual void CalculateMinMaxSize(void);
			virtual bool LoadXML(TiXmlNode* _node);
			// overwrite the set fuction to start annimations ...
			virtual int32_t SubWidgetAdd(ewol::Widget* _newWidget);
			virtual int32_t SubWidgetAddStart(ewol::Widget* _newWidget);
			virtual void SubWidgetRemove(ewol::Widget* _newWidget);
			virtual void SubWidgetUnLink(ewol::Widget* _newWidget);
	};
	
};

#endif
