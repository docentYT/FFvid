#include "FFmpeg.h"

#include <string_view>
#include <thread>

#include <wx/msgdlg.h>

#include "Format.h"

FFmpeg* FFmpeg::GetInstance() {
	if (instance == nullptr) {
		instance = new FFmpeg();
	}

	return instance;
}

bool FFmpeg::isInstalled() {
	startTask();
	bool isInstalled = not system("ffmpeg -version");
	endTask(true);
	return isInstalled;
}

bool FFmpeg::isBusy() {
	return taskCounter != 0;
}

void FFmpeg::trim(const std::string_view startTime, const std::string_view endTime, const std::string_view inputFilePath, const std::string_view outputFilePath, wxGauge* progressGauge) {
	const std::string& command = FORMAT("ffmpeg -ss {} -to {} -i \"{}\" -c copy -y \"{}\"", startTime, endTime, inputFilePath, outputFilePath);
	executeCommand(command, "Trimming completed.", progressGauge);
}

void FFmpeg::executeCommand(const std::string command, const std::string successMessage, wxGauge* progressGauge) {
	startTask(progressGauge);
	const auto f = [this, command, successMessage, progressGauge] {
		if (system(command.c_str())) {
			endTask(false, progressGauge);
			wxMessageBox("Error!");
		}
		else {
			endTask(true, progressGauge);
			wxMessageBox(successMessage);
		}
	};
	std::thread ffmpegThread{ f };
	ffmpegThread.detach();
};


void FFmpeg::startTask(wxGauge* progressGauge) {
	++taskCounter;
	if (progressGauge != nullptr) {
		progressGauge->Pulse();
	}
}

void FFmpeg::endTask(bool success, wxGauge* progressGauge) {
	--taskCounter;
	if (progressGauge != nullptr) {
		if (success) {
			progressGauge->SetValue(100);
		}
		else {
			progressGauge->SetValue(0);
		}
	}
};

FFmpeg* FFmpeg::instance = nullptr;