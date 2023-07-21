#pragma once

#include <atomic>

#include <wx/panel.h>
#include <wx/notebook.h>

class Module {
public:
	std::atomic<bool> busy{ false };
	virtual wxPanel* createPanel(wxNotebook* parent) = 0;
};