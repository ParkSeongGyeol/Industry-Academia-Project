#include "Recipe.h"
#include "RawMaterialManager.h"
#include <sstream>
#include <iomanip>

// 생성자: 기본값 초기화
Recipe::Recipe() = default;

// 배치 생산량에 따라 필요한 원재료별 수량(kg) 계산
std::map<std::string, double> Recipe::getRequiredRawMaterials(double batchSize) const {
    std::map<std::string, double> result;
    for (const auto& [name, ratio] : rawMaterialRatio) {
        result[name] = batchSize * (ratio / 100.0);
    }
    return result;
}

// 원재료 재고가 충분한지 검증
bool Recipe::validateRawMaterialStock(const RawMaterialManager& mgr, double batchSize) const {
    auto required = getRequiredRawMaterials(batchSize);
    for (const auto& [name, amount] : required) {
        if (mgr.getStock(name) < amount)
            return false;
    }
    return true;
}

// 배치 생성에 필요한 정보 반환
std::map<std::string, std::string> Recipe::getBatchParameters() const {
    std::map<std::string, std::string> params;
    params["yeastType"] = yeastType;
    params["fermentationTemp"] = std::to_string(fermentationTemp);
    params["fermentationHours"] = std::to_string(fermentationHours);
    return params;
}

// 스피릿 분할 결과(스피릿명, 분할량) 반환
std::vector<std::pair<std::string, double>> Recipe::getSpiritSplits(double totalAmount) const {
    std::vector<std::pair<std::string, double>> splits;
    double sum = 0.0;
    for (const auto& [name, ratio] : spiritSplits) {
        double amount = totalAmount * (ratio / 100.0);
        splits.emplace_back(name, amount);
        sum += ratio;
    }
    // 비율 합이 100%가 아니면 남은 양을 마지막에 할당
    if (!splits.empty() && sum < 100.0) {
        splits.back().second += totalAmount * ((100.0 - sum) / 100.0);
    }
    return splits;
}

// 숙성(오크) 생성에 필요한 정보 반환
std::map<std::string, std::string> Recipe::getAgingParameters() const {
    std::map<std::string, std::string> params;
    params["oakType"] = oakType;
    params["agingMonths"] = std::to_string(agingMonths);
    return params;
}

// 병입 생성에 필요한 정보 반환
std::map<std::string, std::string> Recipe::getBottlingParameters() const {
    std::map<std::string, std::string> params;
    params["bottledName"] = bottledName;
    params["bottleCount"] = std::to_string(bottleCount);
    params["bottleVolume"] = std::to_string(bottleVolume);
    params["bottlePrice"] = std::to_string(bottlePrice);
    return params;
}

// 레시피 정의 자체의 유효성 검사 (예: 원재료/스피릿 비율 합 100%)
bool Recipe::validateRecipe() const {
    double sum = 0.0;
    for (const auto& [_, ratio] : rawMaterialRatio) sum += ratio;
    if (std::abs(sum - 100.0) > 1e-3) return false;
    sum = 0.0;
    for (const auto& [_, ratio] : spiritSplits) sum += ratio;
    if (!spiritSplits.empty() && std::abs(sum - 100.0) > 1e-3) return false;
    return true;
}

// 각 공정별 ID 기록
void Recipe::addUsedRawMaterialId(const std::string& id) {
    usedRawMaterialIds.push_back(id);
}
void Recipe::setBatchId(const std::string& id) {
    batchId = id;
}
void Recipe::addSpiritId(const std::string& id) {
    spiritIds.push_back(id);
}
void Recipe::addOakAgingId(const std::string& id) {
    oakAgingIds.push_back(id);
}
void Recipe::addBottledId(const std::string& id) {
    bottledIds.push_back(id);
}

// CSV 직렬화 (간단 예시)
std::string Recipe::toCSV() const {
    std::ostringstream oss;
    oss << recipeId << "," << name << "," << batchSizeKg << "," << yeastType << "," << fermentationTemp << "," << fermentationHours
        << "," << distillationABV << "," << distillationCount << "," << oakType << "," << agingMonths
        << "," << bottledName << "," << bottleCount << "," << bottleVolume << "," << bottlePrice;
    // 필요시 추가 필드 직렬화
    return oss.str();
}

// CSV 역직렬화 (간단 예시)
Recipe Recipe::fromCSV(const std::string& line) {
    Recipe r;
    std::istringstream iss(line);
    std::string tmp;
    std::getline(iss, r.recipeId, ',');
    std::getline(iss, r.name, ',');
    std::getline(iss, tmp, ','); r.batchSizeKg = std::stod(tmp);
    std::getline(iss, r.yeastType, ',');
    std::getline(iss, tmp, ','); r.fermentationTemp = std::stod(tmp);
    std::getline(iss, tmp, ','); r.fermentationHours = std::stoi(tmp);
    std::getline(iss, tmp, ','); r.distillationABV = std::stod(tmp);
    std::getline(iss, tmp, ','); r.distillationCount = std::stoi(tmp);
    std::getline(iss, r.oakType, ',');
    std::getline(iss, tmp, ','); r.agingMonths = std::stoi(tmp);
    std::getline(iss, r.bottledName, ',');
    std::getline(iss, tmp, ','); r.bottleCount = std::stoi(tmp);
    std::getline(iss, tmp, ','); r.bottleVolume = std::stod(tmp);
    std::getline(iss, tmp, ','); r.bottlePrice = std::stod(tmp);
    // 필요시 추가 필드 역직렬화
    return r;
}