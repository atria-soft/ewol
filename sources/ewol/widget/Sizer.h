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
		public: // Derived function
			virtual const char * const GetObjectType(void) { return "Ewol::Sizer"; };
			virtual void CalculateSize(const vec2& _availlable);
			virtual void CalculateMinMaxSize(void);
			virtual bool LoadXML(TiXmlNode* _node);
	};
	
};

#endif
