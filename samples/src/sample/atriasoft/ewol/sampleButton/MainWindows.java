package sample.atriasoft.ewol.sampleButton;

import org.atriasoft.ewol.widget.Composer;
import org.atriasoft.ewol.widget.Widget;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {
	
	public MainWindows() {
		setPropertyTitle("Simple Button test");
		//final Widget data = Composer.composerGenerateString("<Composer><Label>hello, how are you</Label></Composer>");
		//final Widget data = Composer.composerGenerateString("<Composer><Button><Label gravity=\"center\">hello, how are you</Label></Button></Composer>");
		final Widget data = Composer.composerGenerateString("<Composer><Button toggle='true' fill='true,false,false' expand='true'>" + "<Label>hello, how are you</Label>"
				+ "<Label>You <br/>Click - Me <b>!!!</b></Label>" + "</Button></Composer>");
		this.setTestWidget(data);
		/*
		final Button simpleButton = Button
				.createLabelButton("1 - My <font color=\"red\">button <i>internal</i></font> <br/>2 - <b>label</b><br/>3 -  an other text ...<br/>4 - and an other line to be sure ...");
		simpleButton.setPropertyExpand(Vector3b.TRUE);
		simpleButton.setPropertyFill(Vector3b.FALSE);
		this.setTestWidget(simpleButton);
		*/
		
	}
}
