/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ewol/widget/TreeView.hpp>
#include <etk/os/FSNode.hpp>
#include <ewol/resource/ColorFile.hpp>
#include <esignal/Signal.hpp>
#include <etk/FlatTree.hpp>

namespace appl {
	namespace widget {
		class BasicTree;
		using BasicTreeShared = ememory::SharedPtr<appl::widget::BasicTree>;
		using BasicTreeWeak = ememory::WeakPtr<appl::widget::BasicTree>;
		/**
		 * @brief Generic display folder class. This widget display the content of a single folder :
		 */
		class BasicTree : public ewol::widget::TreeView {
			protected:
				BasicTree();
			public:
				DECLARE_WIDGET_FACTORY(BasicTree, "BasicTree");
				virtual ~BasicTree();
			protected:
				etk::Color<> getBasicBG() override;
				ivec2 getMatrixSize() const override;
				fluorine::Variant getData(int32_t _role, const ivec2& _pos) override;
				bool onItemEvent(int32_t _IdInput, enum gale::key::status _typeEvent, const ivec2& _pos, const vec2& _mousePosition) override;
				
				using NodeElement = etk::TreeNode<etk::String>;
				ememory::SharedPtr<NodeElement> m_tree;
				etk::FlatTree<etk::String> m_flatTree;
		};
	};
};


