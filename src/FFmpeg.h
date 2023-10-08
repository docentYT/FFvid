#pragma once

#include <string>
#include <string_view>

#include <wx/gauge.h>

class FFmpeg {
public:
	FFmpeg(FFmpeg& other) = delete;
	void operator=(const FFmpeg&) = delete;
	
	static FFmpeg* GetInstance();

	bool isInstalled();
	bool isBusy();

	void removeData(const std::string_view inputFilePath, bool removeVideo, bool removeAudio, bool removeSubtitle, bool removeDataStreams, const std::string_view outputFilePath, wxGauge* progressGauge = nullptr);
	void trim(const std::string_view startTime, const std::string_view endTime, const std::string_view inputFilePath, const std::string_view outputFilePath, wxGauge* progressGauge = nullptr);
	void watermark(const std::string_view inputVideoFilePath, const std::string_view inputWatermarkFilePath, const std::string_view transparency, const std::string_view outputFilePath, wxGauge* progressGauge = nullptr);
private:
	FFmpeg() {};
	static FFmpeg* instance;
	unsigned short taskCounter{ 0 };

	void executeCommand(const std::string command, const std::string successMessage, wxGauge* progressGauge = nullptr);

	void startTask(wxGauge* progressGauge = nullptr);
	void endTask(bool success, wxGauge* progressGauge = nullptr);
};