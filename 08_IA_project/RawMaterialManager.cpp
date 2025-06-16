// -----------------------------------------------------------------------------
// RawMaterialManager.cpp
// ����� ���� ��� ���� ���� (������ ���� ����)
// -----------------------------------------------------------------------------

#include "RawMaterialManager.h"
#include "RecipeManager.h"
#include "Recipe.h"
#include "UIUtils.h"
#include "StorageEnvironment.h"
#include <iostream>
#include <set>
#include <map>
#include "CommonUtils.h"
#include <fstream>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <stdexcept>

using namespace std;

// ----------------------------- ��� ���� -----------------------------
// CSV ���� �ʵ� ���� �� ���ϸ� ��� ����
namespace {
    constexpr int CSV_FIELD_COUNT = 17;
    constexpr char RAW_MATERIAL_CSV[] = "rawmaterial_dummy.csv";
    constexpr char USED_MATERIAL_CSV[] = "used_raw_materials.csv";
    constexpr char STOCK_MATERIAL_CSV[] = "stock_raw_materials.csv";
}

// ----------------------------- ��ƿ��Ƽ �Լ� -----------------------------

// RawMaterial ��ü�� CSV ����� ���Ͽ� ���
void writeCSVHeader(ofstream& file, bool withUsedDate = false) {
    file << "ID,Name,Type,Origin,Weight(kg),Storage,StorageMethod,ExpiryDate,EntryDate,ExitDate,"
         << "Status,Unit,UnitPrice,EntryManager,ExitManager,QualityCheck,QualityCheckDate";
    if (withUsedDate) file << ",UsedDate";
    file << "\n";
}

// RawMaterial ��ü�� ������ CSV �� �ٷ� ���Ͽ� ���
void writeCSVRow(ofstream& file, const RawMaterial& m, bool withUsedDate = false) {
    file << m.getMaterialId() << "," << m.getName() << "," << m.getType() << "," << m.getOrigin() << ","
         << m.getWeightKg() << "," << m.getStorageLocation() << "," << m.getStorageMethod() << ","
         << m.getExpiryDate() << "," << m.getEntryDate() << "," << m.getExitDate() << ","
         << m.getStatus() << "," << m.getUnit() << "," << m.getUnitPrice() << ","
         << m.getEntryManager() << "," << m.getExitManager() << ","
         << m.getQualityCheck() << "," << m.getQualityCheckDate();
    if (withUsedDate) file << "," << getCurrentDate();
    file << "\n";
}

// ID�� ����� �˻� (������ ���� �͸� ��ȯ)
RawMaterial* findMaterialById(vector<RawMaterial>& materials, const string& id) {
    for (auto& m : materials) {
        if (m.getMaterialId() == id && m.getExitDate().empty())
            return &m;
    }
    return nullptr;
}

// ----------------------------- [RawMaterial] Getter/Setter ���� -----------------------------

// Getter
string RawMaterial::getMaterialId() const { return material_id; }
string RawMaterial::getName() const { return name; }
string RawMaterial::getType() const { return type; }
string RawMaterial::getOrigin() const { return origin; }
double RawMaterial::getWeightKg() const { return weight_kg; }
string RawMaterial::getStorageLocation() const { return storage_location; }
string RawMaterial::getStorageMethod() const { return storage_method; }
string RawMaterial::getExpiryDate() const { return expiry_date; }
string RawMaterial::getEntryDate() const { return entry_date; }
string RawMaterial::getExitDate() const { return exit_date; }
string RawMaterial::getStatus() const { return status; }
string RawMaterial::getUnit() const { return unit; }
double RawMaterial::getUnitPrice() const { return unit_price; }
string RawMaterial::getEntryManager() const { return entry_manager; }
string RawMaterial::getExitManager() const { return exit_manager; }
string RawMaterial::getQualityCheck() const { return quality_check; }
string RawMaterial::getQualityCheckDate() const { return quality_check_date; }

