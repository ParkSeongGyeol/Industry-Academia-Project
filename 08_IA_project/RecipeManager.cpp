#include "RecipeManager.h"
#include "RawMaterialManager.h"
#include "UIUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;

// CSV에서 레시피 목록 불러오기
void RecipeManager::loadRecipesFromCSV(const string& filename) {
    recipes.clear();
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[경고] 레시피 CSV 파일을 열 수 없습니다: " << filename << endl;
        return;
    }
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
    if (!file.is_open()) {
        cout << "[오류] 레시피 CSV 저장 실패: " << filename << endl;
        return;
    }
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
    if (recipes.empty()) {
        cout << "등록된 레시피가 없습니다.\n";
        return;
    }
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
    cin.ignore();
    Recipe r;
    cout << "\n=== 레시피 추가 ===\n";
    cout << "ID: "; getline(cin, r.recipeId);
    if (r.recipeId.empty()) { cout << "ID는 필수입니다.\n"; return; }
    cout << "이름: "; getline(cin, r.name);

    // 원재료 비율 입력
    cout << "원재료 비율 입력 (예: 보리:60;호밀:10;물:30): ";
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
        cout << "원재료 비율의 합이 100%가 아닙니다.\n";
        return;
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
    cin.ignore();
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
    cin.ignore();
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
    cin.ignore();
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

// 실제 공정 실행 및 추적
void RecipeManager::runRecipeProcess(const std::string& recipeId, RawMaterialManager& rawMgr) {
    Recipe recipe;
    if (!getRecipeById(recipeId, recipe)) {
        cout << "해당 ID의 레시피를 찾을 수 없습니다.\n";
        return;
    }

    double batchSize;
    cout << "생산할 배치량(kg): ";
    if (!(cin >> batchSize) || batchSize <= 0) {
        cout << "유효한 배치량을 입력하세요.\n";
        cin.clear(); cin.ignore(1000, '\n');
        return;
    }
    cin.ignore();

    // 1. 원재료 검증 및 배치 생산
    if (!recipe.validateRawMaterialStock(rawMgr, batchSize)) {
        cout << "재고가 부족하여 배치 생산이 불가합니다.\n";
        return;
    }
    recipe.produceBatch(rawMgr, batchSize);
    rawMgr.saveMaterialsToCSV("rawmaterial_dummy.csv");

    // 2. 증류 (수율/분획비율 입력)
    double yieldRate = 0.8, headPct = 10, tailPct = 10;
    cout << "증류 수율(0~1, 기본 0.8): ";
    if (!(cin >> yieldRate) || yieldRate <= 0 || yieldRate > 1) {
        cout << "기본값 0.8 사용\n"; yieldRate = 0.8;
        cin.clear(); cin.ignore(1000, '\n');
    }
    cout << "Head 비율(%): ";
    if (!(cin >> headPct) || headPct < 0 || headPct > 100) {
        cout << "기본값 10 사용\n"; headPct = 10;
        cin.clear(); cin.ignore(1000, '\n');
    }
    cout << "Tail 비율(%): ";
    if (!(cin >> tailPct) || tailPct < 0 || tailPct > 100) {
        cout << "기본값 10 사용\n"; tailPct = 10;
        cin.clear(); cin.ignore(1000, '\n');
    }
    cin.ignore();
    recipe.distillBatch(yieldRate, headPct, tailPct);

    // 3. 숙성 (증발률 입력)
    double evaporationRate = 2.5;
    cout << "숙성 증발률(%): ";
    if (!(cin >> evaporationRate) || evaporationRate < 0) {
        cout << "기본값 2.5 사용\n"; evaporationRate = 2.5;
        cin.clear(); cin.ignore(1000, '\n');
    }
    cin.ignore();
    recipe.ageSpirit(evaporationRate);

    // 4. 병입 (병 수/용량 입력)
    cout << "병 수: ";
    if (!(cin >> recipe.bottleCount) || recipe.bottleCount <= 0) {
        cout << "유효한 병 수를 입력하세요.\n";
        cin.clear(); cin.ignore(1000, '\n');
        return;
    }
    cout << "병당 용량(ml): ";
    if (!(cin >> recipe.bottleVolume) || recipe.bottleVolume <= 0) {
        cout << "유효한 병당 용량을 입력하세요.\n";
        cin.clear(); cin.ignore(1000, '\n');
        return;
    }
    cin.ignore();
    recipe.bottleProduct();

    // 5. 전체 공정 로그 출력
    showRecipeProcessLog(recipe);

    // 6. (선택) 공정 결과를 CSV로 저장
    ofstream flog("last_recipe_process_log.txt");
    if (flog.is_open()) {
        for (const auto& line : recipe.getProcessLog()) {
            flog << line << "\n";
        }
        flog.close();
        cout << "[last_recipe_process_log.txt] 파일로 공정 로그가 저장되었습니다.\n";
    }
}

// 전체 공정 로그 출력
void RecipeManager::showRecipeProcessLog(const Recipe& recipe) const {
    cout << "\n=== 생산 공정 로그 ===\n";
    for (const auto& line : recipe.getProcessLog()) {
        cout << line << "\n";
    }
}

// 레시피 관리 페이지 UI
void RecipeManager::showRecipePage() {
    loadRecipesFromCSV("recipe_list.csv");
    RawMaterialManager rawMgr;
    rawMgr.loadMaterialsFromCSV("rawmaterial_dummy.csv");

    int choice;
    do {
        system("cls");
        // 정보 요약 (레시피 수 등)
        std::vector<std::string> infoLines = {
            "등록된 레시피 수: " + std::to_string(recipes.size())
        };
        std::vector<std::string> menu = {
            "[1] 레시피 목록",
            "[2] 레시피 추가",
            "[3] 레시피 수정",
            "[4] 레시피 삭제",
            "[5] 레시피 검색",
            "[6] 레시피 공정 실행",
            "[0] 메인으로"
        };
        // 대시보드 출력 (UIUtils::drawDashboard 사용)
        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        std::cout << "\n입력 >> ";
        if (!(std::cin >> choice)) {
            std::cin.clear(); std::cin.ignore(1000, '\n');
            std::cout << "숫자를 입력하세요.\n";
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
            std::cout << "\n공정을 실행할 레시피 ID 입력: ";
            std::string id; std::getline(std::cin, id);
            runRecipeProcess(id, rawMgr);
            break;
        }
        case 0: std::cout << "메인으로 돌아갑니다.\n"; break;
        default: std::cout << "잘못된 입력입니다.\n"; break;
        }
        if (choice != 0) { std::cout << "\n계속하려면 Enter..."; std::cin.get(); }
    } while (choice != 0);
}