#include "FFmpeg.h"

#include <string_view>
#include <thread>

#include <wx/msgdlg.h>
#include <wx/file.h>
#include <wx/filename.h>

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

void FFmpeg::join(const std::vector<std::string> inputFiles, const std::string_view outputFilePath, bool reencode, wxGauge* progressGauge) {
	if (reencode) {
		std::string command = "ffmpeg ";
		for (const std::string& file : inputFiles) {
			command.append(FORMAT("-i \"{}\" ", file));
		}
		command.append("-filter_complex \"");
		for (int i = 0; i < inputFiles.size(); i++) {
			command.append(FORMAT("[{}:v] [{}:a] ", i, i));
		}
		command.append(FORMAT("concat=n={}:v=1:a=1 [v] [a]\" ", inputFiles.size()));
		command.append(FORMAT("-map \"[v]\" -map \"[a]\" -y \"{}\"", (std::string)outputFilePath));
		executeCommand(command, "Joining completed.", progressGauge);
	}
	else {
		std::string write = "";
		for (const std::string& file : inputFiles) {
			write.append(FORMAT("file '{}'\n", file));
		}
		wxString name = wxFileName::CreateTempFileName("FFvid");
		wxTempFile* temp = new wxTempFile(name);
		temp->Write(write);
		temp->Commit();
		temp->Flush();

		const std::string& command = FORMAT("ffmpeg -f concat -safe 0 -i \"{}\" -c copy -y \"{}\"", (std::string_view)name, outputFilePath);
		executeCommand(command, "Joining completed.", progressGauge);
	}
}

void FFmpeg::removeData(const std::string_view inputFilePath, bool removeVideo, bool removeAudio, bool removeSubtitle, bool removeDataStreams, const std::string_view outputFilePath, wxGauge* progressGauge) {
	std::string params = "";
	if (removeVideo) params += "-vn ";
	if (removeAudio) params += "-an ";
	if (removeSubtitle) params += "-sn ";
	if (removeDataStreams) params += "-dn ";
	if (params == "-vn -an ") {
		throw "The file must contain at least video or audio.";
	}
	
	const std::string& command = FORMAT("ffmpeg -i \"{}\" -c copy -y {}\"{}\"", inputFilePath, params, outputFilePath);
	executeCommand(command, "Deleting data completed.", progressGauge);
}

void FFmpeg::trim(const std::string_view startTime, const std::string_view endTime, const std::string_view inputFilePath, const std::string_view outputFilePath, wxGauge* progressGauge) {
	const std::string& command = FORMAT("ffmpeg -ss {} -to {} -i \"{}\" -c copy -y \"{}\"", startTime, endTime, inputFilePath, outputFilePath);
	executeCommand(command, "Trimming completed.", progressGauge);
}

void FFmpeg::watermark(const std::string_view inputVideoFilePath, const std::string_view inputWatermarkFilePath, const std::string_view transparency, const std::string_view outputFilePath, wxGauge* progressGauge) {
	const std::string& command = FORMAT("ffmpeg -i \"{}\" -i \"{}\" -filter_complex \"[1]format = rgba, colorchannelmixer = aa = {}[logo]; [0] [logo] overlay = (W - w) / 2:(H - h) / 2 : format = auto, format = yuv420p\" -c:a copy -y \"{}\"", inputVideoFilePath, inputWatermarkFilePath, transparency, outputFilePath);
	executeCommand(command, "Adding watermark completed.", progressGauge);
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