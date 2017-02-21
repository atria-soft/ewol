/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Container.hpp>

namespace ewol {
	namespace widget {
		class Composer;
		using ComposerShared = ememory::SharedPtr<ewol::widget::Composer>;
		using ComposerWeak = ememory::WeakPtr<ewol::widget::Composer>;
		/**
		 * @ingroup ewolWidgetGroup
		 * @brief the composer widget is a widget that create a link on a string.file to parse the data and generate some widget tree
		 */
		class Composer : public ewol::widget::Container {
			public:
				eproperty::Value<bool> propertyRemoveIfUnderRemove; //!< Remove the composer if sub element request a remove
				eproperty::Value<std::string> propertySubFile; //!< If loading a sub-file, we must do it here ==> permit to con,figure it in the xml and not have wrong display
			protected:
				/**
				 * @brief Constructor
				 */
				Composer();
			public:
				DECLARE_WIDGET_FACTORY(Composer, "Composer");
				/**
				 * @brief Destructor
				 */
				virtual ~Composer();
				/**
				 * @brief load a composition with a file
				 * @param[in] _fileName Name of the file
				 * @param[in] _id Unique ID that is used in replacing the balise "{ID}" inside the File (do nothing if == 0)
				 * @return true  == > all done OK
				 * @return false  == > some error occured
				 */
				bool loadFromFile(const std::string& _fileName, uint64_t _id=0);
				/**
				 * @brief load a composition with a file
				 * @param[in] _composerXmlString xml to parse directly
				 * @param[in] _id Unique ID that is used in replacing the balise "{ID}" inside the String (do nothing if == 0)
				 * @return true  == > all done OK
				 * @return false  == > some error occured
				 */
				bool loadFromString(const std::string& _composerXmlString, uint64_t _id=0);
			private:
				void requestDestroyFromChild(const ewol::ObjectShared& _child) override;
			public:
				bool loadXML(const exml::Element& _node) override;
			protected:
				virtual void onChangePropertySubFile();
		};
		ewol::WidgetShared composerGenerateString(const std::string& _data = "", uint64_t _id=0);
		ewol::WidgetShared composerGenerateFile(const std::string& _data = "", uint64_t _id=0);
	};
};
