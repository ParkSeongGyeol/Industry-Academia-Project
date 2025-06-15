// -----------------------------------------------------------------------------
// SpiritManager.cpp
// 스피릿(증류주) 관리 기능 정의 파일 (레시피 연동 포함)
// -----------------------------------------------------------------------------

#include "SpiritManager.h"
#include "RecipeManager.h"
#include "Recipe.h"
#include "UIUtils.h"
#include "EncodingUtils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <vector>

using namespace std;

// ----------------------------- 상수 정의 -----------------------------
namespace {
    constexpr char SPIRIT_CSV[] = "spirit_dummy.csv";
}

// ----------------------------- 유틸리티 함수 -----------------------------

// 현재 시스템 날짜를 "YYYY-MM-DD" 형식으로 반환
string getCurrentDate() {
    time_t now = time(nullptr);
    tm t;
    localtime_s(&t, &now);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", &t);
    return string(buf);
}

// 안전한 double 입력 함수
double inputDouble(const string& prompt) {
    double val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cout << "숫자를 입력하세요.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// 안전한 int 입력 함수
int inputInt(const string& prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cout << "정수를 입력하세요.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// 안전한 string 입력 함수
string inputString(const string& prompt) {
    cout << prompt;
    string val;
    getline(cin, val);
    return val;
}

// ----------------------------- [1] 데이터 입출력 -----------------------------

// CSV에서 스피릿 목록 로드
void SpiritManager::loadSpiritsFromCSV(const string& filename) {
    spirits.clear();
    ifstream file(filename);
    applyCP949Locale(file);
    if (!file.is_open()) {
        cout << "[경고] 스피릿 CSV 파일을 열 수 없습니다: " << filename << endl;
        return;
    }
    string line;
    getline(file, line); // 헤더 스킵
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
        getline(iss, token, ','); // FirstDistillAmount (미사용)
        getline(iss, token, ','); // LastDistillAmount (미사용)
        getline(iss, token, ','); // MainDistillAmount (미사용)
        getline(iss, token, ','); s.setProductionDate(token);
        getline(iss, token, ','); s.setAlcoholPercentage(stod(token));
        // 나머지 필드는 빈 값으로 둠
        spirits.push_back(s);
    }
    file.close();
}

// 스피릿 목록을 CSV로 저장
void SpiritManager::saveSpiritsToCSV(const string& filename) {
    ofstream file(filename);
    applyCP949Locale(file);
    if (!file.is_open()) {
        cout << "[오류] 스피릿 CSV 저장 실패: " << filename << endl;
        return;
    }
    // 헤더
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

// ----------------------------- [2] 레시피 기반 스피릿 생산 -----------------------------
/**
 * 레시피 기반 스피릿 생산
 * - 레시피 목록을 보여주고, 사용자가 레시피 ID와 배치량, 증류 조건을 입력
 * - 증류 공정(수율, 분획비율 등) 입력 후, Spirit 객체로 기록
 */
void SpiritManager::produceSpiritByRecipe(RecipeManager& recipeMgr) {
    // 1. 레시피 목록 출력 및 선택
    recipeMgr.listRecipes();
    string recipeId = inputString("\n스피릿 생산에 사용할 레시피 ID 입력: ");
    Recipe recipe;
    if (!recipeMgr.getRecipeById(recipeId, recipe)) {
        cout << "해당 ID의 레시피를 찾을 수 없습니다.\n";
        UIUtils::pauseConsole();
        return;
    }

    // 2. 배치 생산량 입력
    double batchSize = inputDouble("생산할 배치량(kg): ");

    // 3. 증류 수율/분획비율 입력
    double yieldRate = inputDouble("증류 수율(0~1, 예: 0.8): ");
    double headPct = inputDouble("Head 비율(%): ");
    double tailPct = inputDouble("Tail 비율(%): ");

    // 4. 증류 공정 실행
    recipe.distillBatch(yieldRate, headPct, tailPct);

    // 5. Spirit 객체 생성 및 기록
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
    // 기타 필드는 필요시 추가 입력

    spirits.push_back(s);
    saveSpiritsToCSV(SPIRIT_CSV);

    cout << "레시피 기반 스피릿이 생성되었습니다. (ID: " << s.getId() << ")\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [3] 정보 요약/조회/출력 -----------------------------

// 전체 스피릿 요약 정보(개수/평균 도수 등) 반환
string SpiritManager::getSummary() {
    size_t count = spirits.size();
    double totalYield = 0, totalAbv = 0;
    for (const auto& s : spirits) {
        totalYield += s.getYieldLiters();
        totalAbv += s.getAlcoholPercentage();
    }
    string result = "스피릿: " + to_string(count) + "종";
    if (count > 0) {
        result += " / 평균 도수: " + to_string(static_cast<int>(totalAbv / count)) + "%";
    }
    return result;
}

// 대시보드/메뉴용 정보 요약 라인 반환
vector<string> SpiritManager::getPageInfoLines() {
    vector<string> lines;
    size_t count = spirits.size();
    double totalYield = 0;
    double totalAbv = 0;
    for (const auto& s : spirits) {
        totalYield += s.getYieldLiters();
        totalAbv += s.getAlcoholPercentage();
    }
    lines.push_back("등록된 스피릿 수: " + to_string(count) + "개");
    if (count > 0) {
        lines.push_back("총 생산량: " + to_string((int)totalYield) + "L");
        lines.push_back("평균 도수: " + to_string((int)(totalAbv / count)) + "%");
    } else {
        lines.push_back("총 생산량: 0L");
        lines.push_back("평균 도수: -");
    }
    return lines;
}

// 전체 스피릿 목록 출력
void SpiritManager::displaySpirits() {
    cout << "\n=== 스피릿 목록 ===\n";
    for (const auto& s : spirits) {
        cout << "ID: " << s.getId() << "\n"
            << "원재료 함량: " << s.getRawMaterialRatio() << "\n"
            << "배치 ID: " << s.getBatchId() << "\n"
            << "증류 횟수: " << s.getDistillationCount() << "\n"
            << "생산량: " << s.getYieldLiters() << "L\n"
            << "생산 날짜: " << s.getProductionDate() << "\n"
            << "도수: " << s.getAlcoholPercentage() << "%\n"
            << "--------------------------\n";
    }
    UIUtils::pauseConsole();
}

// ----------------------------- [4] CSV 내보내기 -----------------------------

// 스피릿 목록을 CSV로 내보내기
void SpiritManager::exportSpiritsToCSV(const string& filename) {
    saveSpiritsToCSV(filename);
    cout << "[ " << filename << " ] 파일로 저장 완료!\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [5] 입력/수정/삭제/검색 -----------------------------

// 스피릿 정보 추가 입력 처리
void SpiritManager::addSpirit() {
    Spirit spirit;
    string input;
    double doubleInput;
    int intInput;

    cout << "\n=== 스피릿 추가 ===\n";
    spirit.setId(inputString("ID: "));
    spirit.setStorageLocation(inputString("보관 장소: "));
    spirit.setTransferHistory(inputString("이동 기록: "));
    spirit.setProductionDate(inputString("생산 날짜 (YYYY-MM-DD): "));
    spirit.setAlcoholPercentage(inputDouble("도수(%): "));
    spirit.setYieldLiters(inputDouble("생산량(L): "));
    spirit.setRawMaterialRatio(inputString("원재료 함량: "));
    spirit.setFermentationDays(inputInt("발효 기간(일): "));
    spirit.setFirstCutTime(inputString("초류 끊은 시점: "));
    spirit.setLastCutTime(inputString("후류 그만 받은 시점: "));
    spirit.setDistillationCount(inputInt("증류 횟수: "));
    spirit.setParticleSize(inputString("입자 크기 (예: 미분말, 2mm 입자 등): "));
    spirit.setBatchId(inputString("재료 배치 ID: "));
    spirit.setCutYield(inputString("초류/후류/본류 수득량 요약: "));

    spirits.push_back(spirit);
    saveSpiritsToCSV(SPIRIT_CSV);
    cout << "스피릿 추가 완료!\n";
    UIUtils::pauseConsole();
}

// 특정 ID를 기준으로 스피릿 삭제
void SpiritManager::deleteSpirit() {
    string id = inputString("삭제할 스피릿 ID 입력: ");
    auto it = remove_if(spirits.begin(), spirits.end(), [&](const Spirit& s) {
        return s.getId() == id;
    });
    if (it != spirits.end()) {
        spirits.erase(it, spirits.end());
        saveSpiritsToCSV(SPIRIT_CSV);
        cout << "삭제 완료!\n";
    } else {
        cout << "해당 ID의 스피릿을 찾을 수 없습니다.\n";
    }
    UIUtils::pauseConsole();
}

// 스피릿 정보 수정
void SpiritManager::updateSpirit() {
    string id = inputString("\n수정할 스피릿 ID 입력: ");
    for (auto& s : spirits) {
        if (s.getId() == id) {
            cout << "=== 스피릿 정보 수정 ===\n";
            string input;
            cout << "보관 장소 (" << s.getStorageLocation() << "): ";
            getline(cin, input); if (!input.empty()) s.setStorageLocation(input);
            cout << "이동 기록 (" << s.getTransferHistory() << "): ";
            getline(cin, input); if (!input.empty()) s.setTransferHistory(input);
            cout << "생산 날짜 (" << s.getProductionDate() << "): ";
            getline(cin, input); if (!input.empty()) s.setProductionDate(input);
            cout << "도수 (%) (" << s.getAlcoholPercentage() << "): ";
            getline(cin, input); if (!input.empty()) s.setAlcoholPercentage(stod(input));
            cout << "생산량 (L) (" << s.getYieldLiters() << "): ";
            getline(cin, input); if (!input.empty()) s.setYieldLiters(stod(input));
            cout << "원재료 함량 (" << s.getRawMaterialRatio() << "): ";
            getline(cin, input); if (!input.empty()) s.setRawMaterialRatio(input);
            cout << "발효 기간 (일) (" << s.getFermentationDays() << "): ";
            getline(cin, input); if (!input.empty()) s.setFermentationDays(stoi(input));
            cout << "초류 끊은 시점 (" << s.getFirstCutTime() << "): ";
            getline(cin, input); if (!input.empty()) s.setFirstCutTime(input);
            cout << "후류 종료 시점 (" << s.getLastCutTime() << "): ";
            getline(cin, input); if (!input.empty()) s.setLastCutTime(input);
            cout << "증류 횟수 (" << s.getDistillationCount() << "): ";
            getline(cin, input); if (!input.empty()) s.setDistillationCount(stoi(input));
            cout << "입자 크기 (" << s.getParticleSize() << "): ";
            getline(cin, input); if (!input.empty()) s.setParticleSize(input);
            cout << "재료 배치 ID (" << s.getBatchId() << "): ";
            getline(cin, input); if (!input.empty()) s.setBatchId(input);
            cout << "초류/후류/본류 수득량 (" << s.getCutYield() << "): ";
            getline(cin, input); if (!input.empty()) s.setCutYield(input);
            saveSpiritsToCSV(SPIRIT_CSV);
            cout << "스피릿 수정 완료!\n";
            UIUtils::pauseConsole();
            return;
        }
    }
    cout << "해당 ID의 스피릿을 찾을 수 없습니다.\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [6] 메인 메뉴 루프 -----------------------------

// 스피릿 관리 메인 메뉴 (레시피 연동 포함)
void SpiritManager::showSpiritPage() {
    loadSpiritsFromCSV(SPIRIT_CSV);

    RecipeManager recipeMgr;
    recipeMgr.loadRecipesFromCSV("recipe_list.csv");

    int choice;
    do {
        system("cls");
        cout << "=== 스피릿 관리 메뉴 ===\n\n";
        vector<string> infoLines = getPageInfoLines();
        vector<string> menu = {
            "[1] 스피릿 목록 보기",
            "[2] 스피릿 추가",
            "[3] 스피릿 삭제",
            "[4] 스피릿 수정",
            "[5] CSV로 저장",
            "[6] 레시피 기반 스피릿 생산",
            "[0] 메인으로 돌아가기"
        };
        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        cout << "\n입력 >> ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: displaySpirits(); break;
        case 2: addSpirit(); break;
        case 3: deleteSpirit(); break;
        case 4: updateSpirit(); break;
        case 5: exportSpiritsToCSV(SPIRIT_CSV); break;
        case 6: produceSpiritByRecipe(recipeMgr); break;
        case 0: cout << "메인으로 돌아갑니다...\n"; break;
        default: cout << "잘못된 입력입니다.\n"; break;
        }

        if (choice != 0) {
            cout << "\n계속하려면 Enter를 누르세요...";
            cin.get();
        }
    } while (choice != 0);
}