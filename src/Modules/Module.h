#pragma once

#include <wx/string.h>
#include <wx/panel.h>
#include <wx/notebook.h>

class Module {
public:
	const wxString name;
	virtual wxPanel* createPanel(wxNotebook* parent) = 0;

	Module(const wxString& name) :
		name{ name } {};
};