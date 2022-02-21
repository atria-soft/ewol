package sample.atriasoft.ewol.sampleButton;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.Button;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {
	
	public MainWindows() {
		setPropertyTitle("Simple Button test");
		
		Button simpleButton = new Button();
		simpleButton.setPropertyValue("Top Button");
		simpleButton.setPropertyExpand(Vector2b.TRUE_TRUE);
		simpleButton.setPropertyFill(Vector2b.TRUE_TRUE);
		this.setTestWidget(simpleButton);
	}
}
