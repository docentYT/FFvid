#pragma once

#include "../Module.h"

#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/gauge.h>
#include <wx/checkbox.h>

#include "../../Controls/FilePathCtrl.h"
#include "../../Controls/FilesPathsOrderedListView.h"

class Join : public Module {
public:
	wxPanel* createPanel(wxNotebook* parent) override;

	using Module::Module;

private:
	FilesPathsOrderedListView* inputFilesList;
	wxGauge* progressGauge;
	wxCheckBox* reencodeCheckBox;
	FilePathCtrl* outputFilePathCtrl;
	int draggedIndex;
	void joinVideo(wxCommandEvent& evt);
};