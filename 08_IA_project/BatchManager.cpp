#include "BatchManager.h"
#include "FileExporter.h"
#include "UIUtils.h"
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <set>

using namespace std;

//  전체 배치 수 요약을 반환
string BatchManager::getSummary() {
    auto batches = getDummyBatches();
    return "배치 수: " + to_string(batches.size()) + "개";
}


//  배치 정보를 CSV 형식 문자열로 변환
string FermentationBatch::toCSV() const {
    return getBatchId() + "," + getStartDate() + "," + getParticleSize() + "," +
        getYeastType() + "," + getIngredients() + "," +
        to_string(getAmountLiters()) + "," +
        to_string(getTemperature()) + "," +
        to_string(getHumidity()) + "," +
        to_string(getDurationHours());
}


//  예시용 더미 배치 목록 반환
vector<FermentationBatch> BatchManager::getDummyBatches() {
    FermentationBatch b1, b2;

    b1.setBatchId("B001");
    b1.setStartDate("2025-03-01");
    b1.setParticleSize("medium");
    b1.setYeastType("saccharomyces");
    b1.setIngredients("rice, barley");
    b1.setAmountLiters(100.0);
    b1.setTemperature(22.5);
    b1.setHumidity(65);
    b1.setDurationHours(72);

    b2.setBatchId("B002");
    b2.setStartDate("2025-03-05");
    b2.setParticleSize("fine");
    b2.setYeastType("brettanomyces");
    b2.setIngredients("corn, wheat");
    b2.setAmountLiters(85.0);
    b2.setTemperature(21.0);
    b2.setHumidity(70);
    b2.setDurationHours(48);

    return { b1, b2 };
}


//  배치 목록 출력
void BatchManager::displayBatches(const vector<FermentationBatch>& batches) {
    cout << "\n=== 발효 배치 목록 ===\n";
    for (const auto& b : batches) {
        cout << "배치 ID: " << b.getBatchId() << "\n"
            << "시작일: " << b.getStartDate() << "\n"
            << "입자 크기: " << b.getParticleSize() << "\n"
            << "효모 종류: " << b.getYeastType() << "\n"
            << "사용된 재료: " << b.getIngredients() << "\n"
            << "양 (리터): " << b.getAmountLiters() << "L\n"
            << "온도: " << b.getTemperature() << "°C\n"
            << "습도: " << b.getHumidity() << "%\n"
            << "발효 시간: " << b.getDurationHours() << "시간\n"
            << "------------------------\n";
    }
    UIUtils::pauseConsole();
}



//  새 배치 추가
void BatchManager::addBatch() {
    FermentationBatch newBatch;
    string input;
    double dInput;
    int iInput;

    cout << "=== 새 발효 배치 추가 ===\n";
    cout << "배치 ID: ";
    cin >> input; newBatch.setBatchId(input);

    cout << "시작일 (YYYY-MM-DD): ";
    cin >> input; newBatch.setStartDate(input);

    cout << "입자 크기 (예: fine, medium, coarse): ";
    cin >> input; newBatch.setParticleSize(input);

    cout << "효모 종류: ";
    cin >> input; newBatch.setYeastType(input);

    cin.ignore();
    cout << "사용된 재료 (예: 보리, 옥수수): ";
    getline(cin, input); newBatch.setIngredients(input);

    cout << "양 (리터): ";
    cin >> dInput; newBatch.setAmountLiters(dInput);

    cout << "온도 (°C): ";
    cin >> dInput; newBatch.setTemperature(dInput);

    cout << "습도 (%): ";
    cin >> iInput; newBatch.setHumidity(iInput);

    cout << "발효 시간 (시간): ";
    cin >> iInput; newBatch.setDurationHours(iInput);

    batches.push_back(newBatch);
    cout << "\n발효 배치가 추가되었습니다.\n";
    UIUtils::pauseConsole();
}


//  배치 수정
void BatchManager::updateBatch() {
    string id, input;
    double dInput;
    int iInput;

    cout << "수정할 배치 ID 입력: ";
    cin >> id;

    for (auto& batch : batches) {
        if (batch.getBatchId() == id) {
            cout << "\n=== 배치 정보 수정 ===\n";

            cout << "시작일 (" << batch.getStartDate() << "): ";
            cin >> input; batch.setStartDate(input);

            cout << "입자 크기 (" << batch.getParticleSize() << "): ";
            cin >> input; batch.setParticleSize(input);

            cout << "효모 종류 (" << batch.getYeastType() << "): ";
            cin >> input; batch.setYeastType(input);

            cin.ignore();
            cout << "사용된 재료 (" << batch.getIngredients() << "): ";
            getline(cin, input); batch.setIngredients(input);

            cout << "양 (리터) (" << batch.getAmountLiters() << "): ";
            cin >> dInput; batch.setAmountLiters(dInput);

            cout << "온도 (°C) (" << batch.getTemperature() << "): ";
            cin >> dInput; batch.setTemperature(dInput);

            cout << "습도 (%) (" << batch.getHumidity() << "): ";
            cin >> iInput; batch.setHumidity(iInput);

            cout << "발효 시간 (시간) (" << batch.getDurationHours() << "): ";
            cin >> iInput; batch.setDurationHours(iInput);

            cout << "\n 배치가 성공적으로 수정되었습니다.\n";
            UIUtils::pauseConsole();
            return;
        }
    }

    cout << "해당 ID의 배치를 찾을 수 없습니다.\n";
    UIUtils::pauseConsole();
}