// Setter
void RawMaterial::setMaterialId(const string& val) { material_id = val; }
void RawMaterial::setName(const string& val) { name = val; }
void RawMaterial::setType(const string& val) { type = val; }
void RawMaterial::setOrigin(const string& val) { origin = val; }
void RawMaterial::setWeightKg(double val) { weight_kg = val; }
void RawMaterial::setStorageLocation(const string& val) { storage_location = val; }
void RawMaterial::setStorageMethod(const string& val) { storage_method = val; }
void RawMaterial::setExpiryDate(const string& val) { expiry_date = val; }
void RawMaterial::setEntryDate(const string& val) { entry_date = val; }
void RawMaterial::setExitDate(const string& val) { exit_date = val; }
void RawMaterial::setStatus(const string& val) { status = val; }
void RawMaterial::setUnit(const string& val) { unit = val; }
void RawMaterial::setUnitPrice(double val) { unit_price = val; }
void RawMaterial::setEntryManager(const string& val) { entry_manager = val; }
void RawMaterial::setExitManager(const string& val) { exit_manager = val; }
void RawMaterial::setQualityCheck(const string& val) { quality_check = val; }
void RawMaterial::setQualityCheckDate(const string& val) { quality_check_date = val; }

// ----------------------------- [1] ������ ����� -----------------------------

void RawMaterialManager::loadMaterialsFromCSV(const string& filename) {
    materials.clear();
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[���] ����� CSV ������ �� �� �����ϴ�: " << filename << endl;
        return;
    }
    string line;
    getline(file, line); // ��� ��ŵ
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;
        vector<string> fields;
        while (getline(ss, token, ',')) {
            fields.push_back(token);
        }
        if (fields.size() < CSV_FIELD_COUNT) continue;
        try {
            RawMaterial m;
            m.setMaterialId(fields[0]);
            m.setName(fields[1]);
            m.setType(fields[2]);
            m.setOrigin(fields[3]);
            m.setWeightKg(stod(fields[4]));
            m.setStorageLocation(fields[5]);
            m.setStorageMethod(fields[6]);
            m.setExpiryDate(fields[7]);
            m.setEntryDate(fields[8]);
            m.setExitDate(fields[9]);
            m.setStatus(fields[10]);
            m.setUnit(fields[11]);
            m.setUnitPrice(stod(fields[12]));
            m.setEntryManager(fields[13]);
            m.setExitManager(fields[14]);
            m.setQualityCheck(fields[15]);
            m.setQualityCheckDate(fields[16]);
            materials.push_back(m);
        } catch (...) {
            cout << "[���] CSV �Ľ� ����: " << line << endl;
        }
    }
    file.close();
}

void RawMaterialManager::saveMaterialsToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "[����] ���� ���� ����: " << filename << endl;
        return;
    }
    writeCSVHeader(file);
    for (const auto& m : materials) {
        writeCSVRow(file, m);
    }
    file.close();
}

bool RawMaterialManager::exportUsedMaterialsToCSV(const string& filename, const vector<RawMaterial>& usedList) {
    ofstream file(filename);
    if (!file.is_open()) return false;
    writeCSVHeader(file, true);
    for (const auto& m : usedList) {
        writeCSVRow(file, m, true);
    }
    file.close();
    return true;
}

// ----------------------------- [2] ���� ���� -----------------------------

double RawMaterialManager::getStock(const string& materialName) const {
    double total = 0;
    for (const auto& m : materials) {
        if (m.getName() == materialName && m.getExitDate().empty())
            total += m.getWeightKg();
    }
    return total;
}

void RawMaterialManager::consumeMaterial(const string& name, double amount) {
    for (auto& m : materials) {
        if (m.getName() == name && m.getExitDate().empty()) {
            if (m.getWeightKg() >= amount) {
                m.setWeightKg(m.getWeightKg() - amount);
                if (m.getWeightKg() == 0)
                    m.setExitDate(getCurrentDate());
                break;
            } else {
                amount -= m.getWeightKg();
                m.setExitDate(getCurrentDate());
                m.setWeightKg(0);
            }
        }
    }
}

