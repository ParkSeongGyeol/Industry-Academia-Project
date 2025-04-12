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
    void showRawMaterials(); // 목록 출력
private:
    std::vector<RawMaterial> getDummyRawMaterials(); // 테스트용 데이터
};
