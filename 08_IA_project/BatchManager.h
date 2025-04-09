#pragma once
#include <string>
#include <vector>

struct FermentationBatch {
    std::string batch_id;
    std::string start_date;
    double temperature;
    int humidity;
    int duration_hours;
};

class BatchManager {
public:
    //std::vector<FermentationBatch> loadBatchesFromFile(const std::string& filename);
    // ���̺귯�� ���� ���յ� �ý��ۿ��� ���� �����ϱ� ���� �ڵ� ����
    std::vector<FermentationBatch> getDummyBatches();
    void displayBatches(const std::vector<FermentationBatch>& batches);
};

