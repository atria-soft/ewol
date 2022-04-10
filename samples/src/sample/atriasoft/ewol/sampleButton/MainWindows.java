package sample.atriasoft.ewol.sampleButton;

import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.ewol.widget.Button;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {
	
	public MainWindows() {
		setPropertyTitle("Simple Button test");
		final Button simpleButton = Button
				.createLabelButton("1 - My <font color=\"red\">button <i>internal</i></font> <br/>2 - <b>label</b><br/>3 -  an other text ...<br/>4 - and an other line to be sure ...");
		simpleButton.setPropertyExpand(Vector3b.TRUE);
		simpleButton.setPropertyFill(Vector3b.FALSE);
		this.setTestWidget(simpleButton);
	}
}
