/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <appl/debug.h>
#include <appl/TestLabel.h>

#include <ewol/widget/Button.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/List.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/widget/PopUp.h>
#include <ewol/widget/Slider.h>
#include <ewol/widget/Menu.h>
#include <ewol/widget/meta/FileChooser.h>
#include <ewol/widget/meta/Parameter.h>
#include <ewol/widget/Manager.h>

#undef __class__
#define __class__	"TestButton"

appl::TestLabel::TestLabel() {
	addObjectType("appl::TestLabel");
}

void appl::TestLabel::init() {
	ewol::widget::Composer::init();
	
	APPL_INFO("Create " __class__ " (start)");
	
	std::string myDescription = std::string("")
	      + "<sizer mode='vert' fill='true' expand='true'>\n"
	      + "	<sizer mode='hori'>\n"
	      + "		<button name='[TEST]Button:Expand-x' toggle='true'>\n"
	      + "			<label>Expend X <br/> (false)</label>\n"
	      + "			<label>Expend X <br/> <b>(true)</b></label>\n"
	      + "		</button>\n"
	      + "		<button name='[TEST]Button:Expand-y' toggle='true'>\n"
	      + "			<label>Expend Y <br/> (false)</label>\n"
	      + "			<label>Expend Y <br/> <b>(true)</b></label>\n"
	      + "		</button>\n"
	      + "		<button name='[TEST]Button:Toggle' toggle='true'>\n"
	      + "			<label>Toggle <br/> (false)</label>\n"
	      + "			<label>Toggle <br/> <b>(true)</b></label>\n"
	      + "		</button>\n"
	      + "		<button name='[TEST]Button:TextToggle' toggle='true'>\n"
	      + "			<label>Text On toggle state <br/> (false)</label>\n"
	      + "			<label>Text On toggle state <br/> <b>(true)</b></label>\n"
	      + "		</button>\n"
	      + "	</sizer>\n"
	      + "	<sizer mode='hori'>\n"
	      + "		<button name='[TEST]Button:Fill-x' toggle='true'>\n"
	      + "			<label>Fill X <br/> (false)</label>\n"
	      + "			<label>Fill X <br/> <b>(true)</b></label>\n"
	      + "		</button>\n"
	      + "		<button name='[TEST]Button:Fill-y' toggle='true'>\n"
	      + "			<label>Fill Y <br/> (false)</label>\n"
	      + "			<label>Fill Y <br/> <b>(true)</b></label>\n"
	      + "		</button>\n"
	      + "		<button name='[TEST]Button:SetMaxSize' toggle='true'>\n"
	      + "			<label>Max Size <br/> (false)</label>\n"
	      + "			<label>Max Size <br/> <b>(true)</b></label>\n"
	      + "		</button>\n"
	      + "		<button name='[TEST]Button:ChangeText'>\n"
	      + "			<label>Change Text<br/> and other ...</label>\n"
	      + "		</button>\n"
	      + "	</sizer>\n"
	      + "	<sizer mode='hori'>\n"
	      + "		<spacer expand='false,false' fill='true,false' min-size='10,10px' color='#F008'/>\n"
	      + "		<sizer mode='vert'>\n"
	      + "			<spacer expand='false,false' fill='false,true' min-size='10,10px' color='#FF08'/>\n"
	      + "			<label name='[TEST]TO-TEST' expand='false,false' fill='false,false' >\n"
	      + "				Text de base ...\n"
	      + "			</label>\n"
	      + "			<spacer expand='false,false' fill='false,true' min-size='10,10px' color='#0FF8'/>\n"
	      + "		</sizer>\n"
	      + "		<spacer expand='false,false' fill='true,false' min-size='10,10px' color='#0F08'/>\n"
	      + "	</sizer>\n"
	      + "</sizer>";
	loadFromString(myDescription);
	setExpand(bvec2(true,true));
	setFill(bvec2(true,true));
	subBind(ewol::widget::Button, "[TEST]Button:Expand-x", signalValue, shared_from_this(), &appl::TestLabel::onCallbackExpendX);
	subBind(ewol::widget::Button, "[TEST]Button:Expand-y", signalValue, shared_from_this(), &appl::TestLabel::onCallbackExpendY);
	subBind(ewol::widget::Button, "[TEST]Button:Fill-x", signalValue, shared_from_this(), &appl::TestLabel::onCallbackFillX);
	subBind(ewol::widget::Button, "[TEST]Button:Fill-y", signalValue, shared_from_this(), &appl::TestLabel::onCallbackFillY);
	subBind(ewol::widget::Button, "[TEST]Button:SetMaxSize", signalValue, shared_from_this(), &appl::TestLabel::onCallbackMaxSize);
	subBind(ewol::widget::Button, "[TEST]Button:ChangeText", signalPressed, shared_from_this(), &appl::TestLabel::onCallbackChangeLabel);
	m_testWidget = std::dynamic_pointer_cast<ewol::widget::Label>(getSubObjectNamed("[TEST]TO-TEST"));
	if (m_testWidget == nullptr) {
		APPL_CRITICAL("Can not get the pointer of the widget button pointer");
	}
	APPL_INFO("Create " __class__ " (end)");
}

void appl::TestLabel::onCallbackExpendX(const bool& _value) {
	if (m_testWidget == nullptr) {
		return;
	}
	m_testWidget->setExpand(bvec2(_value, m_testWidget->getExpand().y()));
}

void appl::TestLabel::onCallbackExpendY(const bool& _value) {
	if (m_testWidget == nullptr) {
		return;
	}
	m_testWidget->setExpand(bvec2(m_testWidget->getExpand().x(), _value));
}

