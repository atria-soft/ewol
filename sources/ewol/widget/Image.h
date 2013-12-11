/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_IMAGE_H__
#define __EWOL_IMAGE_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <draw/Color.h>
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Image.h>
#include <ewol/widget/WidgetManager.h>

extern const char * const ewolEventImagePressed;

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Image :public ewol::Widget {
			public:
				// Event list of properties
				static const char * const eventPressed;
				// Config list of properties
				static const char * const configRatio;
				static const char * const configSize;
				static const char * const configBorder;
				static const char * const configSource;
			public:
				/**
				 * @brief Main call of recording the widget on the List of "widget named creator"
				 */
				static void init(ewol::WidgetManager& _widgetManager);
			protected:
				ewol::Image m_compositing; //!< compositing element of the image.
			public:
				/**
				 * @brief 
				 */
				Image(const std::string& _file="",
				      const ewol::Dimension& _border=ewol::Dimension(vec2(0,0),ewol::Dimension::Millimeter));
				/**
				 * @brief 
				 */
				virtual ~Image(void) { };
				/**
				 * @brief set All the configuration of the current image
				 * @param[in] _file Filaneme of the new image
				 * @param[in] _border New border size to set
				 */
				void set(const std::string& _file, const ewol::Dimension& _border);
			protected:
				std::string m_fileName; //!< file name of the image.
			public:
				/**
				 * @brief set the new filename
				 * @param[in] _file Filaneme of the new image
				 */
				void setFile(const std::string& _file);
				/**
				 * @brief get the file displayed
				 * @return the filename of the image
				 */
				const std::string& getFile(void) const {
					return m_fileName;
				};
			protected:
				ewol::Dimension m_border; //!< border to add at the image.
			public:
				/**
				 * @brief set tge Border size around the image
				 * @param[in] _border New border size to set
				 */
				void setBorder(const ewol::Dimension& _border);
				/**
				 * @brief get the current border request at the image
				 * @return the border size
				 */
				const ewol::Dimension& getBorder(void) const {
					return m_border;
				};
			protected:
				ewol::Dimension m_imageSize; //!< border to add at the image.
			public:
				/**
				 * @brief set tge Border size around the image
				 * @param[in] _size New border size to set
				 */
				void setImageSize(const ewol::Dimension& _size);
				/**
				 * @brief get the current border request at the image
				 * @return the border size
				 */
				const ewol::Dimension& getImageSize(void) const {
					return m_imageSize;
				};
			protected:
				bool m_keepRatio; //!< keep the image ratio between width and hight
			public:
				/**
				 * @brief set the current status of keeping ratio.
				 * @param[in] _keep The new status of keeping the ratio of this image.
				 */
				void setKeepRatio(bool _keep);
				/**
				 * @brief get the current status of keeping ratio.
				 * @return The status of keeping the ratio of this image.
				 */
				bool getKeepRatio(void) const {
					return m_keepRatio;
				};
			protected: // Derived function
				virtual void onDraw(void);
				virtual bool onSetConfig(const ewol::EConfig& _conf);
				virtual bool onGetConfig(const char* _config, std::string& _result) const;
			public: // Derived function
				virtual void calculateMinMaxSize(void);
				virtual void onRegenerateDisplay(void);
				virtual bool onEventInput(const ewol::EventInput& _event);
				virtual bool loadXML(exml::Element* _node);
		};
	};
};


#endif
