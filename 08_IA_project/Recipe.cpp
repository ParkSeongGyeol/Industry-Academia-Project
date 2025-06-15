#include "Recipe.h"
#include "RawMaterialManager.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cmath>

// 생성자
Recipe::Recipe() = default;

// 원재료 투입량 계산 및 기록
void Recipe::calculateRawMaterialUsage(double batchSize) {
    rawMaterialUsed.clear();
    for (const auto& [name, ratio] : rawMaterialRatio) {
        rawMaterialUsed[name] = batchSize * (ratio / 100.0);
    }
}

// 원재료 재고 검증
bool Recipe::validateRawMaterialStock(const RawMaterialManager& mgr, double batchSize) const {
    for (const auto& [name, ratio] : rawMaterialRatio) {
        double need = batchSize * (ratio / 100.0);
        if (mgr.getStock(name) < need)
            return false;
    }
    return true;
}

// 배치 생산 (원재료 차감, 배치ID/생산량 기록)
std::string Recipe::produceBatch(RawMaterialManager& mgr, double batchSize) {
    calculateRawMaterialUsage(batchSize);
    for (const auto& [name, amount] : rawMaterialUsed) {
        mgr.consumeMaterial(name, amount);
    }
    batchId = "BATCH_" + std::to_string(rand() % 1000000);
    batchOutput = batchSize * 0.95; // 발효 효율 95% 가정
    return batchId;
}

// 증류 (분획별 양 계산, 증류ID/총량 기록)
std::string Recipe::distillBatch(double yieldRate, double headPct, double tailPct) {
    spiritId = "SPIRIT_" + std::to_string(rand() % 1000000);
    spiritOutput = batchOutput * yieldRate; // 예: 80% 수율
    double head = spiritOutput * (headPct / 100.0);
    double tail = spiritOutput * (tailPct / 100.0);
    double heart = spiritOutput - head - tail;
    spiritFractions.clear();
    spiritFractions["Head"] = head;
    spiritFractions["Heart"] = heart;
    spiritFractions["Tail"] = tail;
    return spiritId;
}

// 숙성 (증발량/최종양 계산, 숙성ID 기록)
std::string Recipe::ageSpirit(double evaporationRate) {
    agingId = "AGING_" + std::to_string(rand() % 1000000);
    evaporationLoss = spiritFractions["Heart"] * (evaporationRate / 100.0);
    agedOutput = spiritFractions["Heart"] - evaporationLoss;
    return agingId;
}

// 병입 (병입ID/최종병입량 계산)
std::string Recipe::bottleProduct() {
    bottledId = "BOTTLED_" + std::to_string(rand() % 1000000);
    totalBottledVolume = bottleCount * bottleVolume / 1000.0; // L 단위
    return bottledId;
}

// 공정별 상세 로그/요약 반환
std::vector<std::string> Recipe::getProcessLog() const {
    std::vector<std::string> log;
    std::ostringstream oss;
    oss << "[원재료 투입] ";
    for (const auto& [name, amount] : rawMaterialUsed)
        oss << name << ": " << amount << " ";
    log.push_back(oss.str());

    oss.str(""); oss.clear();
    oss << "[발효] 배치ID: " << batchId << ", 생산량: " << batchOutput << "L";
    log.push_back(oss.str());

    oss.str(""); oss.clear();
    oss << "[증류] 증류ID: " << spiritId << ", 총량: " << spiritOutput << "L, Head: " << spiritFractions.at("Head")
        << "L, Heart: " << spiritFractions.at("Heart") << "L, Tail: " << spiritFractions.at("Tail") << "L";
    log.push_back(oss.str());

    oss.str(""); oss.clear();
    oss << "[숙성] 숙성ID: " << agingId << ", 증발손실: " << evaporationLoss << "L, 숙성 후: " << agedOutput << "L";
    log.push_back(oss.str());

    oss.str(""); oss.clear();
    oss << "[병입] 병입ID: " << bottledId << ", 병 수: " << bottleCount << ", 병당: " << bottleVolume << "ml, 총 병입량: " << totalBottledVolume << "L";
    log.push_back(oss.str());

    return log;
}

// CSV 직렬화/역직렬화 (간단 예시, 필요시 상세 구현)
std::string Recipe::toCSV() const {
    std::ostringstream oss;
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