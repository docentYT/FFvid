#pragma once
#include "../Module.h"

#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/textctrl.h> // for std::atomic without C4996 error
#include <wx/listctrl.h>
#include <wx/gauge.h>
#include <wx/checkbox.h>

#include "../../Controls/FilePathCtrl.h"

class Join : public Module {
public:
	wxPanel* panel;
	std::atomic<bool> busy{ false };
	wxPanel* createPanel(wxNotebook* parent);

private:
	wxListView* inputFilesList;
	wxGauge* progressGauge;
	wxCheckBox* reencodeCheckBox;
	FilePathCtrl* outputFilePathCtrl;
	int draggedIndex;
	void selectInputFiles(wxCommandEvent& evt);
	void moveFileUp(wxCommandEvent& evt);
	void moveFileDown(wxCommandEvent& evt);
	void removeItem(wxCommandEvent& evt);
	void joinVideo(wxCommandEvent& evt);
};