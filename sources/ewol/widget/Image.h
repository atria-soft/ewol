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
			Image(const etk::UString& file="",
			      const ewol::Dimension& border=ewol::Dimension(vec2(0,0),ewol::Dimension::Millimeter));
			/**
			 * @brief 
			 */
			virtual ~Image(void) { };
			/**
			 * @brief Set All the configuration of the current image
			 * @param[in] file Filaneme of the new image
			 * @param[in] border New border size to set
			 * @param[in] size new size of the display
			 */
			void Set(const etk::UString& file, const ewol::Dimension& border);
		protected:
			etk::UString m_fileName; //!< File name of the image.
		public:
			/**
			 * @brief Set the new filename
			 * @param[in] file Filaneme of the new image
			 */
			void SetFile(const etk::UString& file);
			/**
			 * @brief Get the file displayed
			 * @return the filename of the image
			 */
			const etk::UString& GetFile() { return m_fileName; };
		protected:
			ewol::Dimension m_border; //!< border to add at the image.
		public:
			/**
			 * @brief Set tge Border size around the image
			 * @param[in] border New border size to set
			 */
			void SetBorder(const ewol::Dimension& border);
			/**
			 * @brief Get the current border request at the image
			 * @return the border size
			 */
			const ewol::Dimension& GetBorder() { return m_border; };
		protected:
			ewol::Dimension m_imageSize; //!< border to add at the image.
		public:
			/**
			 * @brief Set tge Border size around the image
			 * @param[in] border New border size to set
			 */
			void SetImageSize(const ewol::Dimension& size);
			/**
			 * @brief Get the current border request at the image
			 * @return the border size
			 */
			const ewol::Dimension& GetImageSize() { return m_imageSize; };
		protected:
			bool m_keepRatio; //!< Keep the image ratio between width and hight
		public:
			/**
			 * @brief Set the current status of keeping ratio.
			 * @param[in] The new status of keeping the ratio of this image.
			 */
			void SetKeepRatio(bool keep);
			/**
			 * @brief Get the current status of keeping ratio.
			 * @return The status of keeping the ratio of this image.
			 */
			bool GetKeepRatio(void) { return m_keepRatio; };
		public:
			// Derived function
			virtual const char * const GetObjectType(void) { return "Ewol::Image"; };
			virtual void CalculateMinMaxSize(void);
			virtual void OnRegenerateDisplay(void);
			virtual void OnDraw(ewol::DrawProperty& displayProp);
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, const vec2& pos);
			virtual bool LoadXML(TiXmlNode* node);
	};
};


#endif
