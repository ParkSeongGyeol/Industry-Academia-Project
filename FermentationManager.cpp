// FermentationManager.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "BatchManager.h"  // 배치 관련 헤더 파일 포함

int main()
{
    BatchManager manager;

    // JSON 파일에서 배치 정보 불러오기
    std::vector<FermentationBatch> batches = manager.loadBatchesFromFile("batches.json");

    // 불러온 배치 목록 출력
    manager.displayBatches(batches);

    return 0;
}
