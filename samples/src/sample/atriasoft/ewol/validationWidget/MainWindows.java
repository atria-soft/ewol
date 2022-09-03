package sample.atriasoft.ewol.validationWidget;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.ewol.widget.Composer;
import org.atriasoft.ewol.widget.Widget;

import sample.atriasoft.ewol.BasicWindows;

public class MainWindows extends BasicWindows {
	
	private int index = -1;
	private final List<String> values = new ArrayList<>();
	private final List<String> titles = new ArrayList<>();
	
	public MainWindows() {
		setPropertyTitle("Test all compositing");
		
		this.titles.add("test Entry");
		this.values.add("""
				<Entry name='My name is Bob' fill='true,false,false' expand='true'/>
				""");
		
		this.titles.add("test button");
		this.values.add("""
				<Button name='My name button' fill='true,false,false' expand='true'>
					<Label>hello, how are you</Label>
				</Button>
				""");
		
		this.titles.add("test button toogle");
		this.values.add("""
				<Button name='My name is Bob' toggle='true' fill='true,false,false' expand='true'>
					<Label>hello, how are you</Label>
					<Label>You <br/>Click - Me <b>!?<!--kjlkjlkjlkj-->d</b></Label>
				</Button>
				""");
		this.titles.add("test checkBox");
		this.values.add("""
				<CheckBox
					name='My name button'
					fill='true,false,false'
					expand='true'
					label='<b>Hello, how Are</b> You?<br/>second-life?'>
				</CheckBox>
				""");
		// set first item
		requestNext();
	}
	
	@Override
	public void requestNext() {
		System.out.print("Request change !!!!");
		this.index++;
		setPropertyTitle(this.titles.get(this.index));
		
		final String dataString = this.values.get(this.index);
		final Widget data = Composer.composerGenerateString(dataString);
		this.setTestWidget(data);
	}
}
