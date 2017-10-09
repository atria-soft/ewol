/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/compositing/Image.hpp>
#include <ewol/resource/ColorFile.hpp>
#include <ewol/widget/Manager.hpp>
#include <esignal/Signal.hpp>

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
				esignal::Signal<> signalPressed;
			public: // properties
				eproperty::Value<etk::String> propertySource; //!< file name of the image.
				eproperty::Value<gale::Dimension> propertyBorder; //!< border to add at the image.
				eproperty::Value<gale::Dimension> propertyImageSize; //!< border to add at the image.
				eproperty::Value<bool> propertyKeepRatio; //!< keep the image ratio between width and hight
				eproperty::Range<vec2> propertyPosStart; //!< position in the image to start the sisplay (when we want not to display all the image)
				eproperty::Range<vec2> propertyPosStop; //!< position in the image to start the sisplay (when we want not to display all the image)
				eproperty::Value<bool> propertyDistanceFieldMode; //!< to have a parameter
				eproperty::Value<bool> propertySmooth; //!< display is done in the pixed approximation if false
				eproperty::Value<bool> propertyUseThemeColor; //!< Use the themo color management ("{ewol}THEME:COLOR:Image.json") default false
			protected:
				ewol::compositing::Image m_compositing; //!< compositing element of the image.
				ememory::SharedPtr<ewol::resource::ColorFile> m_colorProperty; //!< theme color property
				int32_t m_colorId; //!< Color of the image.
			public:
				/**
				 * @brief 
				 */
				Image();
				void init() override;
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
				void set(const etk::String& _file, const gale::Dimension& _border);
				/**
				 * @brief Set an image with direct elements
				 * @param[in] _image Image to set in the display
				 */
				void setCustumSource(const egami::Image& _image);
			protected:
				vec2 m_imageRenderSize; //!< size of the image when we render it
			protected:
				void onDraw() override;
			public:
				void calculateMinMaxSize() override;
				void onRegenerateDisplay() override;
				bool onEventInput(const ewol::event::Input& _event) override;
				bool loadXML(const exml::Element& _node) override;
			protected:
				virtual void onChangePropertySource();
				virtual void onChangePropertyImageSize();
				virtual void onChangePropertyGlobalSize();
				virtual void onChangePropertySmooth();
				virtual void onChangePropertyDistanceFieldMode();
				virtual void onChangePropertyUseThemeColor();
		};
	};
};