//  배치 삭제
void BatchManager::deleteBatch() {
    string id;
    cout << "삭제할 배치 ID 입력: ";
    cin >> id;

    auto it = remove_if(batches.begin(), batches.end(), [&](const FermentationBatch& b) {
        return b.getBatchId() == id;
        });

    if (it != batches.end()) {
        batches.erase(it, batches.end());
        cout << "\n배치가 삭제되었습니다.\n";
    }
    else {
        cout << "\n해당 ID의 배치를 찾을 수 없습니다.\n";
    }
    UIUtils::pauseConsole();
}


//  배치 검색
void BatchManager::searchBatch() {
    string id;
    cout << "조회할 배치 ID 입력: ";
    cin >> id;

    for (const auto& batch : batches) {
        if (batch.getBatchId() == id) {
            vector<FermentationBatch> found = { batch };
            displayBatches(found);
            return;
        }
    }

    cout << "\n해당 ID의 배치를 찾을 수 없습니다.\n";
    UIUtils::pauseConsole();
}


//  발효 예측
void BatchManager::predictBatchFermentation() {
    string id;
    cout << "예측할 배치 ID 입력: ";
    cin >> id;

    for (const auto& batch : batches) {
        if (batch.getBatchId() == id) {
            double prediction = predictFermentation(batch);
            cout << "\n예측된 발효 정도: " << prediction << "% 입니다.\n";
            UIUtils::pauseConsole();
            return;
        }
    }

    cout << "\n해당 ID의 배치를 찾을 수 없습니다.\n";
    UIUtils::pauseConsole();
}


//  발효 예측 알고리즘 (간단한 가중치 기반 계산)
double BatchManager::predictFermentation(const FermentationBatch& batch) {
    double size_factor = 1.0;
    string size = batch.getParticleSize();

    if (size == "fine") size_factor = 1.2;
    else if (size == "medium") size_factor = 1.0;
    else if (size == "coarse") size_factor = 0.8;

    double score = (batch.getTemperature() * 0.8) +
        (batch.getHumidity() * 0.2) +
        (batch.getDurationHours() * 1.5);
    score *= size_factor;

    return clamp(score, 0.0, 100.0);
}


//  CSV로 발효 상태 저장
void BatchManager::exportFermentationStatusToCSV() {
    string filename = "fermentation_status.csv";
    if (FileExporter::exportToCSV(batches, filename)) {
        cout << "\n발효 상태가 CSV 파일로 성공적으로 저장되었습니다.\n";
        cout << "저장 경로: " << filesystem::current_path() / filename << "\n";
    }
    else {
        cout << "\nCSV 저장 실패: 파일을 열 수 없습니다.\n";
    }
    UIUtils::pauseConsole();
}


//  발효 중인 배치 수 계산 (72시간 미만인 경우)
int BatchManager::countFermentingBatches() {
    return static_cast<int>(count_if(batches.begin(), batches.end(), [](const FermentationBatch& b) {
        return b.getDurationHours() < 72;
        }));
}

//  평균 발효 시간 계산
double BatchManager::calculateAvgFermentationHours() {
    if (batches.empty()) return 0.0;
    int total = 0;
    for (const auto& b : batches)
        total += b.getDurationHours();
    return static_cast<double>(total) / static_cast<double>(batches.size());
}


//  저장 위치 가져오기 (현재는 "탱크"로 고정됨)
set<string> BatchManager::getStorageLocations() {
    set<string> locations;
    for (const auto& b : batches) {
        locations.insert("탱크"); // 향후 batch.getStorageLocation() 으로 확장 가능
    }
    return locations;
}


//  배치 관리 페이지 메인 루프
void BatchManager::showBatchPage() {
    batches = getDummyBatches(); // 시작 시 더미 데이터 초기화

    int choice;
    do {
        UIUtils::clearConsole();
        cout << "=== 발효 배치 관리 메뉴 ===\n\n";

        int total = static_cast<int>(batches.size());
        int fermenting = countFermentingBatches();
        int completed = total - fermenting;
        double avg = calculateAvgFermentationHours();
        auto locations = getStorageLocations();

        vector<string> infoLines = {
            "전체 배치 수: " + to_string(total) + "개",
            "상태 분포: 발효 중 " + to_string(fermenting) + "개 / 완료 " + to_string(completed) + "개",
            "평균 발효 시간: " + to_string(static_cast<int>(avg)) + "시간",
            "저장 위치: " + UIUtils::joinStrings(locations, " ")
        };

        vector<string> menu = {
            "[1] 배치 목록 조회",
            "[2] 새 배치 추가",
            "[3] 배치 정보 수정",
            "[4] 배치 삭제",
            "[5] 배치 검색",
            "[6] 발효 정도 예측",
            "[7] 발효 상태 CSV 출력",
            "[0] 메인으로 돌아가기"
        };

        UIUtils::drawDashboard(infoLines, menu, 72, 30);

        cout << "\n입력 >> ";
        cin >> choice;

        switch (choice) {
        case 1: displayBatches(batches); break;
        case 2: addBatch(); break;
        case 3: updateBatch(); break;
        case 4: deleteBatch(); break;
        case 5: searchBatch(); break;
        case 6: predictBatchFermentation(); break;
        case 7: exportFermentationStatusToCSV(); break;
        case 0: cout << "메인 메뉴로 돌아갑니다.\n"; break;
        default: UIUtils::printError("잘못된 입력입니다."); UIUtils::pauseConsole(); break;
        }

    } while (choice != 0);
}
