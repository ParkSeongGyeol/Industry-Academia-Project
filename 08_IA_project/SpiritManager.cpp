// -----------------------------------------------------------------------------
// SpiritManager.cpp
// ���Ǹ�(������) ���� ��� ���� ���� (������ ���� ����)
// -----------------------------------------------------------------------------

#include "SpiritManager.h"
#include "RecipeManager.h"
#include "Recipe.h"
#include "UIUtils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <vector>
#include "CommonUtils.h"

using namespace std;

// ----------------------------- ��� ���� -----------------------------
namespace {
    constexpr char SPIRIT_CSV[] = "spirit_dummy.csv";
}

// ----------------------------- [1] ������ ����� -----------------------------

// CSV���� ���Ǹ� ��� �ε�
void SpiritManager::loadSpiritsFromCSV(const string& filename) {
    spirits.clear();
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[���] ���Ǹ� CSV ������ �� �� �����ϴ�: " << filename << endl;
        return;
    }
    string line;
    getline(file, line); // ��� ��ŵ
    while (getline(file, line)) {
        if (line.empty()) continue;
        istringstream iss(line);
        Spirit s;
        string token;
        getline(iss, token, ','); s.setId(token);
        getline(iss, token, ','); s.setRawMaterialRatio(token);
        getline(iss, token, ','); s.setBatchId(token);
        getline(iss, token, ','); s.setDistillationCount(stoi(token));
        getline(iss, token, ','); s.setYieldLiters(stod(token));
        getline(iss, token, ','); // FirstDistillAmount (�̻��)
        getline(iss, token, ','); // LastDistillAmount (�̻��)
        getline(iss, token, ','); // MainDistillAmount (�̻��)
        getline(iss, token, ','); s.setProductionDate(token);
        getline(iss, token, ','); s.setAlcoholPercentage(stod(token));
        // ������ �ʵ�� �� ������ ��
        spirits.push_back(s);
    }
    file.close();
}

// ���Ǹ� ����� CSV�� ����
void SpiritManager::saveSpiritsToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "[����] ���Ǹ� CSV ���� ����: " << filename << endl;
        return;
    }
    // ���
    file << "SpiritID,IngredientContent,BatchID,DistillationCount,TotalAmount,FirstDistillAmount,LastDistillAmount,MainDistillAmount,ProductionDate,ABV\n";
    for (const auto& s : spirits) {
        file << s.getId() << ","
            << s.getRawMaterialRatio() << ","
            << s.getBatchId() << ","
            << s.getDistillationCount() << ","
            << s.getYieldLiters() << ","
            << "" << "," // FirstDistillAmount
            << "" << "," // LastDistillAmount
            << "" << "," // MainDistillAmount
            << s.getProductionDate() << ","
            << s.getAlcoholPercentage() << "\n";
    }
    file.close();
}

// ----------------------------- [Spirit] Getter/Setter ���� -----------------------------

// Getter
std::string Spirit::getId() const { return id; }
std::string Spirit::getStorageLocation() const { return storage_location; }
std::string Spirit::getTransferHistory() const { return transfer_history; }
std::string Spirit::getProductionDate() const { return production_date; }
double Spirit::getAlcoholPercentage() const { return alcohol_percentage; }
double Spirit::getYieldLiters() const { return yield_liters; }
std::string Spirit::getRawMaterialRatio() const { return raw_material_ratio; }
int Spirit::getFermentationDays() const { return fermentation_days; }
std::string Spirit::getFirstCutTime() const { return first_cut_time; }
std::string Spirit::getLastCutTime() const { return last_cut_time; }
int Spirit::getDistillationCount() const { return distillation_count; }
std::string Spirit::getParticleSize() const { return particle_size; }
std::string Spirit::getBatchId() const { return batch_id; }
std::string Spirit::getCutYield() const { return cut_yield; }

