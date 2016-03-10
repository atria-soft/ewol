/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Image.h>
#include <ewol/resource/ColorFile.h>
#include <ewol/widget/Manager.h>
#include <esignal/ISignal.h>

namespace ewol {
	namespace widget {
		class Image;
		using ImageShared = ememory::SharedPtr<ewol::widget::Image>;
		using ImageWeak = ememory::WeakPtr<ewol::widget::Image>;
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Image :public ewol::Widget {
			public: // signals
				esignal::ISignal<> signalPressed;
			public: // properties
				eproperty::Value<std::string> propertySource; //!< file name of the image.
				eproperty::Value<gale::Dimension> propertyBorder; //!< border to add at the image.
				eproperty::Value<gale::Dimension> propertyImageSize; //!< border to add at the image.
				eproperty::Value<bool> propertyKeepRatio; //!< keep the image ratio between width and hight
				eproperty::Range<vec2> propertyPosStart; //!< position in the image to start the sisplay (when we want not to display all the image)
				eproperty::Range<vec2> propertyPosStop; //!< position in the image to start the sisplay (when we want not to display all the image)
				eproperty::Value<bool> propertyDistanceFieldMode; //!< to have a parameter
				eproperty::Value<bool> propertySmooth; //!< display is done in the pixed approximation if false
			protected:
				ewol::compositing::Image m_compositing; //!< compositing element of the image.
				std::shared_ptr<ewol::resource::ColorFile> m_colorProperty; //!< theme color property
				int32_t m_colorId; //!< Color of the image.
			public:
				/**
				 * @brief 
				 */
				Image();
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
				void set(const std::string& _file, const gale::Dimension& _border);
			protected:
				vec2 m_imageRenderSize; //!< size of the image when we render it
			protected: // Derived function
				virtual void onDraw();
			public: // Derived function
				virtual void calculateMinMaxSize();
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
				virtual bool loadXML(const std::shared_ptr<const exml::Element>& _node);
			protected:
				virtual void onChangePropertySource();
				virtual void onChangePropertyImageSize();
				virtual void onChangePropertyGlobalSize();
				virtual void onChangePropertySmooth();
				virtual void onChangePropertyDistanceFieldMode();
		};
	};
};
