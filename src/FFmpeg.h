#pragma once

#include <atomic>
#include <string>
#include <string_view>

#include <wx/gauge.h>

class FFmpeg {
public:
	std::atomic<bool> busy{ false };

	FFmpeg(FFmpeg& other) = delete;
	void operator=(const FFmpeg&) = delete;
	
	static FFmpeg* GetInstance();

	bool isInstalled();
	void trim(const std::string_view startTime, const std::string_view endTime, const std::string_view inputFilePath, const std::string_view outputFilePath, wxGauge* progressGauge = nullptr);

private:
	FFmpeg() {};
	static FFmpeg* instance;
	unsigned short taskCounter{ 0 };

	void executeCommand(const std::string command, const std::string successMessage, wxGauge* progressGauge = nullptr);

	void startTask(wxGauge* progressGauge = nullptr);
	void endTask(bool success, wxGauge* progressGauge = nullptr);
};