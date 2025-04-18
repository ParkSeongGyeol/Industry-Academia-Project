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
    std::string getSummary();   // 화면에 띄워줄 정보
    std::vector<std::string> getPageInfoLines();

    void showInventory();       // 현재 보유 재고만
    void showAllMaterials();    // 전체 이력 조회
    void showRawMaterialPage(); // 콘솔 메뉴 루프
    void addMaterial();
    void updateMaterial();
    void deleteMaterial();      // 출고 처리
    void searchMaterial();
    void showStorageEnvironment(); // 보관 장소 환경 정보 출력


private:
    std::vector<RawMaterial> materials;

    void initializeDummyData();
};