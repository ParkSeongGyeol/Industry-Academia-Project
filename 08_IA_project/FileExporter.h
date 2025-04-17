#pragma once
#include "FermentationBatch.h"
#include <vector>
#include <string>

class FileExporter {
public:
    static bool exportToCSV(const std::vector<FermentationBatch>& batches, const std::string& filename);
};
#pragma once
