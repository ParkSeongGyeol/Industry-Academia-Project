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
    void showRawMaterials(); // ��� ���
private:
    std::vector<RawMaterial> getDummyRawMaterials(); // �׽�Ʈ�� ������
};