void appl::TestLabel::onCallbackFillX(const bool& _value) {
	if (m_testWidget == nullptr) {
		return;
	}
	m_testWidget->setFill(bvec2(_value, m_testWidget->getFill().y()));
}

void appl::TestLabel::onCallbackFillY(const bool& _value) {
	if (m_testWidget == nullptr) {
		return;
	}
	m_testWidget->setFill(bvec2(m_testWidget->getFill().x(), _value));
}


void appl::TestLabel::onCallbackMaxSize(const bool& _value) {
	if (m_testWidget == nullptr) {
		return;
	}
	if (_value == true) {
		m_testWidget->setMaxSize(vec2(400,9999999999));
	} else {
		m_testWidget->setNoMaxSize();
	}
}

void appl::TestLabel::onCallbackChangeLabel() {
	if (m_testWidget == nullptr) {
		return;
	}
	m_labelId++;
	switch(m_labelId) {
		default:
			m_labelId=0;
		case 0:
			m_testWidget->setLabel("Basic label");
			break;
		case 1:
			m_testWidget->setLabel("Basic <bold>label</bold>");
			break;
		case 2:
			m_testWidget->setLabel("<center>\n"
			                       "	Un jour Cosette se <b>regarda</b> par hasard dans son miroir et se dit: Tiens! <b>Il lui semblait presque <i>qu'elle à jolie.</i></b> Ceci la jeta dans un trouble singulier. <font color=\"#FF0000\">Jusqu'au moment elle <b>n'avait</b> point <i>song figure.</i></font> Elle se voyait dans son miroir, mais elle ne s'y regardait pas. Et puis, on lui avait souvent dit qu'elle à laide;<br/> Jean Valjean seul disait doucement : <br/> Mais non!<br/>  mais non!<br/>  Quoi qu'il en fut, Cosette toujours crue laide, et avait grandi dans cette id ec la réation facile de l'enfance. Voici que tout d'un coup son miroir lui disait comme Jean Valjean : Mais non! Elle ne dormit pas de la nuit. Si jolie ? pensait-elle, comme cela serait drole que je fusse jolie! Et elle se rappelait celles de ses compagnes dont la beauté sait effet dans le couvent, et elle se disait : Comment! je serais comme mademoiselle une telle!\n"
			                       "</center>\n");
			break;
		case 3:
			m_testWidget->setLabel("<left>\n"
			                       "	Un jour Cosette se <b>regarda</b> par hasard dans son miroir et se dit: Tiens! <b>Il lui semblait presque <i>qu'elle à jolie.</i></b> Ceci la jeta dans un trouble singulier. <font color=\"#0F0\">Jusqu'au moment elle <b>n'avait</b> point <i>song figure.</i></font> Elle se voyait dans son miroir, mais elle ne s'y regardait pas. Et puis, on lui avait souvent dit qu'elle à laide;<br/> Jean Valjean seul disait doucement :<br/>  Mais non!<br/>  mais non!<br/>  Quoi qu'il en fut, Cosette toujours crue laide, et avait grandi dans cette id ec la réation facile de l'enfance. Voici que tout d'un coup son miroir lui disait comme Jean Valjean : Mais non! Elle ne dormit pas de la nuit. Si jolie ? pensait-elle, comme cela serait drole que je fusse jolie! Et elle se rappelait celles de ses compagnes dont la beauté sait effet dans le couvent, et elle se disait : Comment! je serais comme mademoiselle une telle!\n"
			                       "</left>\n");
			break;
		case 4:
			m_testWidget->setLabel("<right>\n"
			                       "	Un jour Cosette se <b>regarda</b> par hasard dans son miroir et se dit: Tiens! <b>Il lui semblait presque <i>qu'elle à jolie.</i></b> Ceci la jeta dans un trouble singulier. <font color=\"#00F\">Jusqu'au moment elle<b> n'avait</b> point <i>song figure.</i></font> Elle se voyait dans son miroir, mais elle ne s'y regardait pas. Et puis, on lui avait souvent dit qu'elle à laide;<br/> Jean Valjean seul disait doucement :<br/>  Mais non! <br/> mais non!<br/>  Quoi qu'il en fut, Cosette toujours crue laide, et avait grandi dans cette id ec la réation facile de l'enfance. Voici que tout d'un coup son miroir lui disait comme Jean Valjean : Mais non! Elle ne dormit pas de la nuit. Si jolie ? pensait-elle, comme cela serait drole que je fusse jolie! Et elle se rappelait celles de ses compagnes dont la beauté sait effet dans le couvent, et elle se disait : Comment! je serais comme mademoiselle une telle!\n"
			                       "</right>\n");
			break;
		case 5:
			m_testWidget->setLabel("<justify>\n"
			                       "	Un jour Cosette se <b>regarda</b> par hasard dans son miroir et se dit: Tiens! <b>Il lui semblait presque <i>qu'elle à jolie.</i></b> Ceci la jeta dans un trouble singulier. <font color=\"#FF0\">Jusqu'au moment elle <b>n'avait</b> point <i>song figure.</i></font> Elle se voyait dans son miroir, mais elle ne s'y regardait pas. Et puis, on lui avait souvent dit qu'elle à laide;<br/> Jean Valjean seul disait doucement :<br/>  Mais non!<br/>  mais non!<br/>  Quoi qu'il en fut, Cosette toujours crue laide, et avait grandi dans cette id ec la réation facile de l'enfance. Voici que tout d'un coup son miroir lui disait comme Jean Valjean : Mais non! Elle ne dormit pas de la nuit. Si jolie ? pensait-elle, comme cela serait drole que je fusse jolie! Et elle se rappelait celles de ses compagnes dont la beauté sait effet dans le couvent, et elle se disait : Comment! je serais comme mademoiselle une telle!\n"
			                       "</justify>\n");
			break;
	}
}

