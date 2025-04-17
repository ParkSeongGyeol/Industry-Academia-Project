#include "FileExporter.h"
#include <fstream>
#include <iostream>

// CSV ��� ���
bool FileExporter::exportToCSV(const std::vector<FermentationBatch>& batches, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    file << "BatchID,StartDate,Temperature,Humidity,Progress\n";
    for (const auto& batch : batches) {
        file << batch.toCSV() << "\n";
    }

    file.close();
    return true;
}

//  �׽�Ʈ�� main �Լ� �߰� (���� main.cpp ����)
#ifdef _DEBUG || TEST_MODE

int main() {
    std::vector<FermentationBatch> batches = {
        FermentationBatch("F001", "2025-04-15", 25.4, 60.2, 75),
        FermentationBatch("F002", "2025-04-14", 26.1, 58.0, 90)
    };

    if (FileExporter::exportToCSV(batches, "fermentation_status.csv")) {
        std::cout << "��ȿ ���°� ���������� ��µǾ����ϴ�." << std::endl;
    }
    else {
        std::cout << "��� ����!" << std::endl;
    }

    return 0;
}

#endif
