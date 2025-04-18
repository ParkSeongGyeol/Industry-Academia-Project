#pragma once

#include <vector>
#include <string>
#include <filesystem> // C++17 �̻� �ʿ�

#include "BatchManager.h"

// ���Ϸ� ��ȿ ��ġ ����Ʈ�� CSV �������� ����ϴ� ��ƿ��Ƽ
class FileExporter {
public:
    // ��ġ �����͸� CSV ���Ϸ� ����
    static bool exportToCSV(const std::vector<FermentationBatch>& batches, const std::string& filename);
};
