/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Container.hpp>
#include <etk/uri/uri.hpp>

namespace ewol {
	namespace widget {
		class Composer;
		using Composer = ememory::Ptr<ewol::widget::Composer>;
		using ComposerWeak = ememory::WeakPtr<ewol::widget::Composer>;
		/**
		 * @ingroup ewolWidgetGroup
		 * the composer widget is a widget that create a link on a string.file to parse the data and generate some widget tree
		 */
		class Composer : public ewol::widget::Container {
			public:
				eproperty::Value<bool> propertyRemoveIfUnderRemove; //!< Remove the composer if sub element request a remove
				eproperty::Value<etk::Uri> propertySubFile; //!< If loading a sub-file, we must do it here ==> permit to configure it in the xml and not have wrong display
			protected:
				/**
				 * Constructor
				 */
				Composer();
			public:
				DECLARE_WIDGET_FACTORY(Composer, "Composer");
				/**
				 * Destructor
				 */
				 ~Composer();
				/**
				 * load a composition with a file
				 * @param _uri Name of the file
				 * @param _id Unique ID that is used in replacing the balise "{ID}" inside the File (do nothing if == 0)
				 * @return true  == > all done OK
				 * @return false  == > some error occured
				 */
				boolean loadFromFile( etk::Uri _uri, ulong _id=0);
				/**
				 * load a composition with a file
				 * @param _composerXmlString xml to parse directly
				 * @param _id Unique ID that is used in replacing the balise "{ID}" inside the String (do nothing if == 0)
				 * @return true  == > all done OK
				 * @return false  == > some error occured
				 */
				boolean loadFromString( String _composerXmlString, ulong _id=0);
			private:
				void requestDestroyFromChild( EwolObject _child) ;
			public:
				boolean loadXML( exml::Element _node) ;
			protected:
				 void onChangePropertySubFile();
		};
		Widget composerGenerateString( String _data = "", ulong _id=0);
		Widget composerGenerateFile( etk::Uri _uri = "", ulong _id=0);
	};
};
