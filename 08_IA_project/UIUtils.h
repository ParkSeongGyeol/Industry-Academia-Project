#pragma once
#include <string>
#include <vector>
#include <set>
#include <iostream>  // std::cout, std::cin 등 사용 위해 추가

namespace UIUtils {

    //  텍스트 가운데 정렬
    // 문자열을 지정된 너비에 맞춰 가운데 정렬한 결과를 반환
    std::string centerText(const std::string& text, int width);

    //  텍스트 좌측 정렬
    // 문자열을 지정된 너비에 맞춰 좌측 정렬한 결과를 반환
    std::string leftAlignText(const std::string& text, int width);

    //  단일 박스 출력
    // 문자열을 박스 형태로 출력
    void drawBox(const std::string& content, int width = 40);

    //  한 줄에 여러 박스 출력
    void drawRow(const std::vector<std::string>& contents, int width = 40);

    // 2차원 그리드 박스 출력
    void drawGrid(const std::vector<std::vector<std::string>>& grid, int width = 40);

    //  대시보드 출력 (정보/메뉴를 양쪽으로 나눠서 출력)
    void drawDashboard(
        const std::vector<std::string>& infoLines,
        const std::vector<std::string>& menuLines,
        int leftWidth = 72,
        int rightWidth = 55
    );

    //  콘솔 화면 정리
    // 플랫폼에 따라 콘솔을 초기화 (윈도우/리눅스 호환)
    inline void clearConsole() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    //  콘솔 일시 정지
    // 사용자 입력을 받을 때까지 대기
    inline void pauseConsole() {
        std::cout << "\n계속하려면 Enter를 누르세요...";
        std::cin.ignore();
        std::cin.get();
    }

    //  에러 메시지 출력
    inline void printError(const std::string& msg) {
        std::cout << " 오류: " << msg << "\n";
    }

    //  문자열 집합을 구분자로 연결하여 하나의 문자열로 반환
    std::string joinStrings(const std::set<std::string>& items, const std::string& delimiter = " ");

} // namespace UIUtils
