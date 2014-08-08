/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_CONTEXT_APPLICATION_H__
#define __EWOL_CONTEXT_APPLICATION_H__

namespace ewol {
	class Context;
	namespace context {
		class Application {
			protected:
				size_t m_nbStepInit;
			public:
				size_t getNbStepInit() {
					return m_nbStepInit;
				}
			public:
				Application() :
				  m_nbStepInit(1) {};
				virtual ~Application() {};
			public:
				virtual bool init(ewol::Context& _context, size_t _initId) = 0;
				virtual void unInit(ewol::Context& _context) = 0;
		};
	};
};

#endif