// ----------------------------- [2-1] ������ ��� ��ġ ���� ��� -----------------------------
/**
 * ������ ��� ��ġ ����
 * - ����ڰ� ������ ID�� �Է��ϸ�, �ش� �����Ǹ� RecipeManager���� �ҷ��ͼ�
 *   ��� ���� ��, produceBatch�� ȣ���Ͽ� ��ġ ������ ó���Ѵ�.
 * - ��� ���� �� �ȳ� �޽��� ���, ���� �� ��ġID ��� �� ��� CSV ����
 */
void RawMaterialManager::produceBatchByRecipe(RecipeManager& recipeMgr) {
    // 1. ������ ��� ��� �� ����
    recipeMgr.listRecipes();
    string recipeId = inputString("\n����� ������ ID �Է�: ");
    Recipe recipe;
    if (!recipeMgr.getRecipeById(recipeId, recipe)) {
        cout << "�ش� ID�� �����Ǹ� ã�� �� �����ϴ�.\n";
        return;
    }

    // 2. ��ġ ���귮 �Է�
    double batchSize = inputDouble("������ ��ġ��(kg): ");

    // 3. ��� ����
    if (!recipe.validateRawMaterialStock(*this, batchSize)) {
        cout << "��� �����Ͽ� ��ġ ������ �Ұ��մϴ�.\n";
        return;
    }

    // 4. ��ġ ����(����� ���� �� ��ġID ����)
    string batchId = recipe.produceBatch(*this, batchSize);
    saveMaterialsToCSV(RAW_MATERIAL_CSV);

    cout << "��ġ ���� �Ϸ�! ��ġID: " << batchId << "\n";
}

// ----------------------------- [3] ���� ���/��ȸ/��� -----------------------------

// ��ü ����� ��� ����(����/�ѷ�) ��ȯ
string RawMaterialManager::getSummary() {
    double totalKg = 0;
    for (const auto& item : materials)
        if (item.getExitDate().empty())
            totalKg += item.getWeightKg();

    return "�����: " + to_string(materials.size()) + "�� / " + to_string((int)totalKg) + "kg";
}

// ��ú���/�޴��� ���� ��� ���� ��ȯ
vector<string> RawMaterialManager::getPageInfoLines() {
    int totalKinds = 0;
    double totalWeight = 0;
    set<string> storagePlaces;
    map<string, int> typeCount;

    for (const auto& m : materials) {
        if (m.getExitDate().empty()) {
            ++totalKinds;
            totalWeight += m.getWeightKg();
            storagePlaces.insert(m.getStorageLocation());
            typeCount[m.getType()]++;
        }
    }

    vector<string> lines;
    lines.push_back("���� ���� �����: " + to_string(totalKinds) + "�� / " + to_string((int)totalWeight) + "kg");

    string typeSummary = "������ ����: ";
    for (const auto& pair : typeCount)
        typeSummary += pair.first + ": " + to_string(pair.second) + "��  ";
    lines.push_back(typeSummary);

    string storageSummary = "���� ��ġ: ";
    for (const auto& place : storagePlaces)
        storageSummary += place + "  ";
    lines.push_back(storageSummary);

    return lines;
}

// ���� ���� �����(������ ���� �͸�) ��� ���
void RawMaterialManager::showInventory() {
    cout << "\n=== ���� ���� ����� ��� ===\n";
    for (const auto& m : materials) {
        if (m.getExitDate().empty()) {
            cout << "ID: " << m.getMaterialId() << "\n"
                << "�̸�: " << m.getName() << "\n"
                << "����: " << m.getType() << "\n"
                << "������: " << m.getOrigin() << "\n"
                << "���: " << m.getWeightKg() << " " << m.getUnit() << "\n"
                << "�ܰ�: " << m.getUnitPrice() << "��/" << m.getUnit() << "\n"
                << "���� ��ġ: " << m.getStorageLocation() << "\n"
                << "���� ���: " << m.getStorageMethod() << "\n"
                << "�������: " << m.getExpiryDate() << "\n"
                << "�԰���: " << m.getEntryDate() << "\n"
                << "�԰� �����: " << m.getEntryManager() << "\n"
                << "ǰ�� �˻�: " << m.getQualityCheck() << " (" << m.getQualityCheckDate() << ")\n"
                << "-----------------------------\n";
        }
    }
}

