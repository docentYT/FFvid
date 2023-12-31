#include "FilesPathsOrderedListView.h"

FilesPathsOrderedListView::FilesPathsOrderedListView(
	wxWindow* parent, const wxString& staticBoxTitle, const wxString& wildcard, const long dialogStyle) :
	parent(parent),
	filesListView(new wxListView(parent, wxID_ANY)),
	sizer(new wxStaticBoxSizer(wxHORIZONTAL, parent, staticBoxTitle)) {
	this->wildcard = wildcard;
	this->dialogStyle = dialogStyle;

	/* Input files */
	filesListView->AppendColumn("File path");
	filesListView->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);

	// Buttons
	//add
	wxButton* inputFilePathButton = new wxButton(parent, wxID_ANY, "Add");
	inputFilePathButton->Bind(wxEVT_BUTTON, &FilesPathsOrderedListView::addFiles, this);
	//up
	wxBitmap upArrow;
	upArrow.LoadFile("upArrow.bmp", wxBITMAP_TYPE_BMP);
	upArrow.Rescale(upArrow, wxSize(32, 32));
	wxBitmapButton* moveFileUpButton = new wxBitmapButton(parent, wxID_ANY, upArrow);
	moveFileUpButton->Bind(wxEVT_BUTTON, &FilesPathsOrderedListView::moveFilesUp, this);
	//down
	wxBitmap downArrow;
	downArrow.LoadFile("downArrow.bmp", wxBITMAP_TYPE_BMP);
	downArrow.Rescale(downArrow, wxSize(32, 32));
	wxBitmapButton* moveFileDownButton = new wxBitmapButton(parent, wxID_ANY, downArrow);
	moveFileDownButton->Bind(wxEVT_BUTTON, &FilesPathsOrderedListView::moveFilesDown, this);
	//remove
	wxButton* deleteInputFileButton = new wxButton(parent, wxID_ANY, "Remove");
	deleteInputFileButton->Bind(wxEVT_BUTTON, &FilesPathsOrderedListView::removeFiles, this);

	// sizers
	wxBoxSizer* listCtrlButtonsSizer = new wxBoxSizer(wxVERTICAL);
	listCtrlButtonsSizer->Add(inputFilePathButton, 0, wxBOTTOM, 10);
	listCtrlButtonsSizer->Add(moveFileUpButton, 0, wxALIGN_CENTER_HORIZONTAL);
	listCtrlButtonsSizer->Add(moveFileDownButton, 0, wxALIGN_CENTER_HORIZONTAL);
	listCtrlButtonsSizer->Add(deleteInputFileButton, 0, wxTOP, 10);

	sizer->Add(filesListView, 1, wxEXPAND | wxRIGHT, 10);
#ifdef _WIN32
	sizer->Add(listCtrlButtonsSizer, 0, wxALIGN_CENTER_VERTICAL);
#else
	sizer->Add(listCtrlButtonsSizer, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
#endif
}

void FilesPathsOrderedListView::addFiles(wxCommandEvent& evt) {
	wxFileDialog dialog(parent, wxEmptyString, wxEmptyString, wxEmptyString, wildcard, dialogStyle);
	if (dialog.ShowModal() == wxID_CANCEL) return;
	wxArrayString paths;
	dialog.GetPaths(paths);
	wxListItem item;
	filesListView->Freeze();
	auto previousId = filesListView->GetItemCount();
	for (auto path : paths) {
		item.SetId(previousId++);
		item.SetText(path);
		filesListView->InsertItem(item);
	}
	filesListView->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
	filesListView->Thaw();
}

void FilesPathsOrderedListView::moveFilesUp(wxCommandEvent& evt) {
	filesListView->Freeze();

	wxListItem item;
	long itemIndex = -1;
	while ((itemIndex = filesListView->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1) {
		if (itemIndex <= 0) break;
		item.SetId(itemIndex);
		item.SetMask(wxLIST_MASK_TEXT); // On Windows not nessesary
		filesListView->GetItem(item);
		filesListView->DeleteItem(itemIndex);
		item.SetId(itemIndex - 1);
		filesListView->InsertItem(item);
		filesListView->Select(item, true);
		filesListView->Focus(filesListView->GetFirstSelected());
	}

	filesListView->Thaw();
}

void FilesPathsOrderedListView::moveFilesDown(wxCommandEvent& evt) {
	filesListView->Freeze();

	wxArrayInt selectedItems;
#ifdef _WIN32 // there is a bug on wxGTK with GetSelectedItemCount()
	int selectedItemCount = filesListView->GetSelectedItemCount();
	selectedItems.Alloc(selectedItemCount);
#else
	int selectedItemCount = 0;
#endif
	long itemIndex = -1;
	while ((itemIndex = filesListView->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1) {
		if (itemIndex == filesListView->GetItemCount() - 1) {
			filesListView->Thaw();
			return;
		}
		selectedItems.Add(itemIndex);
#ifndef _WIN32
		++selectedItemCount;
#endif
	}
	wxListItem item;
	for (int i = selectedItemCount - 1; i >= 0; i--) {
		int selectedIndex = selectedItems[i];
		item.SetId(selectedIndex);
		item.SetMask(wxLIST_MASK_TEXT); // On Windows not nessesary
		filesListView->GetItem(item);
		filesListView->DeleteItem(selectedIndex);
		item.SetId(selectedIndex + 1);
		filesListView->InsertItem(item);
		filesListView->Select(item, true);
		filesListView->Focus(item);
	}

	filesListView->Thaw();
}

void FilesPathsOrderedListView::removeFiles(wxCommandEvent& evt) {
	filesListView->Freeze();

	wxArrayInt selectedItems;
#ifdef _WIN32 // there is a bug on wxGTK with GetSelectedItemCount()
	int selectedItemCount = filesListView->GetSelectedItemCount();
	selectedItems.Alloc(selectedItemCount);
#else
	int selectedItemCount = 0;
#endif
	long itemIndex = -1;
	while ((itemIndex = filesListView->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1) {
		selectedItems.Add(itemIndex);
#ifndef _WIN32
		++selectedItemCount;
#endif
	}
	for (int i = selectedItemCount - 1; i >= 0; i--) {
		int selectedIndex = selectedItems[i];
		filesListView->Select(selectedIndex, false); // On Windows not nessesary. Without this line on linux you will get assert error in vector.
		filesListView->DeleteItem(selectedIndex);
	}

	filesListView->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
	filesListView->Thaw();
}