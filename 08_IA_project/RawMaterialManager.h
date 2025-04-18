#pragma once
#include <string>
#include <vector>

struct RawMaterial {
    std::string name{};
    std::string type{};
    std::string origin{};
    double weight_kg = 0.0;
    std::string storage_location{};
    std::string storage_method{};
    std::string expiry_date{};
    std::string entry_date{};
    std::string exit_date{};
};

class RawMaterialManager {
public:
    std::string getSummary();   // ȭ�鿡 ����� ����
    std::vector<std::string> getPageInfoLines();

    void showInventory();       // ���� ���� ���
    void showAllMaterials();    // ��ü �̷� ��ȸ
    void showRawMaterialPage(); // �ܼ� �޴� ����
    void addMaterial();
    void updateMaterial();
    void deleteMaterial();      // ��� ó��
    void searchMaterial();
    void showStorageEnvironment(); // ���� ��� ȯ�� ���� ���


private:
    std::vector<RawMaterial> materials;

    void initializeDummyData();
};