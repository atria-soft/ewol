/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

etk::CCout& ewol::keyEvent::operator <<(etk::CCout &os, const ewol::keyEvent::event_te obj)
{
	switch(obj) {
		default:
		case ewol::keyEvent::None :
			os << "None";
			break;
		case ewol::keyEvent::MouseDown :
			os << "MouseDown";
			break;
		case ewol::keyEvent::MouseMove :
			os << "MouseMove";
			break;
		case ewol::keyEvent::MouseSingle :
			os << "MouseSingle";
			break;
		case ewol::keyEvent::MouseDouble :
			os << "MouseDouble";
			break;
		case ewol::keyEvent::MouseTriple :
			os << "MouseTriple";
			break;
		case ewol::keyEvent::MouseQuad :
			os << "MouseQuad";
			break;
		case ewol::keyEvent::MouseQuinte :
			os << "MouseQuinte";
			break;
		case ewol::keyEvent::MouseUp :
			os << "MouseUp";
			break;
		case ewol::keyEvent::MouseEnter :
			os << "MouseEnter";
			break;
		case ewol::keyEvent::MouseLeave :
			os << "MouseLeave";
			break;
		case ewol::keyEvent::MouseAbort :
			os << "MouseAbort";
			break;
		case ewol::keyEvent::MouseTransfer :
			os << "MouseTransfer";
			break;
		case ewol::keyEvent::KeyboardLeft :
			os << "KeyboardLeft";
			break;
		case ewol::keyEvent::KeyboardRight :
			os << "KeyboardRight";
			break;
		case ewol::keyEvent::KeyboardUp :
			os << "KeyboardUp";
			break;
		case ewol::keyEvent::KeyboardDown :
			os << "KeyboardDown";
			break;
		case ewol::keyEvent::KeyboardPageUp :
			os << "KeyboardPageUp";
			break;
		case ewol::keyEvent::KeyboardPageDown :
			os << "KeyboardPageDown";
			break;
		case ewol::keyEvent::KeyboardStart :
			os << "KeyboardStart";
			break;
		case ewol::keyEvent::KeyboardEnd :
			os << "KeyboardEnd";
			break;
		case ewol::keyEvent::KeyboardCenter :
			os << "KeyboardCenter";
			break;
		case ewol::keyEvent::KeyboardStopDefil :
			os << "KeyboardStopDefil";
			break;
		case ewol::keyEvent::KeyboardWait :
			os << "KeyboardWait";
			break;
		case ewol::keyEvent::KeyboardInsert :
			os << "KeyboardInsert";
			break;
		case ewol::keyEvent::KeyboardF1 :
			os << "KeyboardF1";
			break;
		case ewol::keyEvent::KeyboardF2 :
			os << "KeyboardF2";
			break;
		case ewol::keyEvent::KeyboardF3 :
			os << "KeyboardF3";
			break;
		case ewol::keyEvent::KeyboardF4 :
			os << "KeyboardF4";
			break;
		case ewol::keyEvent::KeyboardF5 :
			os << "KeyboardF5";
			break;
		case ewol::keyEvent::KeyboardF6 :
			os << "KeyboardF6";
			break;
		case ewol::keyEvent::KeyboardF7 :
			os << "KeyboardF7";
			break;
		case ewol::keyEvent::KeyboardF8 :
			os << "KeyboardF8";
			break;
		case ewol::keyEvent::KeyboardF9 :
			os << "KeyboardF9";
			break;
		case ewol::keyEvent::KeyboardF10 :
			os << "KeyboardF10";
			break;
		case ewol::keyEvent::KeyboardF11 :
			os << "KeyboardF11";
			break;
		case ewol::keyEvent::KeyboardF12 :
			os << "KeyboardF12";
			break;
		case ewol::keyEvent::KeyboardCapLock :
			os << "KeyboardCapLock";
			break;
		case ewol::keyEvent::KeyboardShiftLeft :
			os << "KeyboardShiftLeft";
			break;
		case ewol::keyEvent::KeyboardShiftRight :
			os << "KeyboardShiftRight";
			break;
		case ewol::keyEvent::KeyboardCtrlLeft :
			os << "KeyboardCtrlLeft";
			break;
		case ewol::keyEvent::KeyboardCtrlRight :
			os << "KeyboardCtrlRight";
			break;
		case ewol::keyEvent::KeyboardMetaLeft :
			os << "KeyboardMetaLeft";
			break;
		case ewol::keyEvent::KeyboardMetaRight :
			os << "KeyboardMetaRight";
			break;
		case ewol::keyEvent::KeyboardAlt :
			os << "KeyboardAlt";
			break;
		case ewol::keyEvent::KeyboardAltGr :
			os << "KeyboardAltGr";
			break;
		case ewol::keyEvent::KeyboardContextMenu :
			os << "KeyboardContextMenu";
			break;
		case ewol::keyEvent::KeyboardVerNum :
			os << "KeyboardVerNum";
			break;
	}
	return os;
}

}

etk::CCout& ewol::keyEvent::operator <<(etk::CCout &os, const ewol::keyEvent::input_te obj)
{
	switch(obj) {
		default:
		case ewol::keyEvent::NoUnknowne :
			os << "Unknow";
			break;
		case ewol::keyEvent::Mouse :
			os << "Mouse";
			break;
		case ewol::keyEvent::Finger :
			os << "Finger";
			break;
		case ewol::keyEvent::Stylet :
			os << "Stylet";
			break;
	}
	return os;
}



