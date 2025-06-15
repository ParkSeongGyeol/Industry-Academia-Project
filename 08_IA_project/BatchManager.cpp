// -----------------------------------------------------------------------------
// BatchManager.cpp
// 발효 배치 관리 기능 정의 파일 (레시피 연동 포함)
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

// ----------------------------- 상수 정의 -----------------------------
namespace {
    constexpr int CSV_FIELD_COUNT = 11;
    constexpr char BATCH_CSV[] = "batch_dummy.csv";
    constexpr char RECIPE_CSV[] = "recipe_list.csv";
    constexpr char RAW_MATERIAL_CSV[] = "rawmaterial_dummy.csv";
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

// CSV 파일에서 배치 목록을 읽어와 batches 벡터에 저장
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

// batches 벡터의 모든 배치 정보를 지정한 CSV 파일로 저장
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
/**
 * 레시피 기반 배치 생산
 * - 레시피 목록을 보여주고, 사용자가 레시피 ID와 배치량을 입력
 * - 원재료 재고 검증 및 차감은 RawMaterialManager에서 처리
 * - 배치 정보는 FermentationBatch로 기록
 */
void BatchManager::produceBatchByRecipe(RecipeManager& recipeMgr, RawMaterialManager& rawMgr) {
    recipeMgr.listRecipes();
    string recipeId = inputString("\n사용할 레시피 ID 입력: ");
    Recipe recipe;
    if (!recipeMgr.getRecipeById(recipeId, recipe)) {
        cout << "해당 ID의 레시피를 찾을 수 없습니다.\n";
        UIUtils::pauseConsole();
        return;
    }
    double batchSize = inputDouble("생산할 배치량(kg): ");
    if (!recipe.validateRawMaterialStock(rawMgr, batchSize)) {
        cout << "재고가 부족하여 배치 생산이 불가합니다.\n";
        UIUtils::pauseConsole();
        return;
    }
    string batchId = recipe.produceBatch(rawMgr, batchSize);
    rawMgr.saveMaterialsToCSV("rawmaterial_dummy.csv"); // 원재료 차감 후 저장

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
    b.setAmountLiters(recipe.batchOutput); // 레시피에서 출력량 계산
    b.setEndDate(getCurrentDate());
    b.setExpiryDate("-");
    b.setParticleSize("중");
    b.setBatchFinishDate(getCurrentDate());

    batches.push_back(b);
    saveBatchesToCSV("batch_dummy.csv"); // 배치 정보 저장

    cout << "레시피 기반 배치가 생성되었습니다. (배치ID: " << batchId << ")\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [3] 정보 요약/조회/출력 -----------------------------

// 전체 배치 요약 정보(개수 등) 반환
string BatchManager::getSummary() {
    return "배치: " + to_string(batches.size()) + "개";
}

// 대시보드/메뉴용 정보 요약 라인 반환
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

// 전체 배치 목록 출력
void BatchManager::showBatchList() {
    cout << "\n=== 발효 배치 목록 ===\n";
    for (const auto& b : batches) {
        cout << "ID: " << b.getBatchId() << " | 효모: " << b.getYeastType() << " | 시작일: " << b.getStartDate() << " | 양: " << b.getAmountLiters() << "L\n";
    }
    UIUtils::pauseConsole();
}

// 배치 상세 정보 출력
void BatchManager::showBatchDetail() {
    cin.ignore();
    string id = inputString("상세 조회할 배치 ID 입력: ");
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

// 발효 중인 배치 목록 출력 (예시)
void BatchManager::showFermentingBatches() {
    cout << "\n=== 발효 중인 배치 목록 ===\n";
    for (const auto& b : batches) {
        if (b.getDurationHours() < 72) {
            cout << "ID: " << b.getBatchId() << " | 효모: " << b.getYeastType() << " | 시작일: " << b.getStartDate() << "\n";
        }
    }
    UIUtils::pauseConsole();
}

// ----------------------------- [4] CSV 내보내기 -----------------------------

// 배치 목록을 CSV로 내보내기
void BatchManager::exportBatchesToCSV() {
    saveBatchesToCSV(BATCH_CSV);
    cout << "\n배치 목록이 CSV 파일로 저장되었습니다.\n";
    cout << "저장 경로: " << filesystem::current_path() / BATCH_CSV << "\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [5] 입력/수정/삭제/검색 -----------------------------

// 배치 신규 추가 (수동 입력)
void BatchManager::addBatch() {
    FermentationBatch b;
    string input;
    double dInput;
    int iInput;
    cin.ignore();
    cout << "\n=== 새 발효 배치 추가 ===\n";
    b.setBatchId(inputString("배치 ID: "));
    b.setYeastType(inputString("효모 종류: "));
    b.setIngredientInfo(inputString("원재료 조성 (예: RM101:60%): "));
    b.setStartDate(inputString("시작일 (YYYY-MM-DD): "));
    b.setDurationHours(inputInt("발효 시간(시간): "));
    b.setTemperature(inputDouble("온도(°C): "));
    b.setAmountLiters(inputDouble("양(리터): "));
    b.setEndDate(inputString("완료일 (YYYY-MM-DD): "));
    b.setExpiryDate(inputString("유통기한 (YYYY-MM-DD): "));
    b.setParticleSize(inputString("입자 크기(소/중/대): "));
    b.setBatchFinishDate(inputString("배치 종료일 (YYYY-MM-DD): "));

    batches.push_back(b);
    saveBatchesToCSV(BATCH_CSV);
    cout << "발효 배치가 추가되었습니다.\n";
    UIUtils::pauseConsole();
}

// 배치 정보 수정
void BatchManager::updateBatch() {
    cin.ignore();
    string id = inputString("수정할 배치 ID 입력: ");
    for (auto& b : batches) {
        if (b.getBatchId() == id) {
            string input;
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

// 배치 삭제
void BatchManager::deleteBatch() {
    cin.ignore();
    string id = inputString("삭제할 배치 ID 입력: ");
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

// 배치 검색(상세 조회)
void BatchManager::searchBatch() {
    cin.ignore();
    string id = inputString("조회할 배치 ID 입력: ");
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

// 간단한 발효 정도 예측(예시)
void BatchManager::predictBatchFermentation() {
    cin.ignore();
    string id = inputString("예측할 배치 ID 입력: ");
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

// 발효 배치 관리 메인 메뉴 (레시피 연동 포함)
void BatchManager::showBatchPage(RecipeManager& recipeMgr, RawMaterialManager& rawMgr) {
    loadBatchesFromCSV("batch_dummy.csv");
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
        case 8: produceBatchByRecipe(recipeMgr, rawMgr); break;
        case 9: exportBatchesToCSV(); break;
        case 0: cout << "메인 메뉴로 돌아갑니다.\n"; break;
        default: UIUtils::printError("잘못된 입력입니다."); UIUtils::pauseConsole(); break;
        }
    } while (choice != 0);
}