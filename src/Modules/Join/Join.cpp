#include "Join.h"

#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/bmpbuttn.h>
#include <wx/listctrl.h>
#include <wx/filedlg.h>
#include <wx/checkbox.h>

#include <format>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>

#include "../../Controls/FilePathCtrl.h"
#include "../../Controls/FilesPathsOrderedListView.h"

static const wxString WILDCARD = "Video|*";

wxPanel* Join::createPanel(wxNotebook* parent) {
	wxPanel* mainPanel = new wxPanel(parent, wxID_ANY);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	/* Input files */
	Join::inputFilesList = new FilesPathsOrderedListView(mainPanel, "List of files to join", WILDCARD, wxFD_OPEN | wxFD_MULTIPLE | wxFD_FILE_MUST_EXIST);

	/* Output file */
	Join::outputFilePathCtrl = new FilePathCtrl(mainPanel, "Output file", WILDCARD, wxFD_SAVE);

	/* Progress gague and join button */
	Join::progressGauge = new wxGauge(mainPanel, wxID_ANY, 100, wxDefaultPosition, wxSize(300, -1));
	Join::reencodeCheckBox = new wxCheckBox(mainPanel, wxID_ANY, "re-encode (slow)\n(if files don't have the same parameters)", wxDefaultPosition, wxSize(300, -1));
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

	Join::panel = mainPanel;
	return mainPanel;
}

void Join::joinVideo(wxCommandEvent& evt) {
	if (Join::inputFilesList->filesListView->GetItemCount() == 0) {
		wxMessageBox("Please specify input files.", "Invalid input");
		return;
	}

	wxString outputFilePath = Join::outputFilePathCtrl->textCtrl->GetValue();
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
			for (int i = 0; i < Join::inputFilesList->filesListView->GetItemCount(); ++i) {
				write.append(std::format("file '{}'\n", (std::string)Join::inputFilesList->filesListView->GetItemText(i)));
			};
			wxString name = wxFileName::CreateTempFileName("FFvid");
			wxTempFile* temp = new wxTempFile(name);
			temp->Write(write);
			temp->Commit();
			temp->Flush();

			return std::format("ffmpeg -f concat -safe 0 -i \"{}\" -c copy -y \"{}\"", (std::string)name, (std::string)outputFilePath);
		};
		const auto concatVideoFilter = [this, outputFilePath]() -> std::string {
			std::string command = "ffmpeg ";
			for (int i = 0; i < Join::inputFilesList->filesListView->GetItemCount(); ++i) {
				command.append(std::format("-i \"{}\" ", (std::string)Join::inputFilesList->filesListView->GetItemText(i)));
			};
			command.append("-filter_complex \"");
			for (int i = 0; i < Join::inputFilesList->filesListView->GetItemCount(); ++i) {
				command.append(std::format("[{}:v] [{}:a] ", i, i));
			};
			command.append(std::format("concat=n={}:v=1:a=1 [v] [a]\" ", Join::inputFilesList->filesListView->GetItemCount()));
			command.append(std::format("-map \"[v]\" -map \"[a]\" -y \"{}\"", (std::string)outputFilePath));
			return command;
		};

		Join::busy = true;
		Join::inputFilesList->filesListView->Freeze();
		std::string command = (Join::reencodeCheckBox->IsChecked()) ? concatVideoFilter() : concactDemuxer();
		if (system(command.c_str())) {
			Join::progressGauge->SetValue(0);
			wxMessageBox("Error!");
		}
		else {
			Join::progressGauge->SetValue(100);
			wxMessageBox("Joining completed.");
		}
		Join::inputFilesList->filesListView->Thaw();
		Join::busy = false;
	};

	std::thread ffmpegThread{f};
	Join::progressGauge->Pulse();
	ffmpegThread.detach();
}