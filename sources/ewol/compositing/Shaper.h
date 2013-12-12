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
#include <ewol/resource/Program.h>
#include <ewol/resource/ConfigFile.h>
#include <ewol/resource/Image.h>
#include <ewol/event/Time.h>

namespace ewol {
	namespace compositing {
		/**
		 * @brief the Shaper system is a basic theme configuration for every widget, it corespond at a background display described by a pool of files
		 */
		// TODO : load image
		// TODO : Abstaraction between states (call by name and the system greate IDs
		class Shaper : public ewol::Compositing {
			private:
				std::string m_name; //!< Name of the configuration of the shaper.
				// External theme config:
				ewol::resource::ConfigFile* m_config; //!< pointer on the config file resources
				int32_t m_confIdPaddingX;   //!< ConfigFile padding property X
				int32_t m_confIdPaddingY;   //!< ConfigFile padding property Y
				int32_t m_confIdChangeTime; //!< ConfigFile padding transition time property
				int32_t m_confProgramFile;  //!< ConfigFile opengGl program Name
				int32_t m_confImageFile;    //!< ConfigFile opengGl program Name
				// openGL shaders programs:
				ewol::resource::Program* m_GLprogram; //!< pointer on the opengl display program
				int32_t m_GLPosition;           //!< openGL id on the element (vertex buffer)
				int32_t m_GLMatrix;             //!< openGL id on the element (transformation matrix)
				int32_t m_GLPropertySize;       //!< openGL id on the element (widget size)
				int32_t m_GLPropertyOrigin;     //!< openGL id on the element (widget origin)
				int32_t m_GLPropertyInsidePos;  //!< openGL id on the element (widget internal element position)
				int32_t m_GLPropertyInsideSize; //!< openGL id on the element (widget internal element size)
				int32_t m_GLStateOld;           //!< openGL id on the element (old state displayed)
				int32_t m_GLStateNew;           //!< openGL id on the element (new state displayed)
				int32_t m_GLStateTransition;    //!< openGL id on the element (transition ofset [0.0..1.0] )
				int32_t m_GLtexID;              //!< openGL id on the element (texture image)
				// For the Image :
				ewol::resource::TextureFile* m_resourceTexture; //!< texture resources (for the image)
				// internal needed data :
				int32_t m_nextStatusRequested;    //!< when status is changing, this represent the next step of it
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
				 * @brief load the openGL program and get all the ID needed
				 */
				void loadProgram(void);
				/**
				 * @brief Un-Load the openGL program and get all the ID needed
				 */
				void unLoadProgram(void);
			public:
				/**
				 * @brief generic constructor
				 * @param[in] _shaperName Name of the file that might be loaded
				 */
				Shaper(const std::string& _shaperName="");
				/**
				 * @brief generic destructor
				 */
				~Shaper(void);
			public:
				/**
				 * @brief draw All the refistered text in the current element on openGL
				 */
				void draw(bool _disableDepthTest=true);
				/**
				 * @brief clear alll tre registered element in the current element
				 */
				void clear(void);
				/**
				 * @brief change the current status in an other
				 * @param[in] _newStatusId the next new status requested
				 * @return true The widget must call this fuction periodicly (and redraw itself)
				 * @return false No need to request the periodic call.
				 */
				bool changeStatusIn(int32_t _newStatusId);
				/**
				 * @brief get the current displayed status of the shaper
				 * @return The Status Id
				 */
				int32_t getCurrentDisplayedStatus(void) {
					return m_stateNew;
				};
				/**
				 * @brief get the next displayed status of the shaper
				 * @return The next status Id (-1 if no status in next)
				 */
				int32_t getNextDisplayedStatus(void) {
					return m_nextStatusRequested;
				};
				/**
				 * @brief get the current trasion status
				 * @return value of the transition status (0.0f when no activity)
				 */
				float getTransitionStatus(void) {
					return m_stateTransition;
				};
				/**
				 * @brief Same as the widfget periodic call (this is for change display)
				 * @param[in] _event The current time of the call.
				 * @return true The widget must call this fuction periodicly (and redraw itself)
				 * @return false No need to request the periodic call.
				 */
				bool periodicCall(const ewol::event::Time& _event);
				/**
				 * @brief set the widget origin (needed fot the display)
				 * @param[in] _newOri : the new widget origin
				 */
				void setOrigin(const vec2& _newOri);
				/**
				 * @brief set the widget size (needed fot the display)
				 * @param[in] _newSize : the new widget size
				 */
				void setSize(const vec2& _newSize);
				/**
				 * @brief set the internal widget size
				 * @param[in] _newInsidePos : the subelement size.
				 */
				void setInsideSize(const vec2& _newInsideSize);
				/**
				 * @brief set the internal widget position
				 * @param[in] _newInsidePos : the subelement position
				 */
				void setInsidePos(const vec2& _newInsidePos);
				/**
				 * @brief get the padding declared by the user in the config file
				 * @return the padding property
				 */
				vec2 getPadding(void);
				/**
				 * @brief change the shaper Source
				 * @param[in] _newFile New file of the shaper
				 */
				void setSource(const std::string& _newFile);
				/**
				 * @brief get the shaper file Source
				 * @return the shapper file name
				 */
				const std::string& getSource(void) const {
					return m_name;
				};
				/**
				 * @brief Sometimes the user declare an image but not allocate the ressources all the time, this is to know it ..
				 * @return the validity od the resources.
				 */
				bool hasSources(void);
			private:
				/**
				 * @brief update the internal vertex table.
				 */
				void updateVertex(void);
		};
	};
};

#endif

