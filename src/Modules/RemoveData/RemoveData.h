#pragma once

#include "../Module.h"

#include <wx/checkbox.h>

#include "../../Controls/FilePathCtrl.h"
#include "../../Controls/ProcessBar.h"

class RemoveData : public Module {
public:
	wxPanel* createPanel(wxNotebook* parent) override;

	using Module::Module;

private:
	FilePathCtrl* inputVideoFile;
	wxCheckBox* removeVideoCheckBox;
	wxCheckBox* removeAudioCheckBox;
	wxCheckBox* removeSubtitleCheckBox;
	wxCheckBox* removeDataStreamsCheckBox;
	FilePathCtrl* outputFile;
	ProcessBar* processBar;

	void removeData(wxCommandEvent& evt);
};