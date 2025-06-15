#include "RecipeManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

// CSV에서 레시피 목록 불러오기
void RecipeManager::loadRecipesFromCSV(const string& filename) {
    recipes.clear();
    ifstream file(filename);
    if (!file.is_open()) return;
    string line;
    getline(file, line); // 헤더 스킵
    while (getline(file, line)) {
        if (line.empty()) continue;
        recipes.push_back(Recipe::fromCSV(line));
    }
    file.close();
}

// 레시피 목록을 CSV로 저장
void RecipeManager::saveRecipesToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) return;
    // 헤더
    file << "ID,Name,RawMaterialRatio,YeastType,FermentationTemp,FermentationHours,DistillationABV,DistillationCount,OakType,AgingMonths,BottledName,BottleCount,BottleVolume,BottlePrice\n";
    for (const auto& r : recipes) {
        file << r.toCSV() << "\n";
    }
    file.close();
}

// 레시피 목록 출력
void RecipeManager::listRecipes() const {
    cout << "\n=== 등록된 레시피 목록 ===\n";
    for (const auto& r : recipes) {
        cout << "ID: " << r.recipeId << " | 이름: " << r.name << "\n";
    }
}

// ID로 레시피 찾기
bool RecipeManager::getRecipeById(const string& id, Recipe& out) const {
    for (const auto& r : recipes) {
        if (r.recipeId == id) {
            out = r;
            return true;
        }
    }
    return false;
}

// 레시피 추가
void RecipeManager::addRecipe() {
    Recipe r;
    cout << "\n=== 레시피 추가 ===\n";
    cout << "ID: "; getline(cin, r.recipeId);
    cout << "이름: "; getline(cin, r.name);

    // 원재료 비율 입력
    cout << "원재료 비율 입력 (예: 보리:60;호밀:10;물:30): ";
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

    cout << "효모: "; getline(cin, r.yeastType);
    cout << "발효 온도(℃): "; cin >> r.fermentationTemp; cin.ignore();
    cout << "발효 시간(시간): "; cin >> r.fermentationHours; cin.ignore();
    cout << "증류 도수(%): "; cin >> r.distillationABV; cin.ignore();
    cout << "증류 횟수: "; cin >> r.distillationCount; cin.ignore();
    cout << "오크통 종류: "; getline(cin, r.oakType);
    cout << "숙성 기간(월): "; cin >> r.agingMonths; cin.ignore();
    cout << "병입 제품명: "; getline(cin, r.bottledName);
    cout << "병 수량: "; cin >> r.bottleCount; cin.ignore();
    cout << "병당 용량(ml): "; cin >> r.bottleVolume; cin.ignore();
    cout << "병당 가격: "; cin >> r.bottlePrice; cin.ignore();

    recipes.push_back(r);
    saveRecipesToCSV("recipe_list.csv");
    cout << "레시피가 추가되었습니다.\n";
}

// 레시피 수정
void RecipeManager::updateRecipe() {
    cout << "\n수정할 레시피 ID 입력: ";
    string id; getline(cin, id);
    for (auto& r : recipes) {
        if (r.recipeId == id) {
            cout << "새 이름(" << r.name << "): ";
            string name; getline(cin, name);
            if (!name.empty()) r.name = name;
            // (필요시 나머지 필드도 수정 입력)
            saveRecipesToCSV("recipe_list.csv");
            cout << "수정 완료.\n";
            return;
        }
    }
    cout << "해당 ID의 레시피를 찾을 수 없습니다.\n";
}

// 레시피 삭제
void RecipeManager::deleteRecipe() {
    cout << "\n삭제할 레시피 ID 입력: ";
    string id; getline(cin, id);
    auto it = remove_if(recipes.begin(), recipes.end(), [&](const Recipe& r) { return r.recipeId == id; });
    if (it != recipes.end()) {
        recipes.erase(it, recipes.end());
        saveRecipesToCSV("recipe_list.csv");
        cout << "삭제 완료.\n";
    } else {
        cout << "해당 ID의 레시피를 찾을 수 없습니다.\n";
    }
}

// 레시피 검색
void RecipeManager::searchRecipe() const {
    cout << "\n검색할 레시피 이름 입력: ";
    string name; getline(cin, name);
    bool found = false;
    for (const auto& r : recipes) {
        if (r.name.find(name) != string::npos) {
            cout << "ID: " << r.recipeId << " | 이름: " << r.name << "\n";
            found = true;
        }
    }
    if (!found) cout << "검색 결과가 없습니다.\n";
}

// 레시피 관리 페이지 UI
void RecipeManager::showRecipePage() {
    loadRecipesFromCSV("recipe_list.csv");
    int choice;
    do {
        system("cls");
        cout << "=== 레시피 관리 메뉴 ===\n";
        cout << "[1] 레시피 목록\n[2] 레시피 추가\n[3] 레시피 수정\n[4] 레시피 삭제\n[5] 레시피 검색\n[0] 메인으로\n";
        cout << "입력 >> ";
        cin >> choice; cin.ignore();
        switch (choice) {
        case 1: listRecipes(); break;
        case 2: addRecipe(); break;
        case 3: updateRecipe(); break;
        case 4: deleteRecipe(); break;
        case 5: searchRecipe(); break;
        case 0: cout << "메인으로 돌아갑니다.\n"; break;
        default: cout << "잘못된 입력입니다.\n"; break;
        }
        if (choice != 0) { cout << "\n계속하려면 Enter..."; cin.get(); }
    } while (choice != 0);
}