// Setter
void Spirit::setId(const std::string& val) { id = val; }
void Spirit::setStorageLocation(const std::string& val) { storage_location = val; }
void Spirit::setTransferHistory(const std::string& val) { transfer_history = val; }
void Spirit::setProductionDate(const std::string& val) { production_date = val; }
void Spirit::setAlcoholPercentage(double val) { alcohol_percentage = val; }
void Spirit::setYieldLiters(double val) { yield_liters = val; }
void Spirit::setRawMaterialRatio(const std::string& val) { raw_material_ratio = val; }
void Spirit::setFermentationDays(int val) { fermentation_days = val; }
void Spirit::setFirstCutTime(const std::string& val) { first_cut_time = val; }
void Spirit::setLastCutTime(const std::string& val) { last_cut_time = val; }
void Spirit::setDistillationCount(int val) { distillation_count = val; }
void Spirit::setParticleSize(const std::string& val) { particle_size = val; }
void Spirit::setBatchId(const std::string& val) { batch_id = val; }
void Spirit::setCutYield(const std::string& val) { cut_yield = val; }


// ----------------------------- [2] ������ ��� ���Ǹ� ���� -----------------------------
/**
 * ������ ��� ���Ǹ� ����
 * - ������ ����� �����ְ�, ����ڰ� ������ ID�� ��ġ��, ���� ������ �Է�
 * - ���� ����(����, ��ȹ���� ��) �Է� ��, Spirit ��ü�� ���
 */
