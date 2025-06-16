#include "RecipeManager.h"
#include "RawMaterialManager.h"
#include "UIUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;

// CSV���� ������ ��� �ҷ�����
void RecipeManager::loadRecipesFromCSV(const string& filename) {
    recipes.clear();
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[���] ������ CSV ������ �� �� �����ϴ�: " << filename << endl;
        return;
    }
    string line;
    getline(file, line); // ��� ��ŵ
    while (getline(file, line)) {
        if (line.empty()) continue;
        recipes.push_back(Recipe::fromCSV(line));
    }
    file.close();
}

// ������ ����� CSV�� ����
void RecipeManager::saveRecipesToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "[����] ������ CSV ���� ����: " << filename << endl;
        return;
    }
    // ���
    file << "ID,Name,RawMaterialRatio,YeastType,FermentationTemp,FermentationHours,DistillationABV,DistillationCount,OakType,AgingMonths,BottledName,BottleCount,BottleVolume,BottlePrice\n";
    for (const auto& r : recipes) {
        file << r.toCSV() << "\n";
    }
    file.close();
}

// ������ ��� ���
void RecipeManager::listRecipes() const {
    cout << "\n=== ��ϵ� ������ ��� ===\n";
    if (recipes.empty()) {
        cout << "��ϵ� �����ǰ� �����ϴ�.\n";
        return;
    }
    for (const auto& r : recipes) {
        cout << "ID: " << r.recipeId << " | �̸�: " << r.name << "\n";
    }
}

// ID�� ������ ã��
bool RecipeManager::getRecipeById(const string& id, Recipe& out) const {
    for (const auto& r : recipes) {
        if (r.recipeId == id) {
            out = r;
            return true;
        }
    }
    return false;
}

// ������ �߰�
void RecipeManager::addRecipe() {
    cin.ignore();
    Recipe r;
    cout << "\n=== ������ �߰� ===\n";
    cout << "ID: "; getline(cin, r.recipeId);
    if (r.recipeId.empty()) { cout << "ID�� �ʼ��Դϴ�.\n"; return; }
    cout << "�̸�: "; getline(cin, r.name);

    // ����� ���� �Է�
    cout << "����� ���� �Է� (��: ����:60;ȣ��:10;��:30): ";
    string ratioStr; getline(cin, ratioStr);
    istringstream iss(ratioStr);
    string token;
    double sum = 0.0;
    while (getline(iss, token, ';')) {
        auto pos = token.find(':');
        if (pos != string::npos) {
            string mat = token.substr(0, pos);
            double val = stod(token.substr(pos + 1));
            r.rawMaterialRatio[mat] = val;
            sum += val;
        }
    }
    if (abs(sum - 100.0) > 1e-3) {
        cout << "����� ������ ���� 100%�� �ƴմϴ�.\n";
        return;
    }

    cout << "ȿ��: "; getline(cin, r.yeastType);
    cout << "��ȿ �µ�(��): "; cin >> r.fermentationTemp; cin.ignore();
    cout << "��ȿ �ð�(�ð�): "; cin >> r.fermentationHours; cin.ignore();
    cout << "���� ����(%): "; cin >> r.distillationABV; cin.ignore();
    cout << "���� Ƚ��: "; cin >> r.distillationCount; cin.ignore();
    cout << "��ũ�� ����: "; getline(cin, r.oakType);
    cout << "���� �Ⱓ(��): "; cin >> r.agingMonths; cin.ignore();
    cout << "���� ��ǰ��: "; getline(cin, r.bottledName);
    cout << "�� ����: "; cin >> r.bottleCount; cin.ignore();
    cout << "���� �뷮(ml): "; cin >> r.bottleVolume; cin.ignore();
    cout << "���� ����: "; cin >> r.bottlePrice; cin.ignore();

    recipes.push_back(r);
    saveRecipesToCSV("recipe_list.csv");
    cout << "�����ǰ� �߰��Ǿ����ϴ�.\n";
}

// ������ ����
void RecipeManager::updateRecipe() {
    cin.ignore();
    cout << "\n������ ������ ID �Է�: ";
    string id; getline(cin, id);
    for (auto& r : recipes) {
        if (r.recipeId == id) {
            cout << "�� �̸�(" << r.name << "): ";
            string name; getline(cin, name);
            if (!name.empty()) r.name = name;
            // (�ʿ�� ������ �ʵ嵵 ���� �Է�)
            saveRecipesToCSV("recipe_list.csv");
            cout << "���� �Ϸ�.\n";
            return;
        }
    }
    cout << "�ش� ID�� �����Ǹ� ã�� �� �����ϴ�.\n";
}

// ������ ����
void RecipeManager::deleteRecipe() {
    cin.ignore();
    cout << "\n������ ������ ID �Է�: ";
    string id; getline(cin, id);
    auto it = remove_if(recipes.begin(), recipes.end(), [&](const Recipe& r) { return r.recipeId == id; });
    if (it != recipes.end()) {
        recipes.erase(it, recipes.end());
        saveRecipesToCSV("recipe_list.csv");
        cout << "���� �Ϸ�.\n";
    } else {
        cout << "�ش� ID�� �����Ǹ� ã�� �� �����ϴ�.\n";
    }
}

// ������ �˻�
void RecipeManager::searchRecipe() const {
    cin.ignore();
    cout << "\n�˻��� ������ �̸� �Է�: ";
    string name; getline(cin, name);
    bool found = false;
    for (const auto& r : recipes) {
        if (r.name.find(name) != string::npos) {
            cout << "ID: " << r.recipeId << " | �̸�: " << r.name << "\n";
            found = true;
        }
    }
    if (!found) cout << "�˻� ����� �����ϴ�.\n";
}

