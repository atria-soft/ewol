/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/Color.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/List.hpp>
#include <ewol/compositing/Compositing.hpp>
#include <fluorine/Variant.hpp>

namespace ewol {
	namespace widget {
		class TreeView;
		using TreeView = ememory::Ptr<ewol::widget::TreeView>;
		using TreeViewWeak = ememory::WeakPtr<ewol::widget::TreeView>;
		
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class TreeView : public ewol::widget::List {
			public: // signals
			public: // properties
				eproperty::Value<float> propertyOffsetTreeView; //!< indentation betwwen every new element.
				eproperty::Value<float> propertyIconTreeViewSize; //!< Size of the icon.
				eproperty::Value<bool> propertyTextIsDecorated; //!< Size of the icon.
			protected:
				TreeView();
				void init() ;
			public:
				 ~TreeView();
			protected:
				/**
				 * Calculate an element size to extimate the render size.
				 * @note Does not generate the with the same size.
				 * @param _pos Position of colomn and Raw of the element.
				 * @return The estimate size of the element.
				 */
				Vector2f calculateElementSize( Vector2i _pos) ;
				/**
				 * Draw an element in the specific size and position.
				 * @param _pos Position of colomn and Raw of the element.
				 * @param _start Start display position.
				 * @param _size Render raw size
				 * @return The estimate size of the element.
				 */
				void drawElement( Vector2i _pos,  Vector2f _start,  Vector2f _size) ;
			protected:
				 void onChangePropertyOffsetTreeView();
				 void onChangePropertyTextDecorated();
				
				boolean onItemEvent( ewol::event::Input _event,  Vector2i _pos,  Vector2f _mousePosition) ;
				 void onItemExpandEvent( Vector2i _pos) { };
		};
	};
};

