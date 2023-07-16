#pragma once

#include "../Module.h"
#include "../../Controls/FilePathCtrl.h"
#include "../../Controls/ProcessBar.h"

class Watermark : public Module {
public:
	wxPanel* panel;
	std::atomic<bool> busy{ false };
	wxPanel* createPanel(wxNotebook* parent) override;

private:
	FilePathCtrl* inputVideoFile;
	FilePathCtrl* watermarkFile;
	wxSpinCtrl* transparencyCtrl;
	FilePathCtrl* outputFile;
	ProcessBar* processBar;

	void addWatermark(wxCommandEvent& evt);
};

