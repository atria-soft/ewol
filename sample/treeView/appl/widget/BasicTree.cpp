/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include "BasicTree.hpp"
#include <etk/tool.hpp>
#include <test-debug/debug.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(appl::widget::BasicTree);

appl::widget::BasicTree::BasicTree() {
	addObjectType("appl::widget::BasicTree");
}

void appl::widget::BasicTree::init() {
	ewol::widget::TreeView::init();
	setMouseLimit(1);
	m_tree = etk::TreeNode<TreeElement>::create(TreeElement("root", false, true));
	for (size_t iii=0; iii<10; ++iii) {
		auto elem_iii = etk::TreeNode<TreeElement>::create(TreeElement("elem_" + etk::toString(iii), false, false));
		m_tree->addChild(elem_iii);
		for (size_t jjj=0; jjj<iii; ++jjj) {
			if (jjj%3 == 0) {
				auto elem_iii_jjj = etk::TreeNode<TreeElement>::create(TreeElement("elem_" + etk::toString(iii) + "____" + etk::toString(jjj) + "<br/> an other line ...", false, false));
				elem_iii->addChild(elem_iii_jjj);
			} else {
				auto elem_iii_jjj = etk::TreeNode<TreeElement>::create(TreeElement("elem_" + etk::toString(iii) + "____" + etk::toString(jjj), false, false));
				elem_iii->addChild(elem_iii_jjj);
			}
		}
	}
	updateFlatTree();
}


void appl::widget::BasicTree::updateFlatTree() {
	m_flatTree.setRoot(m_tree,
	    [&](const TreeElement& _value){
	    	return true;
	    },
	    [&](const TreeElement& _value){
	    	return _value.m_isExpand;
	    });
	markToRedraw();
}


appl::widget::BasicTree::~BasicTree() {
	
}

etk::Color<> appl::widget::BasicTree::getBasicBG() {
	return etk::Color<>(0xAF,0xAF,0xAF,0xFF);
}


ivec2 appl::widget::BasicTree::getMatrixSize() const {
	return ivec2(4, m_flatTree.size());
}

fluorine::Variant appl::widget::BasicTree::getData(int32_t _role, const ivec2& _pos) {
	auto elem = m_flatTree[_pos.y()];
	TreeElement& value = elem->getData();
	switch (_role) {
		case ewol::widget::ListRole::Text:
			if (_pos.x() == 0) {
				return value.m_display;
			}
			if (_pos.x() == 1) {
				//return etk::toString(countToRoot);
			}
			return "value: " + etk::toString(_pos);
		case ewol::widget::ListRole::DistanceToRoot:
			return uint_t(elem->countToRoot());
		case ewol::widget::ListRole::HaveChild:
			return elem->haveChild();
		case ewol::widget::ListRole::IsExpand:
			return value.m_isExpand;
		case ewol::widget::ListRole::Icon:
			if (elem->countToRoot() == 0) {
				return "THEME_GUI:///Home.svg?lib=ewol";
			}
			if (elem->countToRoot() == 1) {
				return "THEME_GUI:///Folder.svg?lib=ewol";
			}
			if (elem->countToRoot() == 2) {
				return "THEME_GUI:///File.svg?lib=ewol";
			}
			return "";
		case ewol::widget::ListRole::FgColor:
			return etk::Color<>(0,0,0,0xFF);
		case ewol::widget::ListRole::BgColor:
			if ((_pos.y()%2) == 0) {
				return etk::Color<>(0,0,0,0x15);
			}
			return fluorine::Variant();
	}
	return fluorine::Variant();
}

void appl::widget::BasicTree::onItemExpandEvent(const ivec2& _pos) {
	TEST_WARNING("Event on expand on " << _pos);
	m_flatTree[_pos.y()]->getData().m_isExpand = m_flatTree[_pos.y()]->getData().m_isExpand?false:true;
	updateFlatTree();
}
