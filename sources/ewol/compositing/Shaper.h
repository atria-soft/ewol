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
#include <ewol/renderer/EventTime.h>

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
			int32_t           m_confImageFile;    //!< ConfigFile OpengGl program Name
			// OpenGL shaders programs:
			ewol::Program* m_GLprogram;            //!< pointer on the opengl display program
			int32_t        m_GLPosition;           //!< openGL id on the element (vertex buffer)
			int32_t        m_GLMatrix;             //!< openGL id on the element (transformation matrix)
			int32_t        m_GLPropertySize;       //!< openGL id on the element (widget size)
			int32_t        m_GLPropertyOrigin;     //!< openGL id on the element (widget origin)
			int32_t        m_GLPropertyInsidePos;  //!< openGL id on the element (widget internal element position)
			int32_t        m_GLPropertyInsideSize; //!< openGL id on the element (widget internal element size)
			int32_t        m_GLStateOld;           //!< openGL id on the element (old state displayed)
			int32_t        m_GLStateNew;           //!< openGL id on the element (new state displayed)
			int32_t        m_GLStateTransition;    //!< openGL id on the element (transition ofset [0.0..1.0] )
			int32_t        m_GLtexID;              //!< openGL id on the element (texture image)
			// For the Image :
			ewol::TextureFile* m_resourceTexture; //!< texture resources (for the image)
			// internal needed data :
			int32_t m_nextStatusRequested;    //!< when status is changing, this represent the next step of it
			int64_t m_time;                   //!< The last time of the dispaly (-1 if nothing progressing)
			vec2    m_propertyOrigin;         //!< widget origin
			vec2    m_propertySize;           //!< widget size
			vec2    m_propertyInsidePosition; //!< internal subwidget position
			vec2    m_propertyInsideSize;     //!< internal subwidget size
			int32_t m_stateOld;               //!< previous state
			int32_t m_stateNew;               //!< destination state
			float   m_stateTransition;        //!< working state between 2 states
			vec2    m_coord[6];               //!< the double triangle coordonates
		private:
			/**
			 * @brief Load the openGL program and get all the ID needed
			 */
			void LoadProgram(void);
			/**
			 * @brief Un-Load the openGL program and get all the ID needed
			 */
			void UnLoadProgram(void);
		public:
			/**
			 * @brief generic constructor
			 * @param[in] _shaperName Name of the file that might be loaded
			 */
			Shaper(const etk::UString& _shaperName="");
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
			 * @param[in] _newStatusId the next new status requested
			 * @return true The widget must call this fuction periodicly (and redraw itself)
			 * @return false No need to request the periodic call.
			 */
			bool ChangeStatusIn(int32_t _newStatusId);
			/**
			 * @brief Get the current displayed status of the shaper
			 * @return The Status Id
			 */
			int32_t GetCurrentDisplayedStatus(void);;
			/**
			 * @brief Get the next displayed status of the shaper
			 * @return The next status Id (-1 if no status in next)
			 */
			int32_t GetNextDisplayedStatus(void);
			/**
			 * @brief Same as the widfget periodic call (this is for change display)
			 * @param[in] _event The current time of the call.
			 * @return true The widget must call this fuction periodicly (and redraw itself)
			 * @return false No need to request the periodic call.
			 */
			bool PeriodicCall(const ewol::EventTime& _event);
			/**
			 * @brief Set the widget origin (needed fot the display)
			 * @param[in] _newOri : the new widget origin
			 */
			void SetOrigin(const vec2& _newOri);
			/**
			 * @brief Set the widget size (needed fot the display)
			 * @param[in] _newSize : the new widget size
			 */
			void SetSize(const vec2& _newSize);
			/**
			 * @brief Set the internal widget size
			 * @param[in] _newInsidePos : the subelement size.
			 */
			void SetInsideSize(const vec2& _newInsideSize);
			/**
			 * @brief Set the internal widget position
			 * @param[in] _newInsidePos : the subelement position
			 */
			void SetInsidePos(const vec2& _newInsidePos);
			/**
			 * @brief Get the padding declared by the user in the config file
			 * @return the padding property
			 */
			vec2 GetPadding(void);
			/**
			 * @brief Change the shaper Source
			 * @param[in] _newFile New file of the shaper
			 */
			void SetSource(const etk::UString& _newFile);
			/**
			 * @brief Get the shaper file Source
			 * @return the shapper file name
			 */
			const etk::UString& GetSource(void) const { return m_name; };
			/**
			 * @brief Sometimes the user declare an image but not allocate the ressources all the time, this is to know it ..
			 * @return the validity od the resources.
			 */
			bool HasSources(void);
		private:
			/**
			 * @brief Update the internal vertex table.
			 */
			void UpdateVectex(void);
	};
};

#endif