void SpiritManager::produceSpiritByRecipe(RecipeManager& recipeMgr) {
    // 1. ������ ��� ��� �� ����
    recipeMgr.listRecipes();
    string recipeId = inputString("\n���Ǹ� ���꿡 ����� ������ ID �Է�: ");
    Recipe recipe;
    if (!recipeMgr.getRecipeById(recipeId, recipe)) {
        cout << "�ش� ID�� �����Ǹ� ã�� �� �����ϴ�.\n";
        UIUtils::pauseConsole();
        return;
    }

    // 2. ��ġ ���귮 �Է�
    double batchSize = inputDouble("������ ��ġ��(kg): ");

    // 3. ���� ����/��ȹ���� �Է�
    double yieldRate = inputDouble("���� ����(0~1, ��: 0.8): ");
    double headPct = inputDouble("Head ����(%): ");
    double tailPct = inputDouble("Tail ����(%): ");

    // 4. ���� ���� ����
    recipe.distillBatch(yieldRate, headPct, tailPct);

    // 5. Spirit ��ü ���� �� ���
    Spirit s;
    s.setId(recipe.spiritId);
    s.setRawMaterialRatio([&] {
        ostringstream oss;
        for (auto it = recipe.rawMaterialUsed.begin(); it != recipe.rawMaterialUsed.end(); ++it) {
            if (it != recipe.rawMaterialUsed.begin()) oss << ";";
            oss << it->first << ":" << fixed << setprecision(2) << it->second << "kg";
        }
        return oss.str();
    }());
    s.setBatchId(recipe.batchId);
    s.setDistillationCount(recipe.distillationCount);
    s.setYieldLiters(recipe.spiritOutput);
    s.setProductionDate(getCurrentDate());
    s.setAlcoholPercentage(recipe.distillationABV);
    // ��Ÿ �ʵ�� �ʿ�� �߰� �Է�

    spirits.push_back(s);
    saveSpiritsToCSV(SPIRIT_CSV);

    cout << "������ ��� ���Ǹ��� �����Ǿ����ϴ�. (ID: " << s.getId() << ")\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [3] ���� ���/��ȸ/��� -----------------------------

// ��ü ���Ǹ� ��� ����(����/��� ���� ��) ��ȯ
string SpiritManager::getSummary() {
    size_t count = spirits.size();
    double totalYield = 0, totalAbv = 0;
    for (const auto& s : spirits) {
        totalYield += s.getYieldLiters();
        totalAbv += s.getAlcoholPercentage();
    }
    string result = "���Ǹ�: " + to_string(count) + "��";
    if (count > 0) {
        result += " / ��� ����: " + to_string(static_cast<int>(totalAbv / count)) + "%";
    }
    return result;
}

// ��ú���/�޴��� ���� ��� ���� ��ȯ
vector<string> SpiritManager::getPageInfoLines() {
    vector<string> lines;
    size_t count = spirits.size();
    double totalYield = 0;
    double totalAbv = 0;
    for (const auto& s : spirits) {
        totalYield += s.getYieldLiters();
        totalAbv += s.getAlcoholPercentage();
    }
    lines.push_back("��ϵ� ���Ǹ� ��: " + to_string(count) + "��");
    if (count > 0) {
        lines.push_back("�� ���귮: " + to_string((int)totalYield) + "L");
        lines.push_back("��� ����: " + to_string((int)(totalAbv / count)) + "%");
    } else {
        lines.push_back("�� ���귮: 0L");
        lines.push_back("��� ����: -");
    }
    return lines;
}

// ��ü ���Ǹ� ��� ���
void SpiritManager::displaySpirits() {
    cout << "\n=== ���Ǹ� ��� ===\n";
    for (const auto& s : spirits) {
        cout << "ID: " << s.getId() << "\n"
            << "����� �Է�: " << s.getRawMaterialRatio() << "\n"
            << "��ġ ID: " << s.getBatchId() << "\n"
            << "���� Ƚ��: " << s.getDistillationCount() << "\n"
            << "���귮: " << s.getYieldLiters() << "L\n"
            << "���� ��¥: " << s.getProductionDate() << "\n"
            << "����: " << s.getAlcoholPercentage() << "%\n"
            << "--------------------------\n";
    }
    UIUtils::pauseConsole();
}

// ----------------------------- [4] CSV �������� -----------------------------

// ���Ǹ� ����� CSV�� ��������
void SpiritManager::exportSpiritsToCSV(const string& filename) {
    saveSpiritsToCSV(filename);
    cout << "[ " << filename << " ] ���Ϸ� ���� �Ϸ�!\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [5] �Է�/����/����/�˻� -----------------------------

// ���Ǹ� ���� �߰� �Է� ó��
void SpiritManager::addSpirit() {
    Spirit spirit;
    string input;
    double doubleInput;
    int intInput;

    cout << "\n=== ���Ǹ� �߰� ===\n";
    spirit.setId(inputString("ID: "));
    spirit.setStorageLocation(inputString("���� ���: "));
    spirit.setTransferHistory(inputString("�̵� ���: "));
    spirit.setProductionDate(inputString("���� ��¥ (YYYY-MM-DD): "));
    spirit.setAlcoholPercentage(inputDouble("����(%): "));
    spirit.setYieldLiters(inputDouble("���귮(L): "));
    spirit.setRawMaterialRatio(inputString("����� �Է�: "));
    spirit.setFermentationDays(inputInt("��ȿ �Ⱓ(��): "));
    spirit.setFirstCutTime(inputString("�ʷ� ���� ����: "));
    spirit.setLastCutTime(inputString("�ķ� �׸� ���� ����: "));
    spirit.setDistillationCount(inputInt("���� Ƚ��: "));
    spirit.setParticleSize(inputString("���� ũ�� (��: �̺и�, 2mm ���� ��): "));
    spirit.setBatchId(inputString("��� ��ġ ID: "));
    spirit.setCutYield(inputString("�ʷ�/�ķ�/���� ���淮 ���: "));

    spirits.push_back(spirit);
    saveSpiritsToCSV(SPIRIT_CSV);
    cout << "���Ǹ� �߰� �Ϸ�!\n";
    UIUtils::pauseConsole();
}

// Ư�� ID�� �������� ���Ǹ� ����
void SpiritManager::deleteSpirit() {
    string id = inputString("������ ���Ǹ� ID �Է�: ");
    auto it = remove_if(spirits.begin(), spirits.end(), [&](const Spirit& s) {
        return s.getId() == id;
    });
    if (it != spirits.end()) {
        spirits.erase(it, spirits.end());
        saveSpiritsToCSV(SPIRIT_CSV);
        cout << "���� �Ϸ�!\n";
    } else {
        cout << "�ش� ID�� ���Ǹ��� ã�� �� �����ϴ�.\n";
    }
    UIUtils::pauseConsole();
}

// ���Ǹ� ���� ����
void SpiritManager::updateSpirit() {
    string id = inputString("\n������ ���Ǹ� ID �Է�: ");
    for (auto& s : spirits) {
        if (s.getId() == id) {
            cout << "=== ���Ǹ� ���� ���� ===\n";
            string input;
            cout << "���� ��� (" << s.getStorageLocation() << "): ";
            getline(cin, input); if (!input.empty()) s.setStorageLocation(input);
            cout << "�̵� ��� (" << s.getTransferHistory() << "): ";
            getline(cin, input); if (!input.empty()) s.setTransferHistory(input);
            cout << "���� ��¥ (" << s.getProductionDate() << "): ";
            getline(cin, input); if (!input.empty()) s.setProductionDate(input);
            cout << "���� (%) (" << s.getAlcoholPercentage() << "): ";
            getline(cin, input); if (!input.empty()) s.setAlcoholPercentage(stod(input));
            cout << "���귮 (L) (" << s.getYieldLiters() << "): ";
            getline(cin, input); if (!input.empty()) s.setYieldLiters(stod(input));
            cout << "����� �Է� (" << s.getRawMaterialRatio() << "): ";
            getline(cin, input); if (!input.empty()) s.setRawMaterialRatio(input);
            cout << "��ȿ �Ⱓ (��) (" << s.getFermentationDays() << "): ";
            getline(cin, input); if (!input.empty()) s.setFermentationDays(stoi(input));
            cout << "�ʷ� ���� ���� (" << s.getFirstCutTime() << "): ";
            getline(cin, input); if (!input.empty()) s.setFirstCutTime(input);
            cout << "�ķ� ���� ���� (" << s.getLastCutTime() << "): ";
            getline(cin, input); if (!input.empty()) s.setLastCutTime(input);
            cout << "���� Ƚ�� (" << s.getDistillationCount() << "): ";
            getline(cin, input); if (!input.empty()) s.setDistillationCount(stoi(input));
            cout << "���� ũ�� (" << s.getParticleSize() << "): ";
            getline(cin, input); if (!input.empty()) s.setParticleSize(input);
            cout << "��� ��ġ ID (" << s.getBatchId() << "): ";
            getline(cin, input); if (!input.empty()) s.setBatchId(input);
            cout << "�ʷ�/�ķ�/���� ���淮 (" << s.getCutYield() << "): ";
            getline(cin, input); if (!input.empty()) s.setCutYield(input);
            saveSpiritsToCSV(SPIRIT_CSV);
            cout << "���Ǹ� ���� �Ϸ�!\n";
            UIUtils::pauseConsole();
            return;
        }
    }
    cout << "�ش� ID�� ���Ǹ��� ã�� �� �����ϴ�.\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [6] ���� �޴� ���� -----------------------------

// ���Ǹ� ���� ���� �޴� (������ ���� ����)
void SpiritManager::showSpiritPage() {
    loadSpiritsFromCSV(SPIRIT_CSV);

    RecipeManager recipeMgr;
    recipeMgr.loadRecipesFromCSV("recipe_list.csv");

    int choice;
    do {
        system("cls");
        cout << "=== ���Ǹ� ���� �޴� ===\n\n";
        vector<string> infoLines = getPageInfoLines();
        vector<string> menu = {
            "[1] ���Ǹ� ��� ����",
            "[2] ���Ǹ� �߰�",
            "[3] ���Ǹ� ����",
            "[4] ���Ǹ� ����",
            "[5] CSV�� ����",
            "[6] ������ ��� ���Ǹ� ����",
            "[0] �������� ���ư���"
        };
        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        cout << "\n�Է� >> ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: displaySpirits(); break;
        case 2: addSpirit(); break;
        case 3: deleteSpirit(); break;
        case 4: updateSpirit(); break;
        case 5: exportSpiritsToCSV(SPIRIT_CSV); break;
        case 6: produceSpiritByRecipe(recipeMgr); break;
        case 0: cout << "�������� ���ư��ϴ�...\n"; break;
        default: cout << "�߸��� �Է��Դϴ�.\n"; break;
        }

        if (choice != 0) {
            cout << "\n����Ϸ��� Enter�� ��������...";
            cin.get();
        }
    } while (choice != 0);
}