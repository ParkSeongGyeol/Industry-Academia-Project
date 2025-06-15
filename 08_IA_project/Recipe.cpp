#include "Recipe.h"
#include "RawMaterialManager.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cmath>

// ������
Recipe::Recipe() = default;

// ����� ���Է� ��� �� ���
void Recipe::calculateRawMaterialUsage(double batchSize) {
    rawMaterialUsed.clear();
    for (const auto& [name, ratio] : rawMaterialRatio) {
        rawMaterialUsed[name] = batchSize * (ratio / 100.0);
    }
}

// ����� ��� ����
bool Recipe::validateRawMaterialStock(const RawMaterialManager& mgr, double batchSize) const {
    for (const auto& [name, ratio] : rawMaterialRatio) {
        double need = batchSize * (ratio / 100.0);
        if (mgr.getStock(name) < need)
            return false;
    }
    return true;
}

// ��ġ ���� (����� ����, ��ġID/���귮 ���)
std::string Recipe::produceBatch(RawMaterialManager& mgr, double batchSize) {
    calculateRawMaterialUsage(batchSize);
    for (const auto& [name, amount] : rawMaterialUsed) {
        mgr.consumeMaterial(name, amount);
    }
    batchId = "BATCH_" + std::to_string(rand() % 1000000);
    batchOutput = batchSize * 0.95; // ��ȿ ȿ�� 95% ����
    return batchId;
}

// ���� (��ȹ�� �� ���, ����ID/�ѷ� ���)
std::string Recipe::distillBatch(double yieldRate, double headPct, double tailPct) {
    spiritId = "SPIRIT_" + std::to_string(rand() % 1000000);
    spiritOutput = batchOutput * yieldRate; // ��: 80% ����
    double head = spiritOutput * (headPct / 100.0);
    double tail = spiritOutput * (tailPct / 100.0);
    double heart = spiritOutput - head - tail;
    spiritFractions.clear();
    spiritFractions["Head"] = head;
    spiritFractions["Heart"] = heart;
    spiritFractions["Tail"] = tail;
    return spiritId;
}

// ���� (���߷�/������ ���, ����ID ���)
std::string Recipe::ageSpirit(double evaporationRate) {
    agingId = "AGING_" + std::to_string(rand() % 1000000);
    evaporationLoss = spiritFractions["Heart"] * (evaporationRate / 100.0);
    agedOutput = spiritFractions["Heart"] - evaporationLoss;
    return agingId;
}

// ���� (����ID/�������Է� ���)
std::string Recipe::bottleProduct() {
    bottledId = "BOTTLED_" + std::to_string(rand() % 1000000);
    totalBottledVolume = bottleCount * bottleVolume / 1000.0; // L ����
    return bottledId;
}

// ������ �� �α�/��� ��ȯ
std::vector<std::string> Recipe::getProcessLog() const {
    std::vector<std::string> log;
    std::ostringstream oss;
    oss << "[����� ����] ";
    for (const auto& [name, amount] : rawMaterialUsed)
        oss << name << ": " << amount << " ";
    log.push_back(oss.str());

    oss.str(""); oss.clear();
    oss << "[��ȿ] ��ġID: " << batchId << ", ���귮: " << batchOutput << "L";
    log.push_back(oss.str());

    oss.str(""); oss.clear();
    oss << "[����] ����ID: " << spiritId << ", �ѷ�: " << spiritOutput << "L, Head: " << spiritFractions.at("Head")
        << "L, Heart: " << spiritFractions.at("Heart") << "L, Tail: " << spiritFractions.at("Tail") << "L";
    log.push_back(oss.str());

    oss.str(""); oss.clear();
    oss << "[����] ����ID: " << agingId << ", ���߼ս�: " << evaporationLoss << "L, ���� ��: " << agedOutput << "L";
    log.push_back(oss.str());

    oss.str(""); oss.clear();
    oss << "[����] ����ID: " << bottledId << ", �� ��: " << bottleCount << ", ����: " << bottleVolume << "ml, �� ���Է�: " << totalBottledVolume << "L";
    log.push_back(oss.str());

    return log;
}

// CSV ����ȭ/������ȭ (���� ����, �ʿ�� �� ����)
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