// ��ü ����� ����� �̷� ���
void RawMaterialManager::showAllMaterials() {
    cout << "\n=== ��ü ����� ����� �̷� ===\n";
    for (const auto& m : materials) {
        cout << "ID: " << m.getMaterialId() << "\n"
            << "�̸�: " << m.getName() << "\n"
            << "����: " << m.getType() << "\n"
            << "������: " << m.getOrigin() << "\n"
            << "����: " << m.getWeightKg() << " " << m.getUnit() << "\n"
            << "�ܰ�: " << m.getUnitPrice() << "��/" << m.getUnit() << "\n"
            << "���� ��ġ: " << m.getStorageLocation() << "\n"
            << "���� ���: " << m.getStorageMethod() << "\n"
            << "�������: " << m.getExpiryDate() << "\n"
            << "�԰���: " << m.getEntryDate() << "\n"
            << "�����: " << (m.getExitDate().empty() ? "-" : m.getExitDate()) << "\n"
            << "����: " << m.getStatus() << "\n"
            << "�԰� �����: " << m.getEntryManager() << "\n"
            << "��� �����: " << (m.getExitManager().empty() ? "-" : m.getExitManager()) << "\n"
            << "ǰ�� �˻�: " << m.getQualityCheck() << " (" << m.getQualityCheckDate() << ")\n"
            << "-----------------------------\n";
    }
}

// ǰ�� �˻� �̿Ϸ� ����� ��� ���
void RawMaterialManager::showUninspectedMaterials() {
    cout << "\n=== ǰ�� �˻� �̿Ϸ� ����� ��� ===\n";
    bool found = false;

    for (const auto& m : materials) {
        if (m.getQualityCheck().empty() || m.getQualityCheck() == "�˻� ����") {
            found = true;
            cout << "ID: " << m.getMaterialId() << "\n"
                << "�̸�: " << m.getName() << "\n"
                << "�԰���: " << m.getEntryDate() << "\n"
                << "���� ��ġ: " << m.getStorageLocation() << "\n"
                << "�԰� �����: " << m.getEntryManager() << "\n"
                << "-----------------------------\n";
        }
    }

    if (!found) {
        cout << "��� ������ ǰ�� �˻縦 �Ϸ��߽��ϴ�.\n";
    }
}

// ����ں� ����� �̷� ���
void RawMaterialManager::showMaterialsByManager() {
    cin.ignore();
    string manager = inputString("\n��ȸ�� ����� �̸� �Է�: ");

    bool found = false;
    cout << "\n=== " << manager << " ��� ����� �̷� ===\n";

    for (const auto& m : materials) {
        if (m.getEntryManager() == manager || m.getExitManager() == manager) {
            found = true;
            cout << "ID: " << m.getMaterialId() << "\n"
                << "�̸�: " << m.getName() << "\n"
                << "�԰���: " << m.getEntryDate() << " (�԰� ���: " << m.getEntryManager() << ")\n"
                << "�����: " << (m.getExitDate().empty() ? "-" : m.getExitDate())
                << " (��� ���: " << (m.getExitManager().empty() ? "-" : m.getExitManager()) << ")\n"
                << "����: " << m.getStatus() << "\n"
                << "-----------------------------\n";
        }
    }

    if (!found) {
        cout << "�ش� ������� ����� ����� �����ϴ�.\n";
    }
}

// ���� ��� ȯ�� ���� ���
void RawMaterialManager::showStorageEnvironment() {
    vector<StorageEnvironment> storageList = {
        {"â�� A", 18.5f, 55.2f},
        {"���� �����", 12.0f, 70.0f},
        {"����� ������", 22.3f, 40.0f}
    };

    cout << "\n=== ���� ��� ȯ�� ���� ��ȸ ===\n";
    for (const auto& storage : storageList) {
        cout << "-----------------------------\n";
        storage.displayInfo();
    }

    cout << "\n����Ϸ��� Enter�� ��������...";
    cin.ignore(); cin.get();
}

// ----------------------------- [4] CSV �������� -----------------------------

