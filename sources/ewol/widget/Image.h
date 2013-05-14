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

extern const char * const ewolEventImagePressed;

namespace widget {
	class Image :public ewol::Widget
	{
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
			static void Init(void);
			/**
			 * @brief Main call to unrecord the widget from the list of "widget named creator"
			 */
			static void UnInit(void);
		protected:
			ewol::Image m_compositing; //!< compositing element of the image.
		public:
			/**
			 * @brief 
			 */
			Image(const etk::UString& _file="",
			      const ewol::Dimension& _border=ewol::Dimension(vec2(0,0),ewol::Dimension::Millimeter));
			/**
			 * @brief 
			 */
			virtual ~Image(void) { };
			/**
			 * @brief Set All the configuration of the current image
			 * @param[in] _file Filaneme of the new image
			 * @param[in] _border New border size to set
			 */
			void Set(const etk::UString& _file, const ewol::Dimension& _border);
		protected:
			etk::UString m_fileName; //!< File name of the image.
		public:
			/**
			 * @brief Set the new filename
			 * @param[in] _file Filaneme of the new image
			 */
			void SetFile(const etk::UString& _file);
			/**
			 * @brief Get the file displayed
			 * @return the filename of the image
			 */
			const etk::UString& GetFile(void) const { return m_fileName; };
		protected:
			ewol::Dimension m_border; //!< border to add at the image.
		public:
			/**
			 * @brief Set tge Border size around the image
			 * @param[in] _border New border size to set
			 */
			void SetBorder(const ewol::Dimension& _border);
			/**
			 * @brief Get the current border request at the image
			 * @return the border size
			 */
			const ewol::Dimension& GetBorder(void) const { return m_border; };
		protected:
			ewol::Dimension m_imageSize; //!< border to add at the image.
		public:
			/**
			 * @brief Set tge Border size around the image
			 * @param[in] _size New border size to set
			 */
			void SetImageSize(const ewol::Dimension& _size);
			/**
			 * @brief Get the current border request at the image
			 * @return the border size
			 */
			const ewol::Dimension& GetImageSize(void) const { return m_imageSize; };
		protected:
			bool m_keepRatio; //!< Keep the image ratio between width and hight
		public:
			/**
			 * @brief Set the current status of keeping ratio.
			 * @param[in] _keep The new status of keeping the ratio of this image.
			 */
			void SetKeepRatio(bool _keep);
			/**
			 * @brief Get the current status of keeping ratio.
			 * @return The status of keeping the ratio of this image.
			 */
			bool GetKeepRatio(void) const { return m_keepRatio; };
		protected: // Derived function
			virtual void OnDraw(void);
			virtual bool OnSetConfig(const ewol::EConfig& _conf);
			virtual bool OnGetConfig(const char* _config, etk::UString& _result) const;
		public: // Derived function
			virtual const char * const GetObjectType(void) { return "Ewol::Image"; };
			virtual void CalculateMinMaxSize(void);
			virtual void OnRegenerateDisplay(void);
			virtual bool OnEventInput(const ewol::EventInput& _event);
			virtual bool LoadXML(TiXmlNode* _node);
	};
};


#endif
