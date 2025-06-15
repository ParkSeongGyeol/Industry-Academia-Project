// -----------------------------------------------------------------------------
// [BatchManager] 발효 배치 관리 기능 정의 파일
// -----------------------------------------------------------------------------
#include "BatchManager.h"
#include "RecipeManager.h"
#include "Recipe.h"
#include "UIUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>

using namespace std;

// ----------------------------- [유틸리티] -----------------------------

namespace {
    constexpr int CSV_FIELD_COUNT = 11;
    constexpr char BATCH_CSV[] = "batch_dummy.csv";
}

// ----------------------------- [FermentationBatch 구현] -----------------------------

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

// CSV 직렬화
string FermentationBatch::toCSV() const {
    ostringstream oss;
    oss << batch_id << "," << yeast_type << "," << ingredient_info << "," << start_date << ","
        << duration_hours << "," << temperature << "," << amount_liters << "," << end_date << ","
        << expiry_date << "," << particle_size << "," << batch_finish_date;
    return oss.str();
}

// CSV 역직렬화
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

// ----------------------------- [1] 데이터 입출력 -----------------------------

void BatchManager::loadBatchesFromCSV(const string& filename) {
    batches.clear();
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[경고] 배치 CSV 파일을 열 수 없습니다: " << filename << endl;
        return;
    }
    string line;
    getline(file, line); // 헤더 스킵
    while (getline(file, line)) {
        if (line.empty()) continue;
        batches.push_back(FermentationBatch::fromCSV(line));
    }
    file.close();
}

void BatchManager::saveBatchesToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "[오류] 파일 저장 실패: " << filename << endl;
        return;
    }
    file << "BatchID,YeastType,IngredientComposition,StartDate,FermentationDuration,Temperature,AmountLiters,EndDate,ExpiryDate,ParticleSize,BatchFinishDate\n";
    for (const auto& b : batches) {
        file << b.toCSV() << "\n";
    }
    file.close();
}

// ----------------------------- [2] 레시피 기반 배치 생산 -----------------------------

void BatchManager::produceBatchByRecipe(RecipeManager& recipeMgr) {
    recipeMgr.listRecipes();
    string recipeId;
    cout << "\n사용할 레시피 ID 입력: ";
    cin >> recipeId;
    Recipe recipe;
    if (!recipeMgr.getRecipeById(recipeId, recipe)) {
        cout << "해당 ID의 레시피를 찾을 수 없습니다.\n";
        return;
    }
    double batchSize;
    cout << "생산할 배치량(리터): ";
    cin >> batchSize;
    // 원재료 재고 검증 등은 RawMaterialManager에서 처리
    // 여기서는 레시피 정보로 배치 생성
    FermentationBatch b;
    b.setBatchId("B" + to_string(batches.size() + 1));
    b.setYeastType(recipe.yeastType);
    b.setIngredientInfo("레시피 기반"); // 실제로는 recipe.rawMaterialRatio 등 활용
    b.setStartDate("2025-01-01"); // 실제로는 현재 날짜 등
    b.setDurationHours(recipe.fermentationHours);
    b.setTemperature(recipe.fermentationTemp);
    b.setAmountLiters(batchSize);
    b.setEndDate("2025-01-04");
    b.setExpiryDate("2026-01-01");
    b.setParticleSize("중");
    b.setBatchFinishDate("2025-01-04");
    batches.push_back(b);
    saveBatchesToCSV(BATCH_CSV);
    cout << "레시피 기반 배치가 생성되었습니다.\n";
}

// ----------------------------- [3] 정보 요약/조회/출력 -----------------------------

string BatchManager::getSummary() {
    return "배치: " + to_string(batches.size()) + "개";
}

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
    set<string> locations = { "탱크" };
    vector<string> lines;
    lines.push_back("전체 배치 수: " + to_string(total) + "개");
    lines.push_back("상태 분포: 발효 중 " + to_string(fermenting) + "개 / 완료 " + to_string(completed) + "개");
    lines.push_back("평균 발효 시간: " + to_string(static_cast<int>(avg)) + "시간");
    string locStr = "저장 위치: ";
    for (const auto& l : locations) locStr += l + " ";
    lines.push_back(locStr);
    return lines;
}

