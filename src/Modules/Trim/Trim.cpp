#include "Trim.h" 

#include <format>
#include <thread>

#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/gauge.h>

#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/log.h>

#include "../../Controls/FilePathCtrl.h"

static const wxString WILDCARD = "Video|*";

wxPanel* Trim::createPanel(wxNotebook* parent) {
	wxPanel* mainPanel = new wxPanel(parent, wxID_ANY);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	/* Input file */
	inputFilePathCtrl = new FilePathCtrl(mainPanel, "Input file", WILDCARD, wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	/* Settings */
	wxStaticText* startTimeLabel = new wxStaticText(mainPanel, wxID_ANY, "Start time (HH:MM:SS):");
	startTimeCtrl = new wxTextCtrl(mainPanel, wxID_ANY, "00:00:00");
	wxStaticText* endTimeLabel = new wxStaticText(mainPanel, wxID_ANY, "End time (HH:MM:SS):");
	endTimeCtrl = new wxTextCtrl(mainPanel, wxID_ANY, "01:00:00");

	wxStaticBox* settingsBox = new wxStaticBox(mainPanel, wxID_ANY, "Settings");
	wxStaticBoxSizer* settingsBoxSizer = new wxStaticBoxSizer(settingsBox, wxVERTICAL);
	wxFlexGridSizer* settingsSizer = new wxFlexGridSizer(2, 2, 10, 20);
	settingsSizer->AddGrowableCol(1);
	settingsSizer->AddGrowableRow(0);
	settingsSizer->Add(startTimeLabel, 0);
	settingsSizer->Add(startTimeCtrl, 0);
	settingsSizer->Add(endTimeLabel, 0);
	settingsSizer->Add(endTimeCtrl, 0);

	settingsBoxSizer->Add(settingsSizer);

	/* Output file */
	outputFilePathCtrl = new FilePathCtrl(mainPanel, "Output file", WILDCARD, wxFD_SAVE);

	/* Progress gague and trim button */
	progressGauge = new wxGauge(mainPanel, wxID_ANY, 100, wxDefaultPosition, wxSize(600, -1));
	wxButton* trimButton = new wxButton(mainPanel, wxID_ANY, "Trim");
	trimButton->Bind(wxEVT_BUTTON, &Trim::trimVideo, this);

	wxBoxSizer* trimSizer = new wxBoxSizer(wxHORIZONTAL);
	trimSizer->AddStretchSpacer();
	trimSizer->Add(progressGauge, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
	trimSizer->AddStretchSpacer();
	trimSizer->Add(trimButton, 0, wxALL, 10);
	trimSizer->AddStretchSpacer();

	/* Main sizer setup */
	mainSizer->Add(inputFilePathCtrl->sizer);
	mainSizer->Add(settingsBoxSizer);
	mainSizer->Add(outputFilePathCtrl->sizer);
	mainSizer->AddStretchSpacer();
	mainSizer->Add(trimSizer, 0, wxLEFT | wxRIGHT, 15);
	mainPanel->SetSizerAndFit(mainSizer);

	panel = mainPanel;
	return mainPanel;
}

void Trim::trimVideo(wxCommandEvent& evt) {
	wxString inputFilePath = inputFilePathCtrl->textCtrl->GetValue();
	if (inputFilePath == wxEmptyString) {
		wxMessageBox("Please specify input file.", "Invalid input");
		return;
	}
	wxString outputFilePath = outputFilePathCtrl->textCtrl->GetValue();
	if (outputFilePath == wxEmptyString) {
		wxMessageBox("Please specify output file.", "Invalid output");
		return;
	}

	wxString startTime = startTimeCtrl->GetValue();
	if (startTime.length() != 8
		or !isdigit(startTime[0]) or !isdigit(startTime[1])
		or !isdigit(startTime[3]) or !isdigit(startTime[4])
		or !isdigit(startTime[6]) or !isdigit(startTime[7])
		or startTime[2] != ':' or startTime[5] != ':') {
		wxMessageBox("Incorrect start time format.", "Invalid time");
		return;
	}

	wxString endTime = endTimeCtrl->GetValue();
	if (endTime.length() != 8
		or !isdigit(endTime[0]) or !isdigit(endTime[1])
		or !isdigit(endTime[3]) or !isdigit(endTime[4])
		or !isdigit(endTime[6]) or !isdigit(endTime[7])
		or endTime[2] != ':' or endTime[5] != ':') {
		wxMessageBox("Incorrect end time format.", "Invalid time");
		return;
	}

	int startH, startM, startS;
	getTimeFromString(startTime, startH, startM, startS);
	int endH, endM, endS;
	getTimeFromString(endTime, endH, endM, endS);

	const auto f = [this, startTime, endTime, inputFilePath, outputFilePath]() {
		busy = true;
		std::string command = std::format("ffmpeg -ss {} -to {} -i \"{}\" -c copy -y \"{}\"", (std::string)startTime, (std::string)endTime, (std::string)inputFilePath, (std::string)outputFilePath);
		if (system(command.c_str())) {
			progressGauge->SetValue(0);
			wxMessageBox("Error!");
		}
		else {
			progressGauge->SetValue(100);
			wxMessageBox("Trimming completed.");
		}
		busy = false;
	};
	std::thread ffmpegThread{f};
	progressGauge->Pulse();
	ffmpegThread.detach();
}

void Trim::getTimeFromString(wxString& string, int& h, int& m, int& s) {
	h = std::atoi(new char[2] {string[0], string[1]});
	m = std::atoi(new char[2] {string[3], string[4]});
	s = std::atoi(new char[2] {string[6], string[7]});
}