// ����(����) ����Ḧ CSV�� ��������
void RawMaterialManager::exportUsedInventoryToCSV() {
    vector<RawMaterial> usedList;
    for (const auto& m : materials) {
        if (!m.getExitDate().empty()) {
            usedList.push_back(m);
        }
    }

    if (usedList.empty()) {
        cout << "\n���� ����ᰡ �����ϴ�.\n";
        return;
    }

    if (exportUsedMaterialsToCSV(USED_MATERIAL_CSV, usedList)) {
        cout << "\n[" << USED_MATERIAL_CSV << "] ���Ϸ� ���� �Ϸ�!\n";
    }
    else {
        cout << "\nCSV ���忡 �����߽��ϴ�.\n";
    }
}

// ������ ����(���� ����) ����Ḧ CSV�� ��������
void RawMaterialManager::exportRemainingStockToCSV() {
    vector<RawMaterial> stockList;
    for (const auto& m : materials) {
        if (m.getExitDate().empty()) {
            stockList.push_back(m);
        }
    }

    if (stockList.empty()) {
        cout << "\n���� ���� ����ᰡ �����ϴ�.\n";
        return;
    }

    if (exportUsedMaterialsToCSV(STOCK_MATERIAL_CSV, stockList)) {
        cout << "\n[" << STOCK_MATERIAL_CSV << "] ���Ϸ� ���� �Ϸ�!\n";
    }
    else {
        cout << "\nCSV ���忡 �����߽��ϴ�.\n";
    }
}

// ----------------------------- [5] �Է�/����/����/�˻� -----------------------------

// ����� �ű� �߰�
void RawMaterialManager::addMaterial() {
    cin.ignore();
    RawMaterial newItem;
    cout << "\n=== ����� �߰� ===\n";

    newItem.setMaterialId(inputString("ID: "));
    newItem.setName(inputString("�̸�: "));
    newItem.setType(inputString("����: "));
    newItem.setOrigin(inputString("��� ����: "));
    newItem.setWeightKg(inputDouble("����(kg): "));
    newItem.setUnit(inputString("���� (��: kg, L): "));
    newItem.setUnitPrice(inputDouble("�ܰ� (" + newItem.getUnit() + "��): "));
    newItem.setStorageLocation(inputString("���� ��ġ: "));
    newItem.setStorageMethod(inputString("���� ���: "));
    newItem.setExpiryDate(inputString("�������: "));
    newItem.setEntryDate(inputString("�԰���(YYYY-MM-DD): "));
    newItem.setEntryManager(inputString("�԰� �����: "));
    newItem.setQualityCheck(inputString("ǰ�� �˻� ��� (��: ���, �ҷ�, �˻� ����): "));
    newItem.setQualityCheckDate(inputString("ǰ�� �˻� ���� (YYYY-MM-DD): "));

    newItem.setExitDate("");
    newItem.setExitManager("");
    newItem.setStatus("����");

    materials.push_back(newItem);
    saveMaterialsToCSV(RAW_MATERIAL_CSV);
    cout << "����ᰡ �߰��Ǿ����ϴ�.\n";
}

// ����� ���� ����(�̸����� �˻�, ������ ���� �͸�)
void RawMaterialManager::updateMaterial() {
    cin.ignore();
    string name = inputString("\n������ ����� �̸� �Է�: ");

    for (auto& m : materials) {
        if (m.getName() == name && m.getExitDate().empty()) {
            cout << "=== ����� ���� ===\n";

            string input;
            input = inputString("���� ��ġ (" + m.getStorageLocation() + "): ");
            if (!input.empty()) m.setStorageLocation(input);

            input = inputString("���� ��� (" + m.getStorageMethod() + "): ");
            if (!input.empty()) m.setStorageMethod(input);

            input = inputString("������� (" + m.getExpiryDate() + "): ");
            if (!input.empty()) m.setExpiryDate(input);

            input = inputString("����(kg) (" + to_string(m.getWeightKg()) + "): ");
            if (!input.empty()) {
                try {
                    m.setWeightKg(stod(input));
                } catch (...) {}
            }

            saveMaterialsToCSV(RAW_MATERIAL_CSV);
            cout << "���� �Ϸ�.\n";
            return;
        }
    }

    cout << "�ش� �̸��� �����(���)�� ã�� �� �����ϴ�.\n";
}

