#include "Trim.h" 

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

#include "../../Format.h"

#include "../../Controls/FilePathCtrl.h"
#include "../../Controls/ProcessBar.h"

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
#ifdef _WIN32
	settingsSizer->Add(startTimeLabel, 0, wxALIGN_CENTER_VERTICAL);
	settingsSizer->Add(startTimeCtrl, 0);
	settingsSizer->Add(endTimeLabel, 0, wxALIGN_CENTER_VERTICAL);
	settingsSizer->Add(endTimeCtrl, 0);
#else
	settingsSizer->Add(startTimeLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	settingsSizer->Add(startTimeCtrl, 0, wxRIGHT, 10);
	settingsSizer->Add(endTimeLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxBOTTOM, 10);
	settingsSizer->Add(endTimeCtrl, 0, wxRIGHT | wxBOTTOM, 10);
#endif

	settingsBoxSizer->Add(settingsSizer);

	/* Output file */
	outputFilePathCtrl = new FilePathCtrl(mainPanel, "Output file", WILDCARD, wxFD_SAVE);

	/* Progress gague and trim button */
	processBar = new ProcessBar(mainPanel, "Trim");
	processBar->button->Bind(wxEVT_BUTTON, &Trim::trimVideo, this);

	/* Main sizer setup */
	mainSizer->Add(inputFilePathCtrl->sizer, 0, wxEXPAND);
	mainSizer->Add(settingsBoxSizer);
	mainSizer->Add(outputFilePathCtrl->sizer, 0, wxEXPAND);
	mainSizer->AddStretchSpacer();
	mainSizer->Add(processBar->sizer, 0, wxEXPAND);
	mainPanel->SetSizerAndFit(mainSizer);

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
		Module::busy = true;
		std::string command = FORMAT("ffmpeg -ss {} -to {} -i \"{}\" -c copy -y \"{}\"", (std::string)startTime, (std::string)endTime, (std::string)inputFilePath, (std::string)outputFilePath);
		if (system(command.c_str())) {
			processBar->progressGauge->SetValue(0);
			wxMessageBox("Error!");
		}
		else {
			processBar->progressGauge->SetValue(100);
			wxMessageBox("Trimming completed.");
		}
		Module::busy = false;
	};
	std::thread ffmpegThread{f};
	processBar->progressGauge->Pulse();
	ffmpegThread.detach();
}

void Trim::getTimeFromString(wxString& string, int& h, int& m, int& s) {
	h = std::atoi(new char[2] {string[0], string[1]});
	m = std::atoi(new char[2] {string[3], string[4]});
	s = std::atoi(new char[2] {string[6], string[7]});
}