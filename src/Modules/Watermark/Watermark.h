#pragma once

#include "../Module.h"

#include <wx/spinctrl.h>

#include "../../Controls/FilePathCtrl.h"
#include "../../Controls/ProcessBar.h"

class Watermark : public Module {
public:
	wxPanel* createPanel(wxNotebook* parent) override;

	using Module::Module;

private:
	FilePathCtrl* inputVideoFile;
	FilePathCtrl* watermarkFile;
	wxSpinCtrl* opacityCtrl;
	FilePathCtrl* outputFile;
	ProcessBar* processBar;

	void addWatermark(wxCommandEvent& evt);
};