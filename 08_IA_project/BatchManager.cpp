// -----------------------------------------------------------------------------
// BatchManager.cpp
// ��ȿ ��ġ ���� ��� ���� ���� (������ ���� ����)
// -----------------------------------------------------------------------------

#include "BatchManager.h"
#include "RecipeManager.h"
#include "Recipe.h"
#include "RawMaterialManager.h"
#include "UIUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <iomanip>
#include <ctime>

using namespace std;

// ----------------------------- ��� ���� -----------------------------
namespace {
    constexpr int CSV_FIELD_COUNT = 11;
    constexpr char BATCH_CSV[] = "batch_dummy.csv";
    constexpr char RECIPE_CSV[] = "recipe_list.csv";
    constexpr char RAW_MATERIAL_CSV[] = "rawmaterial_dummy.csv";
}

// ----------------------------- ��ƿ��Ƽ �Լ� -----------------------------

// ���� �ý��� ��¥�� "YYYY-MM-DD" �������� ��ȯ
string getCurrentDate() {
    time_t now = time(nullptr);
    tm t;
    localtime_s(&t, &now);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", &t);
    return string(buf);
}

// ������ double �Է� �Լ�
double inputDouble(const string& prompt) {
    double val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cout << "���ڸ� �Է��ϼ���.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ������ int �Է� �Լ�
int inputInt(const string& prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cout << "������ �Է��ϼ���.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ������ string �Է� �Լ�
string inputString(const string& prompt) {
    cout << prompt;
    string val;
    getline(cin, val);
    return val;
}

// ----------------------------- [FermentationBatch ����] -----------------------------

string FermentationBatch::getBatchId() const { return batch_id; }
void FermentationBatch::setBatchId(const string& val) { batch_id = val; }
string FermentationBatch::getYeastType() const { return yeast_type; }
void FermentationBatch::setYeastType(const string& val) { yeast_type = val; }
string FermentationBatch::getIngredientInfo() const { return ingredient_info; }
void FermentationBatch::setIngredientInfo(const string& val) { ingredient_info = val; }
string FermentationBatch::getStartDate() const { return start_date; }
void FermentationBatch::setStartDate(const string& val) { start_date = val; }
int FermentationBatch::getDurationHours() const { return duration_hours; }
void FermentationBatch::setDurationHours(int val) { duration_hours = val; }
double FermentationBatch::getTemperature() const { return temperature; }
void FermentationBatch::setTemperature(double val) { temperature = val; }
double FermentationBatch::getAmountLiters() const { return amount_liters; }
void FermentationBatch::setAmountLiters(double val) { amount_liters = val; }
string FermentationBatch::getEndDate() const { return end_date; }
void FermentationBatch::setEndDate(const string& val) { end_date = val; }
string FermentationBatch::getExpiryDate() const { return expiry_date; }
void FermentationBatch::setExpiryDate(const string& val) { expiry_date = val; }
string FermentationBatch::getParticleSize() const { return particle_size; }
void FermentationBatch::setParticleSize(const string& val) { particle_size = val; }
string FermentationBatch::getBatchFinishDate() const { return batch_finish_date; }
void FermentationBatch::setBatchFinishDate(const string& val) { batch_finish_date = val; }

// CSV ����ȭ
string FermentationBatch::toCSV() const {
    ostringstream oss;
    oss << batch_id << "," << yeast_type << "," << ingredient_info << "," << start_date << ","
        << duration_hours << "," << temperature << "," << amount_liters << "," << end_date << ","
        << expiry_date << "," << particle_size << "," << batch_finish_date;
    return oss.str();
}

// CSV ������ȭ
FermentationBatch FermentationBatch::fromCSV(const string& line) {
    FermentationBatch b;
    istringstream iss(line);
    string token;
    getline(iss, b.batch_id, ',');
    getline(iss, b.yeast_type, ',');
    getline(iss, b.ingredient_info, ',');
    getline(iss, b.start_date, ',');
    getline(iss, token, ','); b.duration_hours = stoi(token);
    getline(iss, token, ','); b.temperature = stod(token);
    getline(iss, token, ','); b.amount_liters = stod(token);
    getline(iss, b.end_date, ',');
    getline(iss, b.expiry_date, ',');
    getline(iss, b.particle_size, ',');
    getline(iss, b.batch_finish_date, ',');
    return b;
}

// ----------------------------- [1] ������ ����� -----------------------------

// CSV ���Ͽ��� ��ġ ����� �о�� batches ���Ϳ� ����
void BatchManager::loadBatchesFromCSV(const string& filename) {
    batches.clear();
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[���] ��ġ CSV ������ �� �� �����ϴ�: " << filename << endl;
        return;
    }
    string line;
    getline(file, line); // ��� ��ŵ
    while (getline(file, line)) {
        if (line.empty()) continue;
        batches.push_back(FermentationBatch::fromCSV(line));
    }
    file.close();
}

// batches ������ ��� ��ġ ������ ������ CSV ���Ϸ� ����
void BatchManager::saveBatchesToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "[����] ���� ���� ����: " << filename << endl;
        return;
    }
    file << "BatchID,YeastType,IngredientComposition,StartDate,FermentationDuration,Temperature,AmountLiters,EndDate,ExpiryDate,ParticleSize,BatchFinishDate\n";
    for (const auto& b : batches) {
        file << b.toCSV() << "\n";
    }
    file.close();
}

