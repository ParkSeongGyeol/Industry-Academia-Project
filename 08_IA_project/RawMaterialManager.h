#pragma once
#include <string>
#include <vector>

// ����� ������ ��� ����ü
struct RawMaterial {
    std::string name{};              // ����� �̸�
    std::string type{};              // ����� ���� (��: �, ��Ÿ)
    std::string origin{};            // ������
    double weight_kg = 0.0;          // ���� (kg)
    std::string storage_location{};  // ���� ��ġ
    std::string storage_method{};    // ���� ���
    std::string expiry_date{};       // �������
    std::string entry_date{};        // �԰���
    std::string exit_date{};         // ����� (��� ���̸� ��� ����)
};

// ����� ���� ����� ����ϴ� Ŭ����
class RawMaterialManager {
public:
    std::string getSummary();   // ��ú���� ��� ���ڿ� ��ȯ
    std::vector<std::string> getPageInfoLines(); // ���� ��� ���� ����

    void showInventory();       // ���� ���� ����Ḹ ���
    void showAllMaterials();    // ��ü ����� �̷� ���
    void showRawMaterialPage(); // �ܼ� �޴� ���� ����
    void addMaterial();         // ����� �߰�
    void updateMaterial();      // ����� ���� ����
    void deleteMaterial();      // ��� ó��
    void searchMaterial();      // �̸����� ����� �˻�
    void showStorageEnvironment(); // ���� ��� ȯ�� ���� ���

private:
    std::vector<RawMaterial> materials; // ����� ����Ʈ (�޸� �� ����)
    void initializeDummyData();        // �ʱ� ���� ������ ����
};