// ����� ��� ó��(�̸����� �˻�, ����� �Է�)
void RawMaterialManager::deleteMaterial() {
    cin.ignore();
    string name = inputString("\n��� ó���� ����� �̸� �Է�: ");

    for (auto& m : materials) {
        if (m.getName() == name && m.getExitDate().empty()) {
            string date = inputString("����� �Է� (YYYY-MM-DD): ");
            m.setExitDate(date);
            saveMaterialsToCSV(RAW_MATERIAL_CSV);
            cout << "��� ó�� �Ϸ�.\n";
            return;
        }
    }

    cout << "�ش� �̸��� �����(���)�� ã�� �� �����ϴ�.\n";
}

// ����� �̸����� �˻� �� �� ���� ���
void RawMaterialManager::searchMaterial() {
    cin.ignore();
    string name = inputString("\n�˻��� ����� �̸� �Է�: ");

    bool found = false;
    for (const auto& m : materials) {
        if (m.getName() == name) {
            cout << "�̸�: " << m.getName() << "\n"
                << "����: " << m.getType() << "\n"
                << "���: " << m.getOrigin() << "\n"
                << "����: " << m.getWeightKg() << "kg\n"
                << "���� ��ġ: " << m.getStorageLocation() << "\n"
                << "���� ���: " << m.getStorageMethod() << "\n"
                << "�������: " << m.getExpiryDate() << "\n"
                << "�԰���: " << m.getEntryDate() << "\n"
                << "�����: " << (m.getExitDate().empty() ? "-" : m.getExitDate()) << "\n"
                << "-----------------------------\n";
            found = true;
        }
    }

    if (!found)
        cout << "�ش� �̸��� ����Ḧ ã�� �� �����ϴ�.\n";
}

// ----------------------------- [6] ���� �޴� ���� -----------------------------

// ����� ���� ���� �޴� (������ ���� ����)
void RawMaterialManager::showRawMaterialPage() {
    loadMaterialsFromCSV(RAW_MATERIAL_CSV);

    // ������ �Ŵ��� ��ü ���� (���� ���������� �̱���/���� ������ ���� ����)
    RecipeManager recipeMgr;
    recipeMgr.loadRecipesFromCSV("recipe_list.csv"); // ������ ��� CSV���� �ҷ�����

    int choice;
    do {
        system("cls");
        cout << "=== ����� ���� �޴� ===\n\n";

        vector<string> infoLines = getPageInfoLines();
        vector<string> menu = {
            "[1] ���� ���� ��� ����",
            "[2] ��ü �̷� ����",
            "[3] ����� �߰�",
            "[4] ����� ���� ����",
            "[5] ����� ���� (��� ó��)",
            "[6] ����� �˻�",
            "[7] ���� ��� ȯ������ ����",
            "[8] ������ ��� ��ġ ����",
            "[9] ���� ����� CSV Ȯ��/��������",
            "[10] ������ ���� ��� CSV ��������",
            "[11] ǰ�� �˻� �̿Ϸ� ��� ����",
            "[12] ����ں� ����� �̷� ����",
            "[0] �������� ���ư���"
        };

        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        cout << "\n�Է� >> ";
        cin >> choice;

        switch (choice) {
        case 1: showInventory(); break;
        case 2: showAllMaterials(); break;
        case 3: addMaterial(); break;
        case 4: updateMaterial(); break;
        case 5: deleteMaterial(); break;
        case 6: searchMaterial(); break;
        case 7: showStorageEnvironment(); break;
        case 8:
            produceBatchByRecipe(recipeMgr);
            break;
        case 9:
            exportUsedInventoryToCSV();
            break;
        case 10:
            exportRemainingStockToCSV();
            break;
        case 11:
            showUninspectedMaterials();
            break;
        case 12:
            showMaterialsByManager();
            break;
        case 0: cout << "�������� ���ư��ϴ�...\n"; break;
        default: cout << "�߸��� �Է��Դϴ�.\n"; break;
        }

        if (choice != 0) {
            cout << "\n����Ϸ��� Enter�� ��������...";
            cin.ignore(); cin.get();
        }
    } while (choice != 0);
}

