#include "RawMaterialManager.h"
#include <iostream>

// === RawMaterialManager Ŭ���� ���� ===

std::string RawMaterialManager::getSummary() {
    auto list = getDummyRawMaterials();
    int totalKg = 0;
    for (const auto& item : list)
        totalKg += item.stock_kg;

    return "�����: " + std::to_string(list.size()) + "�� / " + std::to_string(totalKg) + "kg";
}

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

