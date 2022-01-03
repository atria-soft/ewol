package sample.atriasoft.ewol.sampleButton;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.Button;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {
	
	public MainWindows() {
		setPropertyTitle("Simple Button test");
		
		Button simpleButton = new Button();
		simpleButton.setPropertyValue("Top Button");
		simpleButton.setPropertyExpand(new Vector2b(true, true));
		simpleButton.setPropertyFill(new Vector2b(true, false));
		this.setTestWidget(simpleButton);
	}
}
