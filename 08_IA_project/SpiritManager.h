#pragma once
#include <string>
#include <vector>

// ���Ǹ� ������ �����ϴ� ����ü
struct Spirit {
    std::string id{};                   // ���� ID
    std::string storage_location{};     // ���� ���
    std::string transfer_history{};     // �̵� ���
    std::string production_date{};      // ������
    double alcohol_percentage = 0.0;    // ���ڿ� ���� (%)
    double yield_liters = 0.0;          // ���귮 (����)
    std::string raw_material_ratio{};   // ����� ���� ����
    int fermentation_days = 0;          // ��ȿ �ϼ�
    std::string first_cut_time{};       // �ʷ� ���� ����
    std::string last_cut_time{};        // �ķ� ���� ����
    int distillation_count = 0;         // ���� Ƚ��
};

// ���Ǹ� ���� ����� �����ϴ� Ŭ����
class SpiritManager {
private:
    std::vector<Spirit> spirits; // ��ü ���Ǹ� ��� ����

public:
    std::string getSummary();                  // ��ú��忡 ǥ���� ��� ����
    std::vector<std::string> getPageInfoLines(); // �޴� ��� ��� ����

    void showSpiritPage(); // �ܼ� �޴� ���� ����
    void addSpirit();      // ���Ǹ� �߰�
    void deleteSpirit();   // ���Ǹ� ����
    void displaySpirits(); // ��ü ���Ǹ� ��� ���
    void updateSpirit();   // ���Ǹ� ���� ����
};
