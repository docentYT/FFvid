#include "Join.h"

#include <thread>

#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/bmpbuttn.h>
#include <wx/listctrl.h>
#include <wx/filedlg.h>
#include <wx/checkbox.h>

#include <wx/file.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>

#include "../../Format.h"

#include "../../Controls/FilePathCtrl.h"
#include "../../Controls/FilesPathsOrderedListView.h"

static const wxString WILDCARD = "Video|*";

wxPanel* Join::createPanel(wxNotebook* parent) {
	wxPanel* mainPanel = new wxPanel(parent, wxID_ANY);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	/* Input files */
	inputFilesList = new FilesPathsOrderedListView(mainPanel, "List of files to join", WILDCARD, wxFD_OPEN | wxFD_MULTIPLE | wxFD_FILE_MUST_EXIST);

	/* Output file */
	outputFilePathCtrl = new FilePathCtrl(mainPanel, "Output file", WILDCARD, wxFD_SAVE);

	/* Progress gague and join button */
	progressGauge = new wxGauge(mainPanel, wxID_ANY, 100, wxDefaultPosition, mainPanel->FromDIP(wxSize(260, -1)));
	wxString checkBoxText = "re-encode (slow)\n(if files don't have the same parameters)";
	wxSize checkBoxSize = wxSize(mainPanel->GetTextExtent("(if files don't have the same parameters)").x*1.1, -1);
	reencodeCheckBox = new wxCheckBox(mainPanel, wxID_ANY, checkBoxText, wxDefaultPosition, checkBoxSize);
	wxButton* joinButton = new wxButton(mainPanel, wxID_ANY, "Join");
	joinButton->Bind(wxEVT_BUTTON, &Join::joinVideo, this);

	wxBoxSizer* joinSizer = new wxBoxSizer(wxHORIZONTAL);
	joinSizer->AddStretchSpacer();
	joinSizer->Add(progressGauge, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
	joinSizer->AddStretchSpacer();
	joinSizer->Add(reencodeCheckBox, 0, wxALIGN_CENTER_VERTICAL);
	joinSizer->AddStretchSpacer();
	joinSizer->Add(joinButton, 0, wxALL, 10);
	joinSizer->AddStretchSpacer();

	/* Main sizer setup */
	mainSizer->Add(inputFilesList->sizer);
	mainSizer->Add(outputFilePathCtrl->sizer);
	mainSizer->AddStretchSpacer();
	mainSizer->Add(joinSizer, 0, wxLEFT | wxRIGHT, 15);
	mainPanel->SetSizerAndFit(mainSizer);

	panel = mainPanel;
	return mainPanel;
}

void Join::joinVideo(wxCommandEvent& evt) {
	if (inputFilesList->filesListView->GetItemCount() == 0) {
		wxMessageBox("Please specify input files.", "Invalid input");
		return;
	}

	wxString outputFilePath = outputFilePathCtrl->textCtrl->GetValue();
	if (outputFilePath == wxEmptyString) {
		wxMessageBox("Please specify output file.", "Invalid output");
		return;
	}

	if (outputFilePath.find('.') == std::string::npos or outputFilePath.find('.') == outputFilePath.Length() - 1) {
		wxMessageBox("Please specify format of output file.", "Invalid output");
		return;
	}

	const auto f = [this, outputFilePath]() {

		const auto concactDemuxer = [this, outputFilePath]() -> std::string {
			// temp file
			std::string write = "";
			for (int i = 0; i < inputFilesList->filesListView->GetItemCount(); ++i) {
				write.append(FORMAT("file '{}'\n", (std::string)inputFilesList->filesListView->GetItemText(i)));
			};
			wxString name = wxFileName::CreateTempFileName("FFvid");
			wxTempFile* temp = new wxTempFile(name);
			temp->Write(write);
			temp->Commit();
			temp->Flush();

			return FORMAT("ffmpeg -f concat -safe 0 -i \"{}\" -c copy -y \"{}\"", (std::string)name, (std::string)outputFilePath);
		};
		const auto concatVideoFilter = [this, outputFilePath]() -> std::string {
			std::string command = "ffmpeg ";
			for (int i = 0; i < inputFilesList->filesListView->GetItemCount(); ++i) {
				command.append(FORMAT("-i \"{}\" ", (std::string)inputFilesList->filesListView->GetItemText(i)));
			};
			command.append("-filter_complex \"");
			for (int i = 0; i < inputFilesList->filesListView->GetItemCount(); ++i) {
				command.append(FORMAT("[{}:v] [{}:a] ", i, i));
			};
			command.append(FORMAT("concat=n={}:v=1:a=1 [v] [a]\" ", inputFilesList->filesListView->GetItemCount()));
			command.append(FORMAT("-map \"[v]\" -map \"[a]\" -y \"{}\"", (std::string)outputFilePath));
			return command;
		};

		busy = true;
		inputFilesList->filesListView->Freeze();
		std::string command = (reencodeCheckBox->IsChecked()) ? concatVideoFilter() : concactDemuxer();
		if (system(command.c_str())) {
			progressGauge->SetValue(0);
			wxMessageBox("Error!");
		}
		else {
			progressGauge->SetValue(100);
			wxMessageBox("Joining completed.");
		}
		inputFilesList->filesListView->Thaw();
		busy = false;
	};

	std::thread ffmpegThread{f};
	progressGauge->Pulse();
	ffmpegThread.detach();
}