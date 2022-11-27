#pragma once
#include <filesystem>
#include <cmath>

#include "ProcessingConfig.h"
#include "DeviceCoordinator.h"


namespace fs = std::filesystem;

class CpuDeviceCoordinator : public DeviceCoordinator {
public:
	CpuDeviceCoordinator(const CoordinatorType coordinatorType,
	                     const ProcessingMode processingMode,
	                     const std::function<void(std::unique_ptr<Job>, size_t)>& jobFinishedCallback,
		std::function<void(size_t)> notifyWatchdogCallback,
	                     const size_t chunkSizeBytes,
	                     const size_t bytesPerAccumulator,
	                     const size_t cpuBufferSizeBytes,
	                     fs::path& distFilePath,
	                     const size_t id
	);

protected:
	size_t nCores{};
	void onProcessJob() override;
};