// ----------------------------- [2] ������ ��� ��ġ ���� -----------------------------
/**
 * ������ ��� ��ġ ����
 * - ������ ����� �����ְ�, ����ڰ� ������ ID�� ��ġ���� �Է�
 * - ����� ��� ���� �� ������ RawMaterialManager���� ó��
 * - ��ġ ������ FermentationBatch�� ���
 */
void BatchManager::produceBatchByRecipe(RecipeManager& recipeMgr, RawMaterialManager& rawMgr) {
    recipeMgr.listRecipes();
    string recipeId = inputString("\n����� ������ ID �Է�: ");
    Recipe recipe;
    if (!recipeMgr.getRecipeById(recipeId, recipe)) {
        cout << "�ش� ID�� �����Ǹ� ã�� �� �����ϴ�.\n";
        UIUtils::pauseConsole();
        return;
    }
    double batchSize = inputDouble("������ ��ġ��(kg): ");
    if (!recipe.validateRawMaterialStock(rawMgr, batchSize)) {
        cout << "��� �����Ͽ� ��ġ ������ �Ұ��մϴ�.\n";
        UIUtils::pauseConsole();
        return;
    }
    string batchId = recipe.produceBatch(rawMgr, batchSize);
    rawMgr.saveMaterialsToCSV("rawmaterial_dummy.csv"); // ����� ���� �� ����

    FermentationBatch b;
    b.setBatchId(batchId);
    b.setYeastType(recipe.yeastType);
    ostringstream oss;
    for (auto it = recipe.rawMaterialUsed.begin(); it != recipe.rawMaterialUsed.end(); ++it) {
        if (it != recipe.rawMaterialUsed.begin()) oss << ";";
        oss << it->first << ":" << fixed << setprecision(2) << it->second << "kg";
    }
    b.setIngredientInfo(oss.str());
    b.setStartDate(getCurrentDate());
    b.setDurationHours(recipe.fermentationHours);
    b.setTemperature(recipe.fermentationTemp);
    b.setAmountLiters(recipe.batchOutput); // �����ǿ��� ��·� ���
    b.setEndDate(getCurrentDate());
    b.setExpiryDate("-");
    b.setParticleSize("��");
    b.setBatchFinishDate(getCurrentDate());

    batches.push_back(b);
    saveBatchesToCSV("batch_dummy.csv"); // ��ġ ���� ����

    cout << "������ ��� ��ġ�� �����Ǿ����ϴ�. (��ġID: " << batchId << ")\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [3] ���� ���/��ȸ/��� -----------------------------

// ��ü ��ġ ��� ����(���� ��) ��ȯ
string BatchManager::getSummary() {
    return "��ġ: " + to_string(batches.size()) + "��";
}

// ��ú���/�޴��� ���� ��� ���� ��ȯ
vector<string> BatchManager::getPageInfoLines() {
    int total = static_cast<int>(batches.size());
    int fermenting = count_if(batches.begin(), batches.end(), [](const FermentationBatch& b) { return b.getDurationHours() < 72; });
    int completed = total - fermenting;
    double avg = 0.0;
    if (!batches.empty()) {
        int sum = 0;
        for (const auto& b : batches) sum += b.getDurationHours();
        avg = static_cast<double>(sum) / total;
    }
    set<string> locations = { "��ũ" };
    vector<string> lines;
    lines.push_back("��ü ��ġ ��: " + to_string(total) + "��");
    lines.push_back("���� ����: ��ȿ �� " + to_string(fermenting) + "�� / �Ϸ� " + to_string(completed) + "��");
    lines.push_back("��� ��ȿ �ð�: " + to_string(static_cast<int>(avg)) + "�ð�");
    string locStr = "���� ��ġ: ";
    for (const auto& l : locations) locStr += l + " ";
    lines.push_back(locStr);
    return lines;
}

// ��ü ��ġ ��� ���
void BatchManager::showBatchList() {
    cout << "\n=== ��ȿ ��ġ ��� ===\n";
    for (const auto& b : batches) {
        cout << "ID: " << b.getBatchId() << " | ȿ��: " << b.getYeastType() << " | ������: " << b.getStartDate() << " | ��: " << b.getAmountLiters() << "L\n";
    }
    UIUtils::pauseConsole();
}

// ��ġ �� ���� ���
void BatchManager::showBatchDetail() {
    cin.ignore();
    string id = inputString("�� ��ȸ�� ��ġ ID �Է�: ");
    for (const auto& b : batches) {
        if (b.getBatchId() == id) {
            cout << "��ġ ID: " << b.getBatchId() << "\n"
                << "ȿ��: " << b.getYeastType() << "\n"
                << "����� ����: " << b.getIngredientInfo() << "\n"
                << "������: " << b.getStartDate() << "\n"
                << "��ȿ �ð�: " << b.getDurationHours() << "�ð�\n"
                << "�µ�: " << b.getTemperature() << "��C\n"
                << "�� (����): " << b.getAmountLiters() << "L\n"
                << "�Ϸ���: " << b.getEndDate() << "\n"
                << "�������: " << b.getExpiryDate() << "\n"
                << "���� ũ��: " << b.getParticleSize() << "\n"
                << "��ġ ������: " << b.getBatchFinishDate() << "\n";
            UIUtils::pauseConsole();
            return;
        }
    }
    cout << "�ش� ID�� ��ġ�� ã�� �� �����ϴ�.\n";
    UIUtils::pauseConsole();
}

// ��ȿ ���� ��ġ ��� ��� (����)
void BatchManager::showFermentingBatches() {
    cout << "\n=== ��ȿ ���� ��ġ ��� ===\n";
    for (const auto& b : batches) {
        if (b.getDurationHours() < 72) {
            cout << "ID: " << b.getBatchId() << " | ȿ��: " << b.getYeastType() << " | ������: " << b.getStartDate() << "\n";
        }
    }
    UIUtils::pauseConsole();
}

// ----------------------------- [4] CSV �������� -----------------------------

// ��ġ ����� CSV�� ��������
void BatchManager::exportBatchesToCSV() {
    saveBatchesToCSV(BATCH_CSV);
    cout << "\n��ġ ����� CSV ���Ϸ� ����Ǿ����ϴ�.\n";
    cout << "���� ���: " << filesystem::current_path() / BATCH_CSV << "\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [5] �Է�/����/����/�˻� -----------------------------

// ��ġ �ű� �߰� (���� �Է�)
void BatchManager::addBatch() {
    FermentationBatch b;
    string input;
    double dInput;
    int iInput;
    cin.ignore();
    cout << "\n=== �� ��ȿ ��ġ �߰� ===\n";
    b.setBatchId(inputString("��ġ ID: "));
    b.setYeastType(inputString("ȿ�� ����: "));
    b.setIngredientInfo(inputString("����� ���� (��: RM101:60%): "));
    b.setStartDate(inputString("������ (YYYY-MM-DD): "));
    b.setDurationHours(inputInt("��ȿ �ð�(�ð�): "));
    b.setTemperature(inputDouble("�µ�(��C): "));
    b.setAmountLiters(inputDouble("��(����): "));
    b.setEndDate(inputString("�Ϸ��� (YYYY-MM-DD): "));
    b.setExpiryDate(inputString("������� (YYYY-MM-DD): "));
    b.setParticleSize(inputString("���� ũ��(��/��/��): "));
    b.setBatchFinishDate(inputString("��ġ ������ (YYYY-MM-DD): "));

    batches.push_back(b);
    saveBatchesToCSV(BATCH_CSV);
    cout << "��ȿ ��ġ�� �߰��Ǿ����ϴ�.\n";
    UIUtils::pauseConsole();
}

// ��ġ ���� ����
void BatchManager::updateBatch() {
    cin.ignore();
    string id = inputString("������ ��ġ ID �Է�: ");
    for (auto& b : batches) {
        if (b.getBatchId() == id) {
            string input;
            cout << "ȿ�� ���� (" << b.getYeastType() << "): "; getline(cin, input); if (!input.empty()) b.setYeastType(input);
            cout << "����� ���� (" << b.getIngredientInfo() << "): "; getline(cin, input); if (!input.empty()) b.setIngredientInfo(input);
            cout << "������ (" << b.getStartDate() << "): "; getline(cin, input); if (!input.empty()) b.setStartDate(input);
            cout << "��ȿ �ð� (" << b.getDurationHours() << "): "; getline(cin, input); if (!input.empty()) b.setDurationHours(stoi(input));
            cout << "�µ� (" << b.getTemperature() << "): "; getline(cin, input); if (!input.empty()) b.setTemperature(stod(input));
            cout << "�� (" << b.getAmountLiters() << "): "; getline(cin, input); if (!input.empty()) b.setAmountLiters(stod(input));
            cout << "�Ϸ��� (" << b.getEndDate() << "): "; getline(cin, input); if (!input.empty()) b.setEndDate(input);
            cout << "������� (" << b.getExpiryDate() << "): "; getline(cin, input); if (!input.empty()) b.setExpiryDate(input);
            cout << "���� ũ�� (" << b.getParticleSize() << "): "; getline(cin, input); if (!input.empty()) b.setParticleSize(input);
            cout << "��ġ ������ (" << b.getBatchFinishDate() << "): "; getline(cin, input); if (!input.empty()) b.setBatchFinishDate(input);
            saveBatchesToCSV(BATCH_CSV);
            cout << "���� �Ϸ�.\n";
            UIUtils::pauseConsole();
            return;
        }
    }
    cout << "�ش� ID�� ��ġ�� ã�� �� �����ϴ�.\n";
    UIUtils::pauseConsole();
}

// ��ġ ����
void BatchManager::deleteBatch() {
    cin.ignore();
    string id = inputString("������ ��ġ ID �Է�: ");
    auto it = remove_if(batches.begin(), batches.end(), [&](const FermentationBatch& b) { return b.getBatchId() == id; });
    if (it != batches.end()) {
        batches.erase(it, batches.end());
        saveBatchesToCSV(BATCH_CSV);
        cout << "���� �Ϸ�.\n";
    } else {
        cout << "�ش� ID�� ��ġ�� ã�� �� �����ϴ�.\n";
    }
    UIUtils::pauseConsole();
}

// ��ġ �˻�(�� ��ȸ)
void BatchManager::searchBatch() {
    cin.ignore();
    string id = inputString("��ȸ�� ��ġ ID �Է�: ");
    for (const auto& b : batches) {
        if (b.getBatchId() == id) {
            showBatchDetail();
            return;
        }
    }
    cout << "�ش� ID�� ��ġ�� ã�� �� �����ϴ�.\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [6] ��ȿ ���� -----------------------------

// ������ ��ȿ ���� ����(����)
void BatchManager::predictBatchFermentation() {
    cin.ignore();
    string id = inputString("������ ��ġ ID �Է�: ");
    for (const auto& b : batches) {
        if (b.getBatchId() == id) {
            double score = (b.getTemperature() * 0.8) + (b.getDurationHours() * 1.5);
            if (b.getParticleSize() == "��") score *= 1.2;
            else if (b.getParticleSize() == "��") score *= 0.8;
            score = min(100.0, max(0.0, score));
            cout << "\n������ ��ȿ ����: " << score << "% �Դϴ�.\n";
            UIUtils::pauseConsole();
            return;
        }
    }
    cout << "�ش� ID�� ��ġ�� ã�� �� �����ϴ�.\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [7] ���� �޴� ���� -----------------------------

// ��ȿ ��ġ ���� ���� �޴� (������ ���� ����)
void BatchManager::showBatchPage(RecipeManager& recipeMgr, RawMaterialManager& rawMgr) {
    loadBatchesFromCSV("batch_dummy.csv");
    int choice;
    do {
        UIUtils::clearConsole();
        vector<string> infoLines = getPageInfoLines();
        vector<string> menu = {
            "[1] ��ġ ��� ��ȸ",
            "[2] ��ġ �� ��ȸ",
            "[3] �� ��ġ �߰�",
            "[4] ��ġ ���� ����",
            "[5] ��ġ ����",
            "[6] ��ġ �˻�",
            "[7] ��ȿ ���� ����",
            "[8] ������ ��� ��ġ ����",
            "[9] ��ġ CSV ��������",
            "[0] �������� ���ư���"
        };
        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        cout << "\n�Է� >> ";
        cin >> choice;
        switch (choice) {
        case 1: showBatchList(); break;
        case 2: showBatchDetail(); break;
        case 3: addBatch(); break;
        case 4: updateBatch(); break;
        case 5: deleteBatch(); break;
        case 6: searchBatch(); break;
        case 7: predictBatchFermentation(); break;
        case 8: produceBatchByRecipe(recipeMgr, rawMgr); break;
        case 9: exportBatchesToCSV(); break;
        case 0: cout << "���� �޴��� ���ư��ϴ�.\n"; break;
        default: UIUtils::printError("�߸��� �Է��Դϴ�."); UIUtils::pauseConsole(); break;
        }
    } while (choice != 0);
}