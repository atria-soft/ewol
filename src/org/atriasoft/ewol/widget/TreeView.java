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
		using TreeViewShared = ememory::SharedPtr<ewol::widget::TreeView>;
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
				void init() override;
			public:
				virtual ~TreeView();
			protected:
				/**
				 * @brief Calculate an element size to extimate the render size.
				 * @note Does not generate the with the same size.
				 * @param[in] _pos Position of colomn and Raw of the element.
				 * @return The estimate size of the element.
				 */
				vec2 calculateElementSize(const ivec2& _pos) override;
				/**
				 * @brief Draw an element in the specific size and position.
				 * @param[in] _pos Position of colomn and Raw of the element.
				 * @param[in] _start Start display position.
				 * @param[in] _size Render raw size
				 * @return The estimate size of the element.
				 */
				void drawElement(const ivec2& _pos, const vec2& _start, const vec2& _size) override;
			protected:
				virtual void onChangePropertyOffsetTreeView();
				virtual void onChangePropertyTextDecorated();
				
				bool onItemEvent(const ewol::event::Input& _event, const ivec2& _pos, const vec2& _mousePosition) override;
				virtual void onItemExpandEvent(const ivec2& _pos) { };
		};
	};
};

