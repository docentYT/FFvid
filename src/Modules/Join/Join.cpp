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

static const wxString WILDCARD = "Video|*";

wxPanel* Join::createPanel(wxNotebook* parent) {
	wxPanel* mainPanel = new wxPanel(parent, wxID_ANY);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	/* Input files */
	Join::inputFilesList = new wxListView(mainPanel, wxID_ANY, wxDefaultPosition, wxSize(620, 1));
	Join::inputFilesList->AppendColumn("File path");
	Join::inputFilesList->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);

	// Buttons
	//add
	wxButton* inputFilePathButton = new wxButton(mainPanel, wxID_ANY, "Add");
	inputFilePathButton->Bind(wxEVT_BUTTON, &Join::selectInputFiles, this);
	//up
	wxBitmap upArrow;
	upArrow.LoadFile("up.bmp", wxBITMAP_TYPE_BMP);
	upArrow.Rescale(upArrow, wxSize(32, 32));
	wxBitmapButton* moveFileUpButton = new wxBitmapButton(mainPanel, wxID_ANY, upArrow);
	moveFileUpButton->Bind(wxEVT_BUTTON, &Join::moveFileUp, this);
	//down
	wxBitmap downArrow;
	downArrow.LoadFile("down.bmp", wxBITMAP_TYPE_BMP);
	downArrow.Rescale(downArrow, wxSize(32, 32));
	wxBitmapButton* moveFileDownButton = new wxBitmapButton(mainPanel, wxID_ANY, downArrow);
	moveFileDownButton->Bind(wxEVT_BUTTON, &Join::moveFileDown, this);
	//remove
	wxButton* deleteInputFileButton = new wxButton(mainPanel, wxID_ANY, "Remove");
	deleteInputFileButton->Bind(wxEVT_BUTTON, &Join::removeItem, this);

	// sizer
	wxBoxSizer* listCtrlButtonsSizer = new wxBoxSizer(wxVERTICAL);
	listCtrlButtonsSizer->Add(inputFilePathButton, 0, wxALL, 10);
	listCtrlButtonsSizer->Add(moveFileUpButton, 0, wxALIGN_CENTER_HORIZONTAL);
	listCtrlButtonsSizer->Add(moveFileDownButton, 0, wxALIGN_CENTER_HORIZONTAL);
	listCtrlButtonsSizer->Add(deleteInputFileButton, 0, wxALL, 10);

	wxStaticBoxSizer* inputFilesSizer = new wxStaticBoxSizer(wxHORIZONTAL, mainPanel, "List of files to join");
	inputFilesSizer->Add(Join::inputFilesList, 1, wxEXPAND);
	inputFilesSizer->Add(listCtrlButtonsSizer);

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
	mainSizer->Add(inputFilesSizer);
	mainSizer->Add(outputFilePathCtrl->sizer);
	mainSizer->AddStretchSpacer();
	mainSizer->Add(joinSizer, 0, wxLEFT | wxRIGHT, 15);
	mainPanel->SetSizerAndFit(mainSizer);

	Join::panel = mainPanel;
	return mainPanel;
}

void Join::selectInputFiles(wxCommandEvent& evt) {
	wxFileDialog dialog(Join::panel, wxEmptyString, wxEmptyString, wxEmptyString, "", wxFD_OPEN | wxFD_MULTIPLE | wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() == wxID_CANCEL) return;
	wxArrayString paths;
	dialog.GetPaths(paths);
	wxListItem item;
	Join::inputFilesList->Freeze();
	auto previousId = Join::inputFilesList->GetItemCount();
	for (auto path : paths) {
		item.SetId(previousId++);
		item.SetText(path);
		Join::inputFilesList->InsertItem(item);
	}
	Join::inputFilesList->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
	Join::inputFilesList->Thaw();
}

void Join::moveFileUp(wxCommandEvent& evt) {
	auto selectedId = Join::inputFilesList->GetFirstSelected();
	if (selectedId <= 0) return;
	wxListItem item;
	Join::inputFilesList->Freeze();
	item.SetId(selectedId);
	Join::inputFilesList->GetItem(item);
	Join::inputFilesList->DeleteItem(selectedId);
	item.SetId(selectedId - 1);
	Join::inputFilesList->InsertItem(item);
	Join::inputFilesList->Select(item, true);
	Join::inputFilesList->Focus(item);
	Join::inputFilesList->Thaw();
}

void Join::moveFileDown(wxCommandEvent& evt) {
	auto selectedId = Join::inputFilesList->GetFirstSelected();
	if (selectedId == -1 or selectedId == Join::inputFilesList->GetItemCount() - 1) return;
	wxListItem item;
	Join::inputFilesList->Freeze();
	item.SetId(selectedId);
	Join::inputFilesList->GetItem(item);
	Join::inputFilesList->DeleteItem(selectedId);
	item.SetId(selectedId + 1);
	Join::inputFilesList->InsertItem(item);
	Join::inputFilesList->Select(item, true);
	Join::inputFilesList->Focus(item);
	Join::inputFilesList->Thaw();
}

void Join::removeItem(wxCommandEvent& evt) {
	auto selectedId = Join::inputFilesList->GetFirstSelected();
	if (selectedId == -1) return;
	Join::inputFilesList->Freeze();
	Join::inputFilesList->DeleteItem(selectedId);
	Join::inputFilesList->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
	Join::inputFilesList->Thaw();
}

void Join::joinVideo(wxCommandEvent& evt) {
	if (Join::inputFilesList->GetItemCount() == 0) {
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
			for (int i = 0; i < Join::inputFilesList->GetItemCount(); ++i) {
				write.append(std::format("file '{}'\n", (std::string)Join::inputFilesList->GetItemText(i)));
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
			for (int i = 0; i < Join::inputFilesList->GetItemCount(); ++i) {
				command.append(std::format("-i \"{}\" ", (std::string)Join::inputFilesList->GetItemText(i)));
			};
			command.append("-filter_complex \"");
			for (int i = 0; i < Join::inputFilesList->GetItemCount(); ++i) {
				command.append(std::format("[{}:v] [{}:a] ", i, i));
			};
			command.append(std::format("concat=n={}:v=1:a=1 [v] [a]\" ", Join::inputFilesList->GetItemCount()));
			command.append(std::format("-map \"[v]\" -map \"[a]\" -y \"{}\"", (std::string)outputFilePath));
			return command;
		};

		Join::busy = true;
		Join::inputFilesList->Freeze();
		std::string command = (Join::reencodeCheckBox->IsChecked()) ? concatVideoFilter() : concactDemuxer();
		if (system(command.c_str())) {
			Join::progressGauge->SetValue(0);
			wxMessageBox("Error!");
		}
		else {
			Join::progressGauge->SetValue(100);
			wxMessageBox("Joining completed.");
		}
		Join::inputFilesList->Thaw();
		Join::busy = false;
	};

	std::thread ffmpegThread{f};
	Join::progressGauge->Pulse();
	ffmpegThread.detach();
}