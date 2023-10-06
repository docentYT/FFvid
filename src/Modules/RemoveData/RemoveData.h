#pragma once

#include "../Module.h"

#include <atomic>
#include <wx/checkbox.h>

#include "../../Controls/FilePathCtrl.h"
#include "../../Controls/ProcessBar.h"

class RemoveData : public Module {
public:
	wxPanel* panel;
	std::atomic<bool> busy{ false };
	wxPanel* createPanel(wxNotebook* parent) override;

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