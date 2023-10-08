#include "Watermark.h"

#include <thread>

#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/msgdlg.h>

#include "../../Format.h"

#include "../../Controls/FilePathCtrl.h"
#include "../../Controls/ProcessBar.h"

#include "../../FFmpeg.h"

static const wxString VIDEO_WILDCARD = "Video|*";
static const wxString IMAGE_WILDCARD = "Image|*";

wxPanel* Watermark::createPanel(wxNotebook* parent) {
	wxPanel* mainPanel = new wxPanel(parent, wxID_ANY);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	inputVideoFile = new FilePathCtrl(mainPanel, "Input video file", VIDEO_WILDCARD, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	watermarkFile = new FilePathCtrl(mainPanel, "Watermark file", IMAGE_WILDCARD, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	outputFile = new FilePathCtrl(mainPanel, "Output file", VIDEO_WILDCARD, wxFD_SAVE);
	processBar = new ProcessBar(mainPanel, "Add watermark");
	processBar->button->Bind(wxEVT_BUTTON, &Watermark::addWatermark, this);

	/* Settings */
	wxStaticText* opacityLabel = new wxStaticText(mainPanel, wxID_ANY, "Opacity:");
	wxSpinCtrl* opacityCtrl = new wxSpinCtrl(mainPanel, wxID_ANY);
	opacityCtrl->SetRange(0, 100);
	opacityCtrl->SetValue(50);

	wxStaticBox* settingsBox = new wxStaticBox(mainPanel, wxID_ANY, "Settings");
	wxStaticBoxSizer* settingsBoxSizer = new wxStaticBoxSizer(settingsBox, wxVERTICAL);
	wxFlexGridSizer* settingsSizer = new wxFlexGridSizer(1, 2, 10, 20);
	settingsSizer->AddGrowableCol(1);
	settingsSizer->AddGrowableRow(0);
#ifdef _WIN32
	settingsSizer->Add(opacityLabel);
	settingsSizer->Add(opacityCtrl);
#else
	settingsSizer->Add(opacityLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxBOTTOM, 10);
	settingsSizer->Add(opacityCtrl, 0, wxRIGHT | wxBOTTOM, 10);
#endif
	this->opacityCtrl = opacityCtrl;

	settingsBoxSizer->Add(settingsSizer);

	mainSizer->Add(inputVideoFile->sizer, 0, wxEXPAND);
	mainSizer->Add(watermarkFile->sizer, 0, wxEXPAND);
	mainSizer->Add(settingsBoxSizer);
	mainSizer->Add(outputFile->sizer, 0, wxEXPAND);
	mainSizer->AddStretchSpacer();
	mainSizer->Add(processBar->sizer, 0, wxEXPAND);
	mainPanel->SetSizerAndFit(mainSizer);

	return mainPanel;
}

void Watermark::addWatermark(wxCommandEvent& evt) {
	wxString inputVideoFilePath = inputVideoFile->textCtrl->GetValue();
	if (inputVideoFilePath == wxEmptyString) {
		wxMessageBox("Please specify input video file.", "Invalid input");
		return;
	}

	wxString inputWatermarkFilePath = watermarkFile->textCtrl->GetValue();
	if (inputWatermarkFilePath == wxEmptyString) {
		wxMessageBox("Please specify watermark file.", "Invalid input");
		return;
	}

	wxString outputFilePath = outputFile->textCtrl->GetValue();
	if (outputFilePath.find('.') == std::string::npos or outputFilePath.find('.') == outputFilePath.Length() - 1) {
		wxMessageBox("Please specify format of output file.", "Invalid output");
		return;
	}

	std::string transparency = std::to_string(opacityCtrl->GetValue()/100.0);

	FFmpeg::GetInstance()->watermark(
		(std::string_view)inputVideoFilePath,
		(std::string_view)inputWatermarkFilePath,
		(std::string_view)transparency,
		(std::string_view)outputFilePath,
		processBar->progressGauge);
}