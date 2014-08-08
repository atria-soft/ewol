/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_IMAGE_H__
#define __EWOL_IMAGE_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <draw/Color.h>
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Image.h>
#include <ewol/resource/ColorFile.h>
#include <ewol/widget/Manager.h>

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
				static const char * const configDistanceField;
				static const char * const configPartStart;
				static const char * const configPartStop;
			protected:
				ewol::compositing::Image m_compositing; //!< compositing element of the image.
				std::shared_ptr<ewol::resource::ColorFile> m_colorProperty; //!< theme color property
				int32_t m_colorId; //!< Color of the image.
			public:
				/**
				 * @brief 
				 */
				Image();
				void init(const std::string& _file="",
				          const ewol::Dimension& _border=ewol::Dimension(vec2(0,0),ewol::Dimension::Millimeter));
			public:
				DECLARE_WIDGET_FACTORY(Image, "Image");
				/**
				 * @brief 
				 */
				virtual ~Image();
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
				const std::string& getFile() const {
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
				const ewol::Dimension& getBorder() const {
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
				const ewol::Dimension& getImageSize() const {
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
				bool getKeepRatio() const {
					return m_keepRatio;
				};
			protected:
				vec2 m_posStart; //!< position in the image to start the sisplay (when we want not to display all the image)
			public:
				/**
				 * @brief set the current 'start' position in the image to display.
				 * @param[in] _keep The new position in the image vec2([0..1],[0..1])
				 */
				void setStartPos(const vec2& _pos);
				/**
				 * @brief get the current 'start' position in the image to display.
				 * @return The Position of the display in image
				 */
				vec2 getStartPos() const {
					return m_posStart;
				};
			protected:
				vec2 m_posStop; //!< position in the image to start the sisplay (when we want not to display all the image)
			public:
				/**
				 * @brief set the current 'stop' position in the image to display.
				 * @param[in] _keep The new position in the image vec2([0..1],[0..1])
				 */
				void setStopPos(const vec2& _pos);
				/**
				 * @brief get the current 'stop' position in the image to display.
				 * @return The Position of the display in image
				 */
				vec2 getStopPos() const {
					return m_posStop;
				};
			public:
				/**
				 * @brief Set distance field rendering mode
				 * @param[in] _df enable distance field
				 */
				void setDistanceField(bool _df) {
					m_compositing.setDistanceFieldMode(_df);
				}
				/**
				 * @brief Get the distance field render mode.
				 * @return Status of the distance field render mode.
				 */
				bool getDistanceField() const {
					return m_compositing.getDistanceFieldMode();
				}
			protected: // Derived function
				virtual void onDraw();
				virtual bool onSetConfig(const ewol::object::Config& _conf);
				virtual bool onGetConfig(const char* _config, std::string& _result) const;
			public: // Derived function
				virtual void calculateMinMaxSize();
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
				virtual bool loadXML(exml::Element* _node);
		};
	};
};


#endif
