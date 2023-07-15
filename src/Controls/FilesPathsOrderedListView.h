#pragma once

#include <wx/window.h>
#include <wx/listctrl.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/bmpbuttn.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>

class FilesPathsOrderedListView {
public:
	wxListView* const filesListView;
	wxStaticBoxSizer* const sizer;
	FilesPathsOrderedListView(wxWindow* parent, const wxString& staticBoxTitle, const wxString& wildcard, const long dialogStyle);

private:
	wxWindow* const parent;
	wxString wildcard;
	long dialogStyle;

	void addFiles(wxCommandEvent& evt);
	void moveFilesUp(wxCommandEvent& evt);
	void moveFilesDown(wxCommandEvent& evt);
	void removeFiles(wxCommandEvent& evt);
};