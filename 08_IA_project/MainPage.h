#pragma once

#include "RawMaterialManager.h"
#include "BatchManager.h"
#include "SpiritManager.h"
#include "OakAgingManager.h"
#include "BottledWhiskyManager.h"
#include "RecipeManager.h"

// MainPage 클래스는 전체 위스키 생산 시스템의 시작점으로,
// 각 매니저 인스턴스를 생성 및 공유하고 대시보드를 통한 제어를 담당한다.
class MainPage {
public:
    // 메인 루프 실행: 대시보드 출력 및 사용자 입력 처리
    void run();

private:
    // 대시보드 화면 출력 함수
    void displayDashboard(BatchManager& batch, RawMaterialManager& raw,
        SpiritManager& spirit, OakAgingManager& oak,
        BottledWhiskyManager& bottle);

    // 사용자의 선택에 따라 해당 관리 페이지 실행
    void handleSelection(int choice, RawMaterialManager& rawMgr,
        BatchManager& batchMgr, SpiritManager& spiritMgr,
        OakAgingManager& oakMgr, BottledWhiskyManager& bottleMgr,
        RecipeManager& recipeMgr);
};
