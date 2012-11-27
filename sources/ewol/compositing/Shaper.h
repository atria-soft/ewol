/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_COMPOSITING_SHAPER_H__
#define __EWOL_COMPOSITING_SHAPER_H__

#include <ewol/debug.h>
#include <ewol/compositing/Compositing.h>
#include <ewol/renderer/ResourceManager.h>

namespace ewol
{
	/**
	 * @brief the Shaper system is a basic theme configuration for every widget, it corespond at a background display described by a pool of files
	 */
	// TODO : Load image
	// TODO : Abstaraction between states (call by name and the system greate IDs
	class Shaper : public ewol::Compositing
	{
		private:
			etk::UString m_name; //!< Name of the configuration of the shaper.
			// External theme config:
			ewol::ConfigFile* m_config;           //!< pointer on the config file resources
			int32_t           m_confIdPaddingX;   //!< ConfigFile padding property X
			int32_t           m_confIdPaddingY;   //!< ConfigFile padding property Y
			int32_t           m_confIdChangeTime; //!< ConfigFile padding transition time property
			int32_t           m_confProgramFile;  //!< ConfigFile OpengGl program Name
			// OpenGL shaders programs:
			ewol::Program* m_GLprogram;            //!< pointer on the opengl display program
			int32_t        m_GLPosition;           //!< openGL id on the element (vertex buffer)
			int32_t        m_GLMatrix;             //!< openGL id on the element (transformation matrix)
			int32_t        m_GLPropertySize;       //!< openGL id on the element (widget size)
			int32_t        m_GLPropertyInsidePos;  //!< openGL id on the element (widget internal element position)
			int32_t        m_GLPropertyInsideSize; //!< openGL id on the element (widget internal element size)
			int32_t        m_GLStateOld;           //!< openGL id on the element (old state displayed)
			int32_t        m_GLStateNew;           //!< openGL id on the element (new state displayed)
			int32_t        m_GLStateTransition;    //!< openGL id on the element (transition ofset [0.0..1.0] )
			// internal needed data :
			int32_t              m_nextStatusRequested;    //!< when status is changing, this represent the next step of it
			int64_t              m_time;                   //!< The last time of the dispaly (-1 if nothing progressing)
			etk::Vector2D<float> m_propertySize;           //!< widget size
			etk::Vector2D<float> m_propertyInsidePosition; //!< internal subwidget position
			etk::Vector2D<float> m_propertyInsideSize;     //!< internal subwidget size
			int32_t              m_stateOld;               //!< previous state
			int32_t              m_stateNew;               //!< destination state
			float                m_stateTransition;        //!< working state between 2 states
			etk::Vector2D<float> m_coord[6];               //!< the double triangle coordonates
		private:
			/**
			 * @brief Load the openGL program and get all the ID needed
			 */
			void LoadProgram(void);
		public:
			/**
			 * @brief generic constructor
			 * @param[in] imageName Name of the file that might be loaded
			 */
			Shaper(etk::UString shaperName="");
			/**
			 * @brief generic destructor
			 */
			~Shaper(void);
		public:
			/**
			 * @brief Draw All the refistered text in the current element on openGL
			 */
			void Draw(void);
			/**
			 * @brief Clear alll tre registered element in the current element
			 */
			void Clear(void);
			/**
			 * @brief change the current status in an other
			 * @param[in] the next new status requested
			 * @return true The widget must call this fuction periodicly (and redraw itself)
			 * @return false No need to request the periodic call.
			 */
			bool ChangeStatusIn(int32_t newStatusId);
			/**
			 * @brief Same as the widfget periodic call (this is for change display)
			 * @param[in] localTime The current time of the call.
			 * @return true The widget must call this fuction periodicly (and redraw itself)
			 * @return false No need to request the periodic call.
			 */
			bool PeriodicCall(int64_t localTime);
			/**
			 * @brief Set the widget size (needed fot the display)
			 * @param[in] newSize : the new widget size
			 */
			void SetSize(etk::Vector2D<float> newSize);
			/**
			 * @brief Set the internal widget size
			 * @param[in] newInsidePos : the subelement size.
			 */
			void SetInsideSize(etk::Vector2D<float> newInsideSize);
			/**
			 * @brief Set the internal widget position
			 * @param[in] newInsidePos : the subelement position
			 */
			void SetInsidePos(etk::Vector2D<float> newInsidePos);
			/**
			 * @brief Get the padding declared by the user in the config file
			 * @return the padding property
			 */
			etk::Vector2D<float> GetPadding(void);
			/**
			 * @brief Change the image Source ==> can not be done to display 2 images at the same time ...
			 * @param[in] newFile New file of the Image
			 */
			void SetSource(etk::UString newFile);
			/**
			 * @brief Sometimes the user declare an image but not allocate the ressources all the time, this is to know it ..
			 * @return the validity od the resources.
			 */
			bool HasSources(void);
	};
};

#endif

