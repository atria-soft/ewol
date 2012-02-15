/**
 *******************************************************************************
 * @file ewol/widgetMeta/FileChooser.h
 * @brief ewol File chooser meta widget system (header)
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

extern const char * const ewolEventFileChooserCancel;
extern const char * const ewolEventFileChooserValidate;

namespace ewol {
	class FileChooser : public ewol::PopUp
	{
		public:
			FileChooser(void);
			~FileChooser(void);
			virtual bool OnEventAreaExternal(int32_t widgetID, const char * generateEventId, const char * data, etkFloat_t x, etkFloat_t y);
			void SetTitle(etk::UString label);
			void SetValidateLabel(etk::UString label);
			void SetCancelLabel(etk::UString label);
			void SetFolder(etk::UString folder);
			void SetFileName(etk::UString filename);
			etk::UString GetCompleateFileName(void);
			void UpdateCurrentFolder(void);
		private:
			int32_t m_widgetTitleId;
			int32_t m_widgetValidateId;
			int32_t m_widgetCancelId;
			int32_t m_widgetCurrentFolderId;
			int32_t m_widgetCurrentFileNameId;
			int32_t m_widgetListFolderId;
			int32_t m_widgetListFileId;
			int32_t m_widgetCheckBoxId;
			bool    m_hasSelectedFile;
			etk::UString m_folder;
			etk::UString m_file;
	};
};

#endif
