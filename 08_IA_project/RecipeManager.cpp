#include "RecipeManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

// CSV���� ������ ��� �ҷ�����
void RecipeManager::loadRecipesFromCSV(const string& filename) {
    recipes.clear();
    ifstream file(filename);
    if (!file.is_open()) return;
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
    if (!file.is_open()) return;
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
    Recipe r;
    cout << "\n=== ������ �߰� ===\n";
    cout << "ID: "; getline(cin, r.recipeId);
    cout << "�̸�: "; getline(cin, r.name);

    // ����� ���� �Է�
    cout << "����� ���� �Է� (��: ����:60;ȣ��:10;��:30): ";
    string ratioStr; getline(cin, ratioStr);
    istringstream iss(ratioStr);
    string token;
    while (getline(iss, token, ';')) {
        auto pos = token.find(':');
        if (pos != string::npos) {
            string mat = token.substr(0, pos);
            double val = stod(token.substr(pos + 1));
            r.rawMaterialRatio[mat] = val;
        }
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

// ������ ���� ������ UI
void RecipeManager::showRecipePage() {
    loadRecipesFromCSV("recipe_list.csv");
    int choice;
    do {
        system("cls");
        cout << "=== ������ ���� �޴� ===\n";
        cout << "[1] ������ ���\n[2] ������ �߰�\n[3] ������ ����\n[4] ������ ����\n[5] ������ �˻�\n[0] ��������\n";
        cout << "�Է� >> ";
        cin >> choice; cin.ignore();
        switch (choice) {
        case 1: listRecipes(); break;
        case 2: addRecipe(); break;
        case 3: updateRecipe(); break;
        case 4: deleteRecipe(); break;
        case 5: searchRecipe(); break;
        case 0: cout << "�������� ���ư��ϴ�.\n"; break;
        default: cout << "�߸��� �Է��Դϴ�.\n"; break;
        }
        if (choice != 0) { cout << "\n����Ϸ��� Enter..."; cin.get(); }
    } while (choice != 0);
}