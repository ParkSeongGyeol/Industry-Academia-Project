#include "RawMaterialManager.h"
#include <iostream>

std::vector<RawMaterial> RawMaterialManager::getDummyRawMaterials() {
    return {
        {"보리", "곡물", "스코틀랜드", 1200},
        {"호밀", "곡물", "미국", 800},
        {"물", "기타", "제조 현장", 5000}
    };
}

void RawMaterialManager::showRawMaterials() {
    auto materials = getDummyRawMaterials();

    std::cout << "\n=== 원재료 목록 ===\n";
    for (const auto& item : materials) {
        std::cout << "이름: " << item.name << std::endl;
        std::cout << "종류: " << item.type << std::endl;
        std::cout << "출신: " << item.origin << std::endl;
        std::cout << "재고량: " << item.stock_kg << "kg" << std::endl;
        std::cout << "------------------------" << std::endl;
    }
}
