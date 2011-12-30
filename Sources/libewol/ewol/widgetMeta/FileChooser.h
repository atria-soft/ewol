/**
 *******************************************************************************
 * @file ewol/widgetMeta/FileChooser.h
 * @brief ewol File chooser complex widget system (header)
 * @author Edouard DUPIN
 * @date 29/12/2011
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#ifndef __EWOL_FILE_CHOOSER_H__
#define __EWOL_FILE_CHOOSER_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/widget/PopUp.h>

namespace ewol {
	class FileChooser : public ewol::PopUp
	{
		public:
			FileChooser(void);
			~FileChooser(void);
			virtual bool OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * eventExternId, etkFloat_t x, etkFloat_t y);
			void SetTitle(etk::String label);
			void SetValidateLabel(etk::String label);
			void SetCancelLabel(etk::String label);
			void SetFolder(etk::String folder);
			void UpdateCurrentFolder(void);
		private:
			int32_t m_widgetTitleId;
			int32_t m_widgetValidateId;
			int32_t m_widgetCancelId;
			int32_t m_widgetCurrentFolderId;
			int32_t m_widgetListFolderId;
			int32_t m_widgetListFileId;
			etk::String m_folder;
	};
};

#endif
