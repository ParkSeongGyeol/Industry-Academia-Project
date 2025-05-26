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
    oss << recipeId << "," << name << "," << batchSizeKg << "," << yeastType << "," << fermentationTemp << "," << fermentationHours
        << "," << distillationABV << "," << distillationCount << "," << oakType << "," << agingMonths
        << "," << bottledName << "," << bottleCount << "," << bottleVolume << "," << bottlePrice;
    // �ʿ�� �߰� �ʵ� ����ȭ
    return oss.str();
}

// CSV ������ȭ (���� ����)
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
    // �ʿ�� �߰� �ʵ� ������ȭ
    return r;
}