#include "SpiritManager.h"
#include "UIUtils.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

// 스피릿 개수와 평균 도수를 요약해 반환
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


// 페이지 상단 정보: 스피릿 수, 총 생산량, 평균 도수 등
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
    }
    else {
        lines.push_back("총 생산량: 0L");
        lines.push_back("평균 도수: -");
    }

    return lines;
}


// 콘솔 UI 루프: 사용자 선택에 따라 기능 실행
void SpiritManager::showSpiritPage() {
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
            "[0] 메인으로 돌아가기"
        };

        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        cout << "\n입력 >> ";
        cin >> choice;

        switch (choice) {
        case 1: displaySpirits(); break;
        case 2: addSpirit(); break;
        case 3: deleteSpirit(); break;
        case 4: updateSpirit(); break;
        case 5: exportSpiritsToCSV("spirits.csv"); break;
        case 0: cout << "메인으로 돌아갑니다...\n"; break;
        default: cout << "잘못된 입력입니다.\n"; break;
        }

        if (choice != 0) {
            cout << "\n계속하려면 Enter를 누르세요...";
            cin.ignore(); cin.get();
        }

    } while (choice != 0);
}


// 스피릿 정보 추가 입력 처리
void SpiritManager::addSpirit() {
    Spirit spirit;
    string input;
    double doubleInput;
    int intInput;

    cout << "\n=== 스피릿 추가 ===\n";

    cout << "ID: ";
    cin >> input; spirit.setId(input);
    cin.ignore();

    cout << "보관 장소: ";
    getline(cin, input); spirit.setStorageLocation(input);

    cout << "이동 기록: ";
    getline(cin, input); spirit.setTransferHistory(input);

    cout << "생산 날짜 (YYYY-MM-DD): ";
    getline(cin, input); spirit.setProductionDate(input);

    cout << "도수(%): ";
    cin >> doubleInput; spirit.setAlcoholPercentage(doubleInput);

    cout << "생산량(L): ";
    cin >> doubleInput; spirit.setYieldLiters(doubleInput);
    cin.ignore();

    cout << "원재료 함량: ";
    getline(cin, input); spirit.setRawMaterialRatio(input);

    cout << "발효 기간(일): ";
    cin >> intInput; spirit.setFermentationDays(intInput);
    cin.ignore();

    cout << "초류 끊은 시점: ";
    getline(cin, input); spirit.setFirstCutTime(input);

    cout << "후류 그만 받은 시점: ";
    getline(cin, input); spirit.setLastCutTime(input);

    cout << "증류 횟수: ";
    cin >> intInput; spirit.setDistillationCount(intInput);
    cin.ignore();

    cout << "입자 크기 (예: 미분말, 2mm 입자 등): ";
    getline(cin, input); spirit.setParticleSize(input);

    cout << "재료 배치 ID: ";
    getline(cin, input); spirit.setBatchId(input);

    cout << "초류/후류/본류 수득량 요약: ";
    getline(cin, input); spirit.setCutYield(input);

    spirits.push_back(spirit);
    cout << "스피릿 추가 완료!\n";
}


// 특정 ID를 기준으로 스피릿 삭제
void SpiritManager::deleteSpirit() {
    string id;
    cout << "삭제할 스피릿 ID 입력: ";
    cin >> id;

    auto it = remove_if(spirits.begin(), spirits.end(), [&](const Spirit& s) {
        return s.getId() == id;
        });

    if (it != spirits.end()) {
        spirits.erase(it, spirits.end());
        cout << "삭제 완료!\n";
    }
    else {
        cout << "해당 ID의 스피릿을 찾을 수 없습니다.\n";
    }
}


