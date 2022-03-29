package sample.atriasoft.ewol.sampleButton;

import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.ewol.widget.Button;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {
	
	public MainWindows() {
		setPropertyTitle("Simple Button test");
		
		Button simpleButton = Button.createLabelButton("Top Button");
		simpleButton.setPropertyExpand(Vector3b.TRUE);
		simpleButton.setPropertyFill(Vector3b.TRUE);
		this.setTestWidget(simpleButton);
	}
}
