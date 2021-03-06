/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ewol/debug.hpp>
#include <ewol/compositing/Compositing.hpp>
#include <gale/resource/Program.hpp>
#include <ewol/resource/ConfigFile.hpp>
#include <ewol/resource/ColorFile.hpp>
#include <ewol/resource/TextureFile.hpp>
#include <ewol/event/Time.hpp>
#include <ewol/Padding.hpp>

namespace ewol {
	namespace compositing {
		enum renderMode {
			renderSingleSquare, //!< basic historic render mode
			renderBorder, //!< Render 4 squares for coiner, and renctangle for border, a big rentangle for background and 8 rectangle for the outside part
			renderOneBorder,
		};
		#define SHAPER_NB_MAX_QUAD (5*5)
		#define SHAPER_NB_MAX_TRIANGLE (SHAPER_NB_MAX_QUAD*2)
		#define SHAPER_NB_MAX_VERTEX (SHAPER_NB_MAX_TRIANGLE*3)
		enum shaperPos {
			shaperPosLeft,
			shaperPosRight,
			shaperPosTop,
			shaperPosButtom,
			shaperPosCount,
		};
		/**
		 * @brief the Shaper system is a basic theme configuration for every widget, it corespond at a background display described by a pool of files
		 */
		// TODO : load image
		// TODO : Abstaraction between states (call by name and the system greate IDs
		class Shaper : public ewol::Compositing {
			private:
				etk::Uri m_uri; //!< Name of the configuration of the shaper.
				// External theme config:
				ememory::SharedPtr<ewol::resource::ConfigFile> m_config; //!< pointer on the config file resources
				int32_t m_confIdPaddingOut[shaperPosCount]; //!< Padding out property : X-left X-right Y-top Y-buttom
				int32_t m_confIdBorder[shaperPosCount]; //!< border property : X-left X-right Y-top Y-buttom
				int32_t m_confIdPaddingIn[shaperPosCount]; //!< Padding in property : X-left X-right Y-top Y-buttom
				int32_t m_confIdMode; //!< Display mode
				int32_t m_confIdDisplayOutside; //!< Display outside of the shape...
				int32_t m_confIdChangeTime;    //!< ConfigFile padding transition time property
				int32_t m_confProgramFile;     //!< ConfigFile opengGl program Name
				int32_t m_confColorFile;       //!< ConfigFile opengGl color file Name
				int32_t m_confImageFile;       //!< ConfigFile opengGl program Name
				// openGL shaders programs:
				ememory::SharedPtr<gale::resource::Program> m_GLprogram; //!< pointer on the opengl display program
				int32_t m_GLPosition;           //!< openGL id on the element (vertex buffer)
				int32_t m_GLMatrix;             //!< openGL id on the element (transformation matrix)
				int32_t m_GLPropertyPos;       //!< openGL id on the element (simple ratio position in the widget : ____/-----\_____ on vec2(X,Y))
				int32_t m_GLStateActivate;      //!< openGL id on the element (activate state displayed)
				int32_t m_GLStateOld;           //!< openGL id on the element (old state displayed)
				int32_t m_GLStateNew;           //!< openGL id on the element (new state displayed)
				int32_t m_GLStateTransition;    //!< openGL id on the element (transition ofset [0.0..1.0] )
				int32_t m_GLtexID;              //!< openGL id on the element (texture image)
				// For the Image :
				ememory::SharedPtr<ewol::resource::TextureFile> m_resourceTexture; //!< texture resources (for the image)
				// internal needed data :
				int32_t m_nextStatusRequested;    //!< when status is changing, this represent the next step of it
				vec2    m_propertyOrigin;         //!< widget origin
				vec2    m_propertySize;           //!< widget size
				vec2    m_propertyInsidePosition; //!< internal subwidget position
				vec2    m_propertyInsideSize;     //!< internal subwidget size
				int32_t m_stateActivate;          //!< Activate state of the element
				int32_t m_stateOld;               //!< previous state
				int32_t m_stateNew;               //!< destination state
				float   m_stateTransition;        //!< working state between 2 states
				int32_t m_nbVertexToDisplay;
				// color management theme:
				ememory::SharedPtr<ewol::resource::ColorFile> m_colorProperty; //!< input resource for color management
				etk::Vector<ivec2> m_listAssiciatedId; //!< Corellation ID between ColorProperty (Y) and OpenGL Program (X)
			protected:
				static const int32_t m_vboIdCoord;
				static const int32_t m_vboIdPos;
				ememory::SharedPtr<gale::resource::VirtualBufferObject> m_VBO;
			private:
				/**
				 * @brief load the openGL program and get all the ID needed
				 */
				void loadProgram();
				/**
				 * @brief Un-Load the openGL program and get all the ID needed
				 */
				void unLoadProgram();
			public:
				/**
				 * @brief generic constructor
				 * @param[in] _uri URI of the file that might be loaded
				 */
				Shaper(const etk::Uri& _uri="");
				/**
				 * @brief generic destructor
				 */
				virtual ~Shaper();
			public:
				/**
				 * @brief draw All the refistered text in the current element on openGL
				 */
				void draw(bool _disableDepthTest=true);
				/**
				 * @brief clear alll tre registered element in the current element
				 */
				void clear();
				/**
				 * @brief Change the current state
				 * @param[in] _newState Current state of the configuration
				 * @return true Need redraw.
				 * @return false No need redraw.
				 */
				bool setState(int32_t _newState);
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
				int32_t getCurrentDisplayedStatus() {
					return m_stateNew;
				};
				/**
				 * @brief get the next displayed status of the shaper
				 * @return The next status Id (-1 if no status in next)
				 */
				int32_t getNextDisplayedStatus() {
					return m_nextStatusRequested;
				};
				/**
				 * @brief get the current trasion status
				 * @return value of the transition status (0.0f when no activity)
				 */
				float getTransitionStatus() {
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
				 * @brief get the padding declared by the user in the config file
				 * @return the padding property
				 */
				ewol::Padding getPadding();
				ewol::Padding getPaddingIn();
				ewol::Padding getPaddingOut();
				/**
				 * @brief get the padding declared by the user in the config file
				 * @return the padding property
				 */
				ewol::Padding getBorder();
				/**
				 * @brief change the shaper Source
				 * @param[in] _uri New file of the shaper
				 */
				void setSource(const etk::Uri& _uri);
				/**
				 * @brief get the shaper file Source
				 * @return the shapper file name
				 */
				const etk::Uri& getSource() const {
					return m_uri;
				};
				/**
				 * @brief Sometimes the user declare an image but not allocate the ressources all the time, this is to know it ..
				 * @return the validity od the resources.
				 */
				bool hasSources();
			public:
				/**
				 * @brief set the shape property:
				 * 
				 *   ********************************************************************************
				 *   *                                                                        _size *
				 *   *                                                                              *
				 *   *        * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *       *
				 *   *                                                                              *
				 *   *        |                                                             |       *
				 *   *             ***************************************************              *
				 *   *        |    *                                                 *      |       *
				 *   *             *                                                 *              *
				 *   *        |    *     * - - - - - - - - - - - - - - - - - - *     *      |       *
				 *   *             *                                _insideSize      *              *
				 *   *        |    *     |                                     |     *      |       *
				 *   *             *                                                 *              *
				 *   *        |    *     |                                     |     *      |       *
				 *   *             *                                                 *              *
				 *   *        |    *     |                                     |     *      |       *
				 *   *             *                                                 *              *
				 *   *        |    *     |                                     |     *      |       *
				 *   *             *                                                 *              *
				 *   *        |    *     |                                     |     *      |       *
				 *   *             *                                                 *              *
				 *   *        |    *     |                                     |     *      |       *
				 *   *             *      _insidePos                                 *              *
				 *   *        |    *     * - - - - - - - - - - - - - - - - - - *     *      |       *
				 *   *             *                                                 *              *
				 *   *        |    ***************************************************      |       *
				 *   *                                                                              *
				 *   *        |                                                             |       *
				 *   *                                                                              *
				 *   *        * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *       *
				 *   *                                                                              *
				 *   *                                                                              *
				 *   ********************************************************************************
				 *   _origin
				 *
				 *
				 * @param[in] _origin Origin of the display
				 * @param[in] _size Size of the display
				 * @param[in] _insidePos Positin of the internal data
				 * @param[in] _insideSize Size of the internal data
				 */
				void setShape(const vec2& _origin, const vec2& _size, const vec2& _insidePos, const vec2& _insideSize);
				// @previous
				void setShape(const vec2& _origin, const vec2& _size) {
					ewol::Padding tmp = getPadding();
					setShape(_origin, _size, _origin+vec2(tmp.xLeft(), tmp.yButtom()), _size - vec2(tmp.x(), tmp.y()));
				}
			public:
				/**
				 * @brief Get an ID on the color instance element
				 * @param[in] _name Name of the element requested
				 * @return The Id of the color
				 */
				int32_t requestColor(const etk::String& _name);
				/**
				 * @brief Get The color associated at an ID.
				 * @param[in] _id Id of the color
				 * @return the reference on the color
				 */
				const etk::Color<float>& getColor(int32_t _id);
			public:
				/**
				 * @brief Get an ID on the configuration instance element
				 * @param[in] _name Name of the element requested
				 * @return The Id of the element
				 */
				int32_t requestConfig(const etk::String& _name);
				/**
				 * @brief Get The number associated at an ID.
				 * @param[in] _id Id of the parameter
				 * @return the requested number.
				 */
				double getConfigNumber(int32_t _id);
			public:
				/**
				 * @brief Set activate state of the element
				 * @param[in] _status New activate status
				 */
				void setActivateState(int32_t _status) {
					m_stateActivate = _status;
				}
			private:
				void addVertexLine(float _yTop,
				                   float _yButtom,
				                   float _x1,
				                   float _x2,
				                   float _x3,
				                   float _x4,
				                   float _x5,
				                   float _x6,
				                   float _x7,
				                   float _x8,
				                   float _yValTop,
				                   float _yValButtom,
				                   const float* _table,
				                   bool _displayOutside);
			public:
				/* ****************************************************
				 *    == operator
				 *****************************************************/
				bool operator== (const Shaper& _obj) const {
					return _obj.m_uri == m_uri;
				}
				bool operator!= (const Shaper& _obj) const {
					return _obj.m_uri != m_uri;
				}
		};
	}
}


