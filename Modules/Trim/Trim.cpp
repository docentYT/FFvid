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

static const wxString WILDCARD = "Video";

wxPanel* Trim::createPanel(wxNotebook* parent) {
	wxPanel* mainPanel = new wxPanel(parent, wxID_ANY);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	/* Input file */
	Trim::inputFilePathCtrl = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(600, -1));
	wxButton* inputFilePathButton = new wxButton(mainPanel, wxID_ANY, "Browse");
	inputFilePathButton->Bind(wxEVT_BUTTON, &Trim::selectInput, this);

	wxStaticBoxSizer* inputFilePathSizer = new wxStaticBoxSizer(wxHORIZONTAL, mainPanel, "Input file");
	inputFilePathSizer->AddStretchSpacer();
	inputFilePathSizer->Add(inputFilePathCtrl, 0, wxALL, 10);
	inputFilePathSizer->AddStretchSpacer();
	inputFilePathSizer->Add(inputFilePathButton, 0, wxALL, 10);
	inputFilePathSizer->AddStretchSpacer();

	/* Settings */
	wxStaticText* startTimeLabel = new wxStaticText(mainPanel, wxID_ANY, "Start time (HH:MM:SS):");
	Trim::startTimeCtrl = new wxTextCtrl(mainPanel, wxID_ANY, "00:00:00");
	wxStaticText* endTimeLabel = new wxStaticText(mainPanel, wxID_ANY, "End time (HH:MM:SS):");
	Trim::endTimeCtrl = new wxTextCtrl(mainPanel, wxID_ANY, "01:00:00");

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
	Trim::outputFilePathCtrl = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(600, -1));
	wxButton* outputFilePathButton = new wxButton(mainPanel, wxID_ANY, "Browse");
	outputFilePathButton->Bind(wxEVT_BUTTON, &Trim::selectOutput, this);

	wxStaticBoxSizer* outputFilePathSizer = new wxStaticBoxSizer(wxHORIZONTAL, mainPanel, "Output file");
	outputFilePathSizer->AddStretchSpacer();
	outputFilePathSizer->Add(outputFilePathCtrl, 0, wxALL, 10);
	outputFilePathSizer->AddStretchSpacer();
	outputFilePathSizer->Add(outputFilePathButton, 0, wxALL, 10);
	outputFilePathSizer->AddStretchSpacer();

	/* Progress gague and trim button */
	Trim::progressGauge = new wxGauge(mainPanel, wxID_ANY, 100, wxDefaultPosition, wxSize(600, -1));
	wxButton* trimButton = new wxButton(mainPanel, wxID_ANY, "Trim");
	trimButton->Bind(wxEVT_BUTTON, &Trim::trimVideo, this);

	wxBoxSizer* saveAndExitSizer = new wxBoxSizer(wxHORIZONTAL);
	saveAndExitSizer->AddStretchSpacer();
	saveAndExitSizer->Add(progressGauge, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
	saveAndExitSizer->AddStretchSpacer();
	saveAndExitSizer->Add(trimButton, 0, wxALL, 10);
	saveAndExitSizer->AddStretchSpacer();

	/* Main sizer setup */
	mainSizer->Add(inputFilePathSizer);
	mainSizer->Add(settingsBoxSizer);
	mainSizer->Add(outputFilePathSizer);
	mainSizer->Add(saveAndExitSizer, 0, wxLEFT | wxRIGHT, 15);
	mainPanel->SetSizerAndFit(mainSizer);

	Trim::panel = mainPanel;
	return mainPanel;
}

void Trim::selectInput(wxCommandEvent& evt) {
	wxFileDialog dialog(Trim::panel, wxEmptyString, wxEmptyString, wxEmptyString, WILDCARD, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() == wxID_CANCEL) return;
	Trim::inputFilePathCtrl->SetValue(dialog.GetPath());
}

void Trim::selectOutput(wxCommandEvent& evt) {
	wxFileDialog dialog(Trim::panel, wxEmptyString, wxEmptyString, wxEmptyString, WILDCARD, wxFD_SAVE);
	if (dialog.ShowModal() == wxID_CANCEL) return;
	Trim::outputFilePathCtrl->SetValue(dialog.GetPath());
}

void Trim::trimVideo(wxCommandEvent& evt) {
	wxString inputFilePath = Trim::inputFilePathCtrl->GetValue();
	if (inputFilePath == wxEmptyString) {
		wxMessageBox("Please specify input file.", "Invalid input");
		return;
	}
	wxString outputFilePath = Trim::outputFilePathCtrl->GetValue();
	if (Trim::outputFilePathCtrl->GetValue() == wxEmptyString) {
		wxMessageBox("Please specify output file.", "Invalid output");
		return;
	}

	wxString startTime = Trim::startTimeCtrl->GetValue();
	if (startTime.length() != 8
		or !isdigit(startTime[0]) or !isdigit(startTime[1])
		or !isdigit(startTime[3]) or !isdigit(startTime[4])
		or !isdigit(startTime[6]) or !isdigit(startTime[7])
		or startTime[2] != ':' or startTime[5] != ':') {
		wxMessageBox("Incorrect start time format.", "Invalid time");
		return;
	}

	wxString endTime = Trim::endTimeCtrl->GetValue();
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
		Trim::busy = true;
		std::string command = std::format("ffmpeg -ss {} -to {} -i \"{}\" -c copy -y \"{}\"", (std::string)startTime, (std::string)endTime, (std::string)inputFilePath, (std::string)outputFilePath);
		if (system(command.c_str())) {
			Trim::progressGauge->SetValue(0);
			wxMessageBox("Error!");
		}
		else {
			Trim::progressGauge->SetValue(100);
			wxMessageBox("Trimming completed.");
		}
		Trim::busy = false;
	};
	std::thread ffmpegThread{f};
	Trim::progressGauge->Pulse();
	ffmpegThread.detach();
}

void Trim::getTimeFromString(wxString& string, int& h, int& m, int& s) {
	h = std::atoi(new char[2] {string[0], string[1]});
	m = std::atoi(new char[2] {string[3], string[4]});
	s = std::atoi(new char[2] {string[6], string[7]});
}