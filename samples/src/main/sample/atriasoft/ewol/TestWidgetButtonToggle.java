package sample.atriasoft.ewol;

import org.atriasoft.ewol.widget.Composer;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetButtonToggle implements TestWidgetInterface {

	@Override
	public Widget getWidget() {
		return Composer.composerGenerateString("""
					<Button name='My name is Bob' toggle='true' fill='true,false' expand='true'>
					<Label>hello, how are you</Label>
					<Label>You <br/>Click - Me <b>!?<!--kjlkjlkjlkj-->d</b></Label>
				</Button>
				""");
	}

	@Override
	public String getTitle() {
		return "Simple Button toggle";
	}
}
