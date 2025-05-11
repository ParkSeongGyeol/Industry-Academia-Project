#include "FileExporter.h"
#include "BatchManager.h" // struct FermentationBatch 정의 포함
#include <fstream>        // 파일 출력에 사용
#include <iostream>
#include <filesystem>     // 현재 경로 확인용 (C++17 이상에서 사용 가능)

//  FileExporter 클래스의 정적 함수
//  주어진 배치 데이터를 CSV 파일로 저장
bool FileExporter::exportToCSV(const std::vector<FermentationBatch>& batches, const std::string& filename) {
    // 파일 스트림 열기 (쓰기 모드)
    std::ofstream file(filename);

    // 파일 열기 실패 시 false 반환
    if (!file.is_open()) return false;

    //  CSV 헤더 작성
    file << "BatchID,StartDate,ParticleSize,YeastType,Ingredients,Amount(L),Temperature,Humidity,Duration(H)\n";

    //  각 배치 데이터를 한 줄씩 CSV 형식으로 출력
    for (const auto& batch : batches) {
        file << batch.toCSV() << "\n";  // FermentationBatch::toCSV() 사용
    }

    //  파일 닫기
    file.close();

    //  성공적으로 저장 완료
    return true;
}
