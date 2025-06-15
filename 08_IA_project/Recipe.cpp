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
    // 원재료 비율 직렬화
    std::string ratioStr;
    for (const auto& [k, v] : rawMaterialRatio) {
        if (!ratioStr.empty()) ratioStr += ";";
        ratioStr += k + ":" + std::to_string(v);
    }
    oss << recipeId << "," << name << "," << ratioStr << "," << yeastType << "," << fermentationTemp << "," << fermentationHours
        << "," << distillationABV << "," << distillationCount << "," << oakType << "," << agingMonths
        << "," << bottledName << "," << bottleCount << "," << bottleVolume << "," << bottlePrice;
    return oss.str();
}

// CSV 역직렬화 (간단 예시)
Recipe Recipe::fromCSV(const std::string& line) {
    Recipe r;
    std::istringstream iss(line);
    std::string tmp;
    getline(iss, r.recipeId, ',');
    getline(iss, r.name, ',');
    getline(iss, tmp, ','); // ratio
    std::istringstream riss(tmp);
    std::string pair;
    while (getline(riss, pair, ';')) {
        auto pos = pair.find(':');
        if (pos != std::string::npos) {
            r.rawMaterialRatio[pair.substr(0, pos)] = stod(pair.substr(pos + 1));
        }
    }
    getline(iss, r.yeastType, ',');
    getline(iss, tmp, ','); r.fermentationTemp = stod(tmp);
    getline(iss, tmp, ','); r.fermentationHours = stoi(tmp);
    getline(iss, tmp, ','); r.distillationABV = stod(tmp);
    getline(iss, tmp, ','); r.distillationCount = stoi(tmp);
    getline(iss, r.oakType, ',');
    getline(iss, tmp, ','); r.agingMonths = stoi(tmp);
    getline(iss, r.bottledName, ',');
    getline(iss, tmp, ','); r.bottleCount = stoi(tmp);
    getline(iss, tmp, ','); r.bottleVolume = stod(tmp);
    getline(iss, tmp, ','); r.bottlePrice = stod(tmp);
    return r;
}

// 배치 생성
std::string Recipe::produceBatch(RawMaterialManager& mgr, double batchSize) {
    // 1. 필요 원재료 계산
    auto required = getRequiredRawMaterials(batchSize);

    // 2. 재고 차감
    for (const auto& [name, amount] : required) {
        mgr.consumeMaterial(name, amount);
    }

    // 3. 배치 ID 생성 및 저장
    std::string newBatchId = "BATCH_" + std::to_string(rand()); // 실제로는 더 안전한 ID 생성 필요
    setBatchId(newBatchId);

    // 4. 사용된 원재료 ID 기록 (선택)
    // mgr.materials에서 출고된 원재료의 ID를 usedRawMaterialIds에 추가하는 로직 구현 가능

    return newBatchId;
}