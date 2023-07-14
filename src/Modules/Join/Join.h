#pragma once
#include "../Module.h"

#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/textctrl.h> // for std::atomic without C4996 error
#include <wx/listctrl.h>
#include <wx/gauge.h>
#include <wx/checkbox.h>

#include "../../Controls/FilePathCtrl.h"
#include "../../Controls/FilesPathsOrderedListView.h"

class Join : public Module {
public:
	wxPanel* panel;
	std::atomic<bool> busy{ false };
	wxPanel* createPanel(wxNotebook* parent);

private:
	FilesPathsOrderedListView* inputFilesList;
	wxGauge* progressGauge;
	wxCheckBox* reencodeCheckBox;
	FilePathCtrl* outputFilePathCtrl;
	int draggedIndex;
	void joinVideo(wxCommandEvent& evt);
};