#include "Recipe.h"
#include "RawMaterialManager.h"
#include <sstream>
#include <iomanip>

// ������: �⺻�� �ʱ�ȭ
Recipe::Recipe() = default;

// ��ġ ���귮�� ���� �ʿ��� ����Ằ ����(kg) ���
std::map<std::string, double> Recipe::getRequiredRawMaterials(double batchSize) const {
    std::map<std::string, double> result;
    for (const auto& [name, ratio] : rawMaterialRatio) {
        result[name] = batchSize * (ratio / 100.0);
    }
    return result;
}

// ����� ��� ������� ����
bool Recipe::validateRawMaterialStock(const RawMaterialManager& mgr, double batchSize) const {
    auto required = getRequiredRawMaterials(batchSize);
    for (const auto& [name, amount] : required) {
        if (mgr.getStock(name) < amount)
            return false;
    }
    return true;
}

// ��ġ ������ �ʿ��� ���� ��ȯ
std::map<std::string, std::string> Recipe::getBatchParameters() const {
    std::map<std::string, std::string> params;
    params["yeastType"] = yeastType;
    params["fermentationTemp"] = std::to_string(fermentationTemp);
    params["fermentationHours"] = std::to_string(fermentationHours);
    return params;
}

// ���Ǹ� ���� ���(���Ǹ���, ���ҷ�) ��ȯ
std::vector<std::pair<std::string, double>> Recipe::getSpiritSplits(double totalAmount) const {
    std::vector<std::pair<std::string, double>> splits;
    double sum = 0.0;
    for (const auto& [name, ratio] : spiritSplits) {
        double amount = totalAmount * (ratio / 100.0);
        splits.emplace_back(name, amount);
        sum += ratio;
    }
    // ���� ���� 100%�� �ƴϸ� ���� ���� �������� �Ҵ�
    if (!splits.empty() && sum < 100.0) {
        splits.back().second += totalAmount * ((100.0 - sum) / 100.0);
    }
    return splits;
}

// ����(��ũ) ������ �ʿ��� ���� ��ȯ
std::map<std::string, std::string> Recipe::getAgingParameters() const {
    std::map<std::string, std::string> params;
    params["oakType"] = oakType;
    params["agingMonths"] = std::to_string(agingMonths);
    return params;
}

// ���� ������ �ʿ��� ���� ��ȯ
std::map<std::string, std::string> Recipe::getBottlingParameters() const {
    std::map<std::string, std::string> params;
    params["bottledName"] = bottledName;
    params["bottleCount"] = std::to_string(bottleCount);
    params["bottleVolume"] = std::to_string(bottleVolume);
    params["bottlePrice"] = std::to_string(bottlePrice);
    return params;
}

// ������ ���� ��ü�� ��ȿ�� �˻� (��: �����/���Ǹ� ���� �� 100%)
bool Recipe::validateRecipe() const {
    double sum = 0.0;
    for (const auto& [_, ratio] : rawMaterialRatio) sum += ratio;
    if (std::abs(sum - 100.0) > 1e-3) return false;
    sum = 0.0;
    for (const auto& [_, ratio] : spiritSplits) sum += ratio;
    if (!spiritSplits.empty() && std::abs(sum - 100.0) > 1e-3) return false;
    return true;
}

// �� ������ ID ���
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

// CSV ����ȭ (���� ����)
std::string Recipe::toCSV() const {
    std::ostringstream oss;
    // ����� ���� ����ȭ
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

// CSV ������ȭ (���� ����)
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

// ��ġ ����
std::string Recipe::produceBatch(RawMaterialManager& mgr, double batchSize) {
    // 1. �ʿ� ����� ���
    auto required = getRequiredRawMaterials(batchSize);

    // 2. ��� ����
    for (const auto& [name, amount] : required) {
        mgr.consumeMaterial(name, amount);
    }

    // 3. ��ġ ID ���� �� ����
    std::string newBatchId = "BATCH_" + std::to_string(rand()); // �����δ� �� ������ ID ���� �ʿ�
    setBatchId(newBatchId);

    // 4. ���� ����� ID ��� (����)
    // mgr.materials���� ���� ������� ID�� usedRawMaterialIds�� �߰��ϴ� ���� ���� ����

    return newBatchId;
}