void BatchManager::showBatchList() {
    cout << "\n=== 발효 배치 목록 ===\n";
    for (const auto& b : batches) {
        cout << "ID: " << b.getBatchId() << " | 효모: " << b.getYeastType() << " | 시작일: " << b.getStartDate() << " | 양: " << b.getAmountLiters() << "L\n";
    }
    UIUtils::pauseConsole();
}

void BatchManager::showBatchDetail() {
    cin.ignore();
    string id;
    cout << "상세 조회할 배치 ID 입력: "; getline(cin, id);
    for (const auto& b : batches) {
        if (b.getBatchId() == id) {
            cout << "배치 ID: " << b.getBatchId() << "\n"
                << "효모: " << b.getYeastType() << "\n"
                << "원재료 조성: " << b.getIngredientInfo() << "\n"
                << "시작일: " << b.getStartDate() << "\n"
                << "발효 시간: " << b.getDurationHours() << "시간\n"
                << "온도: " << b.getTemperature() << "°C\n"
                << "양 (리터): " << b.getAmountLiters() << "L\n"
                << "완료일: " << b.getEndDate() << "\n"
                << "유통기한: " << b.getExpiryDate() << "\n"
                << "입자 크기: " << b.getParticleSize() << "\n"
                << "배치 종료일: " << b.getBatchFinishDate() << "\n";
            UIUtils::pauseConsole();
            return;
        }
    }
    cout << "해당 ID의 배치를 찾을 수 없습니다.\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [4] CSV 내보내기 -----------------------------

void BatchManager::exportBatchesToCSV() {
    saveBatchesToCSV(BATCH_CSV);
    cout << "\n배치 목록이 CSV 파일로 저장되었습니다.\n";
    cout << "저장 경로: " << filesystem::current_path() / BATCH_CSV << "\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [5] 입력/수정/삭제/검색 -----------------------------

void BatchManager::addBatch() {
    FermentationBatch b;
    string input;
    double dInput;
    int iInput;
    cin.ignore();
    cout << "\n=== 새 발효 배치 추가 ===\n";
    cout << "배치 ID: "; getline(cin, input); b.setBatchId(input);
    cout << "효모 종류: "; getline(cin, input); b.setYeastType(input);
    cout << "원재료 조성 (예: RM101:60%): "; getline(cin, input); b.setIngredientInfo(input);
    cout << "시작일 (YYYY-MM-DD): "; getline(cin, input); b.setStartDate(input);
    cout << "발효 시간(시간): "; cin >> iInput; b.setDurationHours(iInput);
    cout << "온도(°C): "; cin >> dInput; b.setTemperature(dInput);
    cout << "양(리터): "; cin >> dInput; b.setAmountLiters(dInput);
    cin.ignore();
    cout << "완료일 (YYYY-MM-DD): "; getline(cin, input); b.setEndDate(input);
    cout << "유통기한 (YYYY-MM-DD): "; getline(cin, input); b.setExpiryDate(input);
    cout << "입자 크기(소/중/대): "; getline(cin, input); b.setParticleSize(input);
    cout << "배치 종료일 (YYYY-MM-DD): "; getline(cin, input); b.setBatchFinishDate(input);

    batches.push_back(b);
    saveBatchesToCSV(BATCH_CSV);
    cout << "발효 배치가 추가되었습니다.\n";
    UIUtils::pauseConsole();
}

void BatchManager::updateBatch() {
    cin.ignore();
    string id;
    cout << "수정할 배치 ID 입력: "; getline(cin, id);
    for (auto& b : batches) {
        if (b.getBatchId() == id) {
            string input;
            double dInput;
            int iInput;
            cout << "효모 종류 (" << b.getYeastType() << "): "; getline(cin, input); if (!input.empty()) b.setYeastType(input);
            cout << "원재료 조성 (" << b.getIngredientInfo() << "): "; getline(cin, input); if (!input.empty()) b.setIngredientInfo(input);
            cout << "시작일 (" << b.getStartDate() << "): "; getline(cin, input); if (!input.empty()) b.setStartDate(input);
            cout << "발효 시간 (" << b.getDurationHours() << "): "; getline(cin, input); if (!input.empty()) b.setDurationHours(stoi(input));
            cout << "온도 (" << b.getTemperature() << "): "; getline(cin, input); if (!input.empty()) b.setTemperature(stod(input));
            cout << "양 (" << b.getAmountLiters() << "): "; getline(cin, input); if (!input.empty()) b.setAmountLiters(stod(input));
            cout << "완료일 (" << b.getEndDate() << "): "; getline(cin, input); if (!input.empty()) b.setEndDate(input);
            cout << "유통기한 (" << b.getExpiryDate() << "): "; getline(cin, input); if (!input.empty()) b.setExpiryDate(input);
            cout << "입자 크기 (" << b.getParticleSize() << "): "; getline(cin, input); if (!input.empty()) b.setParticleSize(input);
            cout << "배치 종료일 (" << b.getBatchFinishDate() << "): "; getline(cin, input); if (!input.empty()) b.setBatchFinishDate(input);
            saveBatchesToCSV(BATCH_CSV);
            cout << "수정 완료.\n";
            UIUtils::pauseConsole();
            return;
        }
    }
    cout << "해당 ID의 배치를 찾을 수 없습니다.\n";
    UIUtils::pauseConsole();
}

void BatchManager::deleteBatch() {
    cin.ignore();
    string id;
    cout << "삭제할 배치 ID 입력: "; getline(cin, id);
    auto it = remove_if(batches.begin(), batches.end(), [&](const FermentationBatch& b) { return b.getBatchId() == id; });
    if (it != batches.end()) {
        batches.erase(it, batches.end());
        saveBatchesToCSV(BATCH_CSV);
        cout << "삭제 완료.\n";
    } else {
        cout << "해당 ID의 배치를 찾을 수 없습니다.\n";
    }
    UIUtils::pauseConsole();
}

void BatchManager::searchBatch() {
    cin.ignore();
    string id;
    cout << "조회할 배치 ID 입력: "; getline(cin, id);
    for (const auto& b : batches) {
        if (b.getBatchId() == id) {
            showBatchDetail();
            return;
        }
    }
    cout << "해당 ID의 배치를 찾을 수 없습니다.\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [6] 발효 예측 -----------------------------

void BatchManager::predictBatchFermentation() {
    cin.ignore();
    string id;
    cout << "예측할 배치 ID 입력: "; getline(cin, id);
    for (const auto& b : batches) {
        if (b.getBatchId() == id) {
            double score = (b.getTemperature() * 0.8) + (b.getDurationHours() * 1.5);
            if (b.getParticleSize() == "소") score *= 1.2;
            else if (b.getParticleSize() == "대") score *= 0.8;
            score = min(100.0, max(0.0, score));
            cout << "\n예측된 발효 정도: " << score << "% 입니다.\n";
            UIUtils::pauseConsole();
            return;
        }
    }
    cout << "해당 ID의 배치를 찾을 수 없습니다.\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [7] 메인 메뉴 루프 -----------------------------

void BatchManager::showBatchPage() {
    loadBatchesFromCSV(BATCH_CSV);
    int choice;
    do {
        UIUtils::clearConsole();
        vector<string> infoLines = getPageInfoLines();
        vector<string> menu = {
            "[1] 배치 목록 조회",
            "[2] 배치 상세 조회",
            "[3] 새 배치 추가",
            "[4] 배치 정보 수정",
            "[5] 배치 삭제",
            "[6] 배치 검색",
            "[7] 발효 정도 예측",
            "[8] 레시피 기반 배치 생산",
            "[9] 배치 CSV 내보내기",
            "[0] 메인으로 돌아가기"
        };
        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        cout << "\n입력 >> ";
        cin >> choice;
        switch (choice) {
        case 1: showBatchList(); break;
        case 2: showBatchDetail(); break;
        case 3: addBatch(); break;
        case 4: updateBatch(); break;
        case 5: deleteBatch(); break;
        case 6: searchBatch(); break;
        case 7: predictBatchFermentation(); break;
        case 8: produceBatchByRecipe(/* RecipeManager 연동 */); break;
        case 9: exportBatchesToCSV(); break;
        case 0: cout << "메인 메뉴로 돌아갑니다.\n"; break;
        default: UIUtils::printError("잘못된 입력입니다."); UIUtils::pauseConsole(); break;
        }
    } while (choice != 0);
}
