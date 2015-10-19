/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license GPL v3 (see license file)
 */


#include <etk/types.h>
#include <ewol/ewol.h>
#include <gale/context/commandLine.h>

#include <appl/debug.h>
#include <appl/Windows.h>
#include <ewol/object/Object.h>
#include <ewol/widget/Manager.h>
#include <ewol/context/Context.h>


class MainApplication : public ewol::context::Application {
	public:
		bool init(ewol::Context& _context, size_t _initId) {
			APPL_INFO("==> Init APPL (START) [" << ewol::getBoardType() << "] (" << ewol::getCompilationMode() << ")");
			
			// TODO : Remove this : Move if in the windows properties
			_context.setSize(vec2(800, 600));
			
			// select internal data for font ...
			_context.getFontDefault().setUseExternal(true);
			_context.getFontDefault().set("FreeSerif;DejaVuSansMono", 19);
			
			std::shared_ptr<ewol::widget::Windows> basicWindows = appl::Windows::create();
			// create the specific windows
			_context.setWindows(basicWindows);
			APPL_INFO("==> Init APPL (END)");
			return true;
		}
		
		void unInit(ewol::Context& _context) {
			APPL_INFO(" == > Un-Init " PROJECT_NAME " (START)");
			// nothing to do ...
			APPL_INFO(" == > Un-Init " PROJECT_NAME " (END)");
		}
};



/**
 * @brief Main of the program (This can be set in every case, but it is not used in Andoid...).
 * @param std IO
 * @return std IO
 */
int main(int _argc, const char *_argv[]) {
	// second possibility
	return ewol::run(new MainApplication(), _argc, _argv);
}