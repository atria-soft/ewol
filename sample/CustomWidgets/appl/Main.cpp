/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license BSD 3 clauses (see license file)
 */


#include <etk/types.hpp>
#include <ewol/ewol.hpp>
#include <gale/context/commandLine.hpp>

#include <appl/debug.hpp>
#include <appl/Windows.hpp>
#include <ewol/object/Object.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/context/Context.hpp>
#include <appl/widget/VectorDisplay.hpp>

namespace appl {
	class MainApplication : public ewol::context::Application {
		public:
			void onCreate(ewol::Context& _context) override {
				APPL_INFO(" == > CREATE ... " << PROJECT_NAME << "  v" << APPL_VERSION << " (START) [" << gale::getBoardType() << "] (" << gale::getCompilationMode() << ") (BEGIN)");
				for( int32_t iii=0 ; iii<_context.getCmd().size(); iii++) {
					etk::String tmpppp = _context.getCmd().get(iii);
					if (    tmpppp == "-h"
					     || tmpppp == "--help") {
						APPL_INFO("  -h/--help display this help" );
						exit(0);
					}
				}
				// TODO : Remove this : Move if in the windows properties
				_context.setSize(vec2(800, 600));
				
				// select internal data for font ...
				_context.getFontDefault().setUseExternal(true);
				_context.getFontDefault().set("FreeSerif;DejaVuSansMono", 19);
				
				appl::widget::VectorDisplay::createManagerWidget(_context.getWidgetManager());
				
				ewol::widget::WindowsShared basicWindows = appl::Windows::create();
				// create the specific windows
				_context.setWindows(basicWindows);
				APPL_INFO("==> CREATE ... " PROJECT_NAME " (END)");
			}
			
			void onStart(ewol::Context& _context) override {
				APPL_INFO("==> START ... " PROJECT_NAME " (BEGIN)");
				// nothing to do ...
				APPL_INFO("==> START ... " PROJECT_NAME " (END)");
			}
			void onStop(ewol::Context& _context) override {
				APPL_INFO("==> STOP ... " PROJECT_NAME " (START)");
				// nothing to do ...
				APPL_INFO("==> STOP ... " PROJECT_NAME " (END)");
			}
	};
}

/**
 * @brief Main of the program (This can be set in every case, but it is not used in Andoid...).
 * @param std IO
 * @return std IO
 */
int main(int _argc, const char *_argv[]) {
	// second possibility
	return ewol::run(ETK_NEW(appl::MainApplication), _argc, _argv);
}