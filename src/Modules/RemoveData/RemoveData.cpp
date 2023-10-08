#include "RemoveData.h"

#include <thread>

#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/statbox.h>
#include <wx/msgdlg.h>

#include "../../Format.h"

#include "../../Controls/FilePathCtrl.h"
#include "../../Controls/ProcessBar.h"

#include "../../FFmpeg.h"

static const wxString VIDEO_WILDCARD = "Video|*";
static const wxString IMAGE_WILDCARD = "Image|*";

wxPanel* RemoveData::createPanel(wxNotebook* parent) {
	wxPanel* mainPanel = new wxPanel(parent, wxID_ANY);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	inputVideoFile = new FilePathCtrl(mainPanel, "Input file", VIDEO_WILDCARD, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	outputFile = new FilePathCtrl(mainPanel, "Output file", VIDEO_WILDCARD, wxFD_SAVE);
	processBar = new ProcessBar(mainPanel, "Remove data");
	processBar->button->Bind(wxEVT_BUTTON, &RemoveData::removeData, this);

	/* Settings */
	wxCheckBox* removeVideoCheckBox = new wxCheckBox(mainPanel, wxID_ANY, "Remove video");
	wxCheckBox* removeAudioCheckBox = new wxCheckBox(mainPanel, wxID_ANY, "Remove audio");
	wxCheckBox* removeSubtitleCheckBox = new wxCheckBox(mainPanel, wxID_ANY, "Remove subtitle");
	wxCheckBox* removeDataStreamsCheckBox = new wxCheckBox(mainPanel, wxID_ANY, "Remove data streams");

	this->removeVideoCheckBox = removeVideoCheckBox;
	this->removeAudioCheckBox = removeAudioCheckBox;
	this->removeSubtitleCheckBox = removeSubtitleCheckBox;
	this->removeDataStreamsCheckBox = removeDataStreamsCheckBox;

	wxStaticBox* settingsBox = new wxStaticBox(mainPanel, wxID_ANY, "Settings");
	wxStaticBoxSizer* settingsBoxSizer = new wxStaticBoxSizer(settingsBox, wxVERTICAL);
	wxFlexGridSizer* settingsSizer = new wxFlexGridSizer(2, 2, 10, 20);
	settingsSizer->AddGrowableCol(0);
	settingsSizer->AddGrowableCol(1);
#ifdef _WIN32
	settingsSizer->Add(removeVideoCheckBox);
	settingsSizer->Add(removeAudioCheckBox);
	settingsSizer->Add(removeSubtitleCheckBox);
	settingsSizer->Add(removeDataStreamsCheckBox);
#else
	settingsSizer->Add(removeVideoCheckBox, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxBOTTOM, 10);
	settingsSizer->Add(removeAudioCheckBox, 0, wxRIGHT | wxBOTTOM, 10);
	settingsSizer->Add(removeSubtitleCheckBox, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxBOTTOM, 10);
	settingsSizer->Add(removeDataStreamsCheckBox, 0, wxRIGHT | wxBOTTOM, 10);
#endif

	settingsBoxSizer->Add(settingsSizer);

	mainSizer->Add(inputVideoFile->sizer, 0, wxEXPAND);
	mainSizer->Add(settingsBoxSizer);
	mainSizer->Add(outputFile->sizer, 0, wxEXPAND);
	mainSizer->AddStretchSpacer();
	mainSizer->Add(processBar->sizer, 0, wxEXPAND);
	mainPanel->SetSizerAndFit(mainSizer);

	return mainPanel;
}

void RemoveData::removeData(wxCommandEvent& evt) {
	wxString inputFilePath = inputVideoFile->textCtrl->GetValue();
	if (inputFilePath == wxEmptyString) {
		wxMessageBox("Please specify input video file.", "Invalid input");
		return;
	}

	wxString outputFilePath = outputFile->textCtrl->GetValue();
	if (outputFilePath.find('.') == std::string::npos or outputFilePath.find('.') == outputFilePath.Length() - 1) {
		wxMessageBox("Please specify format of output file.", "Invalid output");
		return;
	}

	if (removeVideoCheckBox->IsChecked() && removeAudioCheckBox->IsChecked()) {
		wxMessageBox("The file must contain at least video or audio.", "Invalid options");
		return;
	}

	FFmpeg::GetInstance()->removeData(
		(std::string_view)inputFilePath,
		removeVideoCheckBox->IsChecked(),
		removeAudioCheckBox->IsChecked(),
		removeSubtitleCheckBox->IsChecked(),
		removeDataStreamsCheckBox->IsChecked(),
		(std::string_view)outputFilePath,
		processBar->progressGauge
	);
}