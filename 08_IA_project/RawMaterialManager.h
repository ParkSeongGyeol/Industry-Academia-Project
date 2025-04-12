#pragma once
#include <string>
#include <vector>

struct RawMaterial {
    std::string name;
    std::string type;
    std::string origin;
    int stock_kg;
};

class RawMaterialManager {
public:
    std::string getSummary(); // ��� ���� ��ȯ
    void showRawMaterials(); // ��� ���
private:
    std::vector<RawMaterial> getDummyRawMaterials(); // �׽�Ʈ�� ������
};