// ���� ���� ���� �� ����
void RecipeManager::runRecipeProcess(const std::string& recipeId, RawMaterialManager& rawMgr) {
    Recipe recipe;
    if (!getRecipeById(recipeId, recipe)) {
        cout << "�ش� ID�� �����Ǹ� ã�� �� �����ϴ�.\n";
        return;
    }

    double batchSize;
    cout << "������ ��ġ��(kg): ";
    if (!(cin >> batchSize) || batchSize <= 0) {
        cout << "��ȿ�� ��ġ���� �Է��ϼ���.\n";
        cin.clear(); cin.ignore(1000, '\n');
        return;
    }
    cin.ignore();

    // 1. ����� ���� �� ��ġ ����
    if (!recipe.validateRawMaterialStock(rawMgr, batchSize)) {
        cout << "��� �����Ͽ� ��ġ ������ �Ұ��մϴ�.\n";
        return;
    }
    recipe.produceBatch(rawMgr, batchSize);
    rawMgr.saveMaterialsToCSV("rawmaterial_dummy.csv");

    // 2. ���� (����/��ȹ���� �Է�)
    double yieldRate = 0.8, headPct = 10, tailPct = 10;
    cout << "���� ����(0~1, �⺻ 0.8): ";
    if (!(cin >> yieldRate) || yieldRate <= 0 || yieldRate > 1) {
        cout << "�⺻�� 0.8 ���\n"; yieldRate = 0.8;
        cin.clear(); cin.ignore(1000, '\n');
    }
    cout << "Head ����(%): ";
    if (!(cin >> headPct) || headPct < 0 || headPct > 100) {
        cout << "�⺻�� 10 ���\n"; headPct = 10;
        cin.clear(); cin.ignore(1000, '\n');
    }
    cout << "Tail ����(%): ";
    if (!(cin >> tailPct) || tailPct < 0 || tailPct > 100) {
        cout << "�⺻�� 10 ���\n"; tailPct = 10;
        cin.clear(); cin.ignore(1000, '\n');
    }
    cin.ignore();
    recipe.distillBatch(yieldRate, headPct, tailPct);

    // 3. ���� (���߷� �Է�)
    double evaporationRate = 2.5;
    cout << "���� ���߷�(%): ";
    if (!(cin >> evaporationRate) || evaporationRate < 0) {
        cout << "�⺻�� 2.5 ���\n"; evaporationRate = 2.5;
        cin.clear(); cin.ignore(1000, '\n');
    }
    cin.ignore();
    recipe.ageSpirit(evaporationRate);

    // 4. ���� (�� ��/�뷮 �Է�)
    cout << "�� ��: ";
    if (!(cin >> recipe.bottleCount) || recipe.bottleCount <= 0) {
        cout << "��ȿ�� �� ���� �Է��ϼ���.\n";
        cin.clear(); cin.ignore(1000, '\n');
        return;
    }
    cout << "���� �뷮(ml): ";
    if (!(cin >> recipe.bottleVolume) || recipe.bottleVolume <= 0) {
        cout << "��ȿ�� ���� �뷮�� �Է��ϼ���.\n";
        cin.clear(); cin.ignore(1000, '\n');
        return;
    }
    cin.ignore();
    recipe.bottleProduct();

    // 5. ��ü ���� �α� ���
    showRecipeProcessLog(recipe);

    // 6. (����) ���� ����� CSV�� ����
    ofstream flog("last_recipe_process_log.txt");
    if (flog.is_open()) {
        for (const auto& line : recipe.getProcessLog()) {
            flog << line << "\n";
        }
        flog.close();
        cout << "[last_recipe_process_log.txt] ���Ϸ� ���� �αװ� ����Ǿ����ϴ�.\n";
    }
}

// ��ü ���� �α� ���
void RecipeManager::showRecipeProcessLog(const Recipe& recipe) const {
    cout << "\n=== ���� ���� �α� ===\n";
    for (const auto& line : recipe.getProcessLog()) {
        cout << line << "\n";
    }
}

// ������ ���� ������ UI
void RecipeManager::showRecipePage() {
    loadRecipesFromCSV("recipe_list.csv");
    RawMaterialManager rawMgr;
    rawMgr.loadMaterialsFromCSV("rawmaterial_dummy.csv");

    int choice;
    do {
        system("cls");
        // ���� ��� (������ �� ��)
        std::vector<std::string> infoLines = {
            "��ϵ� ������ ��: " + std::to_string(recipes.size())
        };
        std::vector<std::string> menu = {
            "[1] ������ ���",
            "[2] ������ �߰�",
            "[3] ������ ����",
            "[4] ������ ����",
            "[5] ������ �˻�",
            "[6] ������ ���� ����",
            "[0] ��������"
        };
        // ��ú��� ��� (UIUtils::drawDashboard ���)
        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        std::cout << "\n�Է� >> ";
        if (!(std::cin >> choice)) {
            std::cin.clear(); std::cin.ignore(1000, '\n');
            std::cout << "���ڸ� �Է��ϼ���.\n";
            continue;
        }
        std::cin.ignore();
        switch (choice) {
        case 1: listRecipes(); break;
        case 2: addRecipe(); break;
        case 3: updateRecipe(); break;
        case 4: deleteRecipe(); break;
        case 5: searchRecipe(); break;
        case 6: {
            std::cout << "\n������ ������ ������ ID �Է�: ";
            std::string id; std::getline(std::cin, id);
            runRecipeProcess(id, rawMgr);
            break;
        }
        case 0: std::cout << "�������� ���ư��ϴ�.\n"; break;
        default: std::cout << "�߸��� �Է��Դϴ�.\n"; break;
        }
        if (choice != 0) { std::cout << "\n����Ϸ��� Enter..."; std::cin.get(); }
    } while (choice != 0);
}