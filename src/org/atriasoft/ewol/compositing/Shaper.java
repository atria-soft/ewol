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
				etk::Uri this.uri; //!< Name of the configuration of the shaper.
				// External theme config:
				ememory::Ptr<ewol::resource::ConfigFile> this.config; //!< pointer on the config file resources
				int this.confIdPaddingOut[shaperPosCount]; //!< Padding out property : X-left X-right Y-top Y-buttom
				int this.confIdBorder[shaperPosCount]; //!< border property : X-left X-right Y-top Y-buttom
				int this.confIdPaddingIn[shaperPosCount]; //!< Padding in property : X-left X-right Y-top Y-buttom
				int this.confIdMode; //!< Display mode
				int this.confIdDisplayOutside; //!< Display outside of the shape...
				int this.confIdChangeTime;    //!< ConfigFile padding transition time property
				int this.confProgramFile;     //!< ConfigFile opengGl program Name
				int this.confColorFile;       //!< ConfigFile opengGl color file Name
				int this.confImageFile;       //!< ConfigFile opengGl program Name
				// openGL shaders programs:
				ememory::Ptr<gale::resource::Program> this.GLprogram; //!< pointer on the opengl display program
				int this.GLPosition;           //!< openGL id on the element (vertex buffer)
				int this.GLMatrix;             //!< openGL id on the element (transformation matrix)
				int this.GLPropertyPos;       //!< openGL id on the element (simple ratio position in the widget : ____/-----\_____ on Vector2f(X,Y))
				int this.GLStateActivate;      //!< openGL id on the element (activate state displayed)
				int this.GLStateOld;           //!< openGL id on the element (old state displayed)
				int this.GLStateNew;           //!< openGL id on the element (new state displayed)
				int this.GLStateTransition;    //!< openGL id on the element (transition ofset [0.0..1.0] )
				int this.GLtexID;              //!< openGL id on the element (texture image)
				// For the Image :
				ememory::Ptr<ewol::resource::TextureFile> this.resourceTexture; //!< texture resources (for the image)
				// internal needed data :
				int this.nextStatusRequested;    //!< when status is changing, this represent the next step of it
				Vector2f    this.propertyOrigin;         //!< widget origin
				Vector2f    this.propertySize;           //!< widget size
				Vector2f    this.propertyInsidePosition; //!< internal subwidget position
				Vector2f    this.propertyInsideSize;     //!< internal subwidget size
				int this.stateActivate;          //!< Activate state of the element
				int this.stateOld;               //!< previous state
				int this.stateNew;               //!< destination state
				float   this.stateTransition;        //!< working state between 2 states
				int this.nbVertexToDisplay;
				// color management theme:
				ememory::Ptr<ewol::resource::ColorFile> this.colorProperty; //!< input resource for color management
				List<Vector2i> this.listAssiciatedId; //!< Corellation ID between ColorProperty (Y) and OpenGL Program (X)
			protected:
				static  int this.vboIdCoord;
				static  int this.vboIdPos;
				ememory::Ptr<gale::resource::VirtualBufferObject> this.VBO;
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
				 * @brief generic ructor
				 * @param[in] _uri URI of the file that might be loaded
				 */
				Shaper( etk::Uri _uri="");
				/**
				 * @brief generic destructor
				 */
				 ~Shaper();
			public:
				/**
				 * @brief draw All the refistered text in the current element on openGL
				 */
				void draw(boolean _disableDepthTest=true);
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
				boolean setState(int _newState);
				/**
				 * @brief change the current status in an other
				 * @param[in] _newStatusId the next new status requested
				 * @return true The widget must call this fuction periodicly (and redraw itself)
				 * @return false No need to request the periodic call.
				 */
				boolean changeStatusIn(int _newStatusId);
				/**
				 * @brief get the current displayed status of the shaper
				 * @return The Status Id
				 */
				int getCurrentDisplayedStatus() {
					return this.stateNew;
				};
				/**
				 * @brief get the next displayed status of the shaper
				 * @return The next status Id (-1 if no status in next)
				 */
				int getNextDisplayedStatus() {
					return this.nextStatusRequested;
				};
				/**
				 * @brief get the current trasion status
				 * @return value of the transition status (0.0f when no activity)
				 */
				float getTransitionStatus() {
					return this.stateTransition;
				};
				/**
				 * @brief Same as the widfget periodic call (this is for change display)
				 * @param[in] _event The current time of the call.
				 * @return true The widget must call this fuction periodicly (and redraw itself)
				 * @return false No need to request the periodic call.
				 */
				boolean periodicCall( ewol::event::Time _event);
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
				void setSource( etk::Uri _uri);
				/**
				 * @brief get the shaper file Source
				 * @return the shapper file name
				 */
				 etk::Uri getSource()  {
					return this.uri;
				};
				/**
				 * @brief Sometimes the user declare an image but not allocate the ressources all the time, this is to know it ..
				 * @return the validity od the resources.
				 */
				boolean hasSources();
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
				void setShape( Vector2f _origin,  Vector2f _size,  Vector2f _insidePos,  Vector2f _insideSize);
				// @previous
				void setShape( Vector2f _origin,  Vector2f _size) {
					ewol::Padding tmp = getPadding();
					setShape(_origin, _size, _origin+Vector2f(tmp.xLeft(), tmp.yButtom()), _size - Vector2f(tmp.x(), tmp.y()));
				}
			public:
				/**
				 * @brief Get an ID on the color instance element
				 * @param[in] _name Name of the element requested
				 * @return The Id of the color
				 */
				int requestColor( String _name);
				/**
				 * @brief Get The color associated at an ID.
				 * @param[in] _id Id of the color
				 * @return the reference on the color
				 */
				 etk::Color<float> getColor(int _id);
			public:
				/**
				 * @brief Get an ID on the configuration instance element
				 * @param[in] _name Name of the element requested
				 * @return The Id of the element
				 */
				int requestConfig( String _name);
				/**
				 * @brief Get The number associated at an ID.
				 * @param[in] _id Id of the parameter
				 * @return the requested number.
				 */
				double getConfigNumber(int _id);
			public:
				/**
				 * @brief Set activate state of the element
				 * @param[in] _status New activate status
				 */
				void setActivateState(int _status) {
					this.stateActivate = _status;
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
				                    float* _table,
				                   boolean _displayOutside);
			public:
				/* ****************************************************
				 *    == operator
				 *****************************************************/
				boolean operator== ( Shaper _obj)  {
					return _obj.this.uri == this.uri;
				}
				boolean operator!= ( Shaper _obj)  {
					return _obj.this.uri != this.uri;
				}
		};
	}
}