// 전체 스피릿 목록 출력
void SpiritManager::displaySpirits() {
    cout << "\n=== 스피릿 목록 ===\n";
    for (const auto& s : spirits) {
        cout << "ID: " << s.getId() << "\n"
            << "보관 장소: " << s.getStorageLocation() << "\n"
            << "이동 기록: " << s.getTransferHistory() << "\n"
            << "생산 날짜: " << s.getProductionDate() << "\n"
            << "도수: " << s.getAlcoholPercentage() << "%\n"
            << "생산량: " << s.getYieldLiters() << "L\n"
            << "원재료 함량: " << s.getRawMaterialRatio() << "\n"
            << "발효 기간: " << s.getFermentationDays() << "일\n"
            << "초류 끊은 시점: " << s.getFirstCutTime() << "\n"
            << "후류 종료 시점: " << s.getLastCutTime() << "\n"
            << "증류 횟수: " << s.getDistillationCount() << "\n"
            << "입자 크기: " << s.getParticleSize() << "\n"
            << "재료 배치 ID: " << s.getBatchId() << "\n"
            << "초류/후류/본류 수득량: " << s.getCutYield() << "\n"
            << "--------------------------\n";
    }
}



// 스피릿 정보 수정
void SpiritManager::updateSpirit() {
    string id;
    cout << "\n수정할 스피릿 ID 입력: ";
    cin >> id;

    for (auto& s : spirits) {
        if (s.getId() == id) {
            cin.ignore();
            cout << "=== 스피릿 정보 수정 ===\n";

            string input;
            double doubleInput;
            int intInput;

            cout << "보관 장소 (" << s.getStorageLocation() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setStorageLocation(input);

            cout << "이동 기록 (" << s.getTransferHistory() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setTransferHistory(input);

            cout << "생산 날짜 (" << s.getProductionDate() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setProductionDate(input);

            cout << "도수 (%) (" << s.getAlcoholPercentage() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setAlcoholPercentage(stod(input));

            cout << "생산량 (L) (" << s.getYieldLiters() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setYieldLiters(stod(input));

            cout << "원재료 함량 (" << s.getRawMaterialRatio() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setRawMaterialRatio(input);

            cout << "발효 기간 (일) (" << s.getFermentationDays() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setFermentationDays(stoi(input));

            cout << "초류 끊은 시점 (" << s.getFirstCutTime() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setFirstCutTime(input);

            cout << "후류 종료 시점 (" << s.getLastCutTime() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setLastCutTime(input);

            cout << "증류 횟수 (" << s.getDistillationCount() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setDistillationCount(stoi(input));

            cout << "입자 크기 (" << s.getParticleSize() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setParticleSize(input);

            cout << "재료 배치 ID (" << s.getBatchId() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setBatchId(input);

            cout << "초류/후류/본류 수득량 (" << s.getCutYield() << "): ";
            getline(cin, input);
            if (!input.empty()) s.setCutYield(input);

            cout << "스피릿 수정 완료!\n";
            return;
        }
    }

    cout << "해당 ID의 스피릿을 찾을 수 없습니다.\n";
}

void SpiritManager::exportSpiritsToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "CSV 파일을 열 수 없습니다.\n";
        return;
    }

    // 헤더
    file << "ID,StorageLocation,TransferHistory,ProductionDate,AlcoholPercentage,YieldLiters,"
        << "RawMaterialRatio,FermentationDays,FirstCutTime,LastCutTime,DistillationCount,"
        << "ParticleSize,BatchId,CutYield\n";

    for (const auto& s : spirits) {
        file << s.getId() << ","
            << s.getStorageLocation() << ","
            << s.getTransferHistory() << ","
            << s.getProductionDate() << ","
            << s.getAlcoholPercentage() << ","
            << s.getYieldLiters() << ","
            << s.getRawMaterialRatio() << ","
            << s.getFermentationDays() << ","
            << s.getFirstCutTime() << ","
            << s.getLastCutTime() << ","
            << s.getDistillationCount() << ","
            << s.getParticleSize() << ","
            << s.getBatchId() << ","
            << s.getCutYield() << "\n";
    }

    file.close();
    cout << "[ " << filename << " ] 파일로 저장 완료!\n";
}