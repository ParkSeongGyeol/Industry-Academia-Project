#include "FileExporter.h"
#include "BatchManager.h" // struct FermentationBatch ���� ����
#include <fstream>        // ���� ��¿� ���
#include <iostream>
#include <filesystem>     // ���� ��� Ȯ�ο� (C++17 �̻󿡼� ��� ����)

//  FileExporter Ŭ������ ���� �Լ�
//  �־��� ��ġ �����͸� CSV ���Ϸ� ����
bool FileExporter::exportToCSV(const std::vector<FermentationBatch>& batches, const std::string& filename) {
    // ���� ��Ʈ�� ���� (���� ���)
    std::ofstream file(filename);

    // ���� ���� ���� �� false ��ȯ
    if (!file.is_open()) return false;

    //  CSV ��� �ۼ�
    file << "BatchID,StartDate,ParticleSize,YeastType,Ingredients,Amount(L),Temperature,Humidity,Duration(H)\n";

    //  �� ��ġ �����͸� �� �پ� CSV �������� ���
    for (const auto& batch : batches) {
        file << batch.toCSV() << "\n";  // FermentationBatch::toCSV() ���
    }

    //  ���� �ݱ�
    file.close();

    //  ���������� ���� �Ϸ�
    return true;
}
