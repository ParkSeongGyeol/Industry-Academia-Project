#include "RawMaterialManager.h"
#include <iostream>

std::vector<RawMaterial> RawMaterialManager::getDummyRawMaterials() {
    return {
        {"����", "�", "����Ʋ����", 1200},
        {"ȣ��", "�", "�̱�", 800},
        {"��", "��Ÿ", "���� ����", 5000}
    };
}

void RawMaterialManager::showRawMaterials() {
    auto materials = getDummyRawMaterials();

    std::cout << "\n=== ����� ��� ===\n";
    for (const auto& item : materials) {
        std::cout << "�̸�: " << item.name << std::endl;
        std::cout << "����: " << item.type << std::endl;
        std::cout << "���: " << item.origin << std::endl;
        std::cout << "���: " << item.stock_kg << "kg" << std::endl;
        std::cout << "------------------------" << std::endl;
    }
}
