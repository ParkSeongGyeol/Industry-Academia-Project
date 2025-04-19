#pragma once
#include <string>
#include <vector>
#include <set>
#include <iostream>  // std::cout, std::cin 등 사용 위해 추가

namespace UIUtils {

    // 텍스트 가운데 정렬
    std::string centerText(const std::string& text, int width);

    // 박스 출력
    void drawBox(const std::string& content, int width = 20);
    void drawRow(const std::vector<std::string>& contents, int width = 20);
    void drawGrid(const std::vector<std::vector<std::string>>& grid, int width = 20);

    // 대시보드 출력 (좌/우 영역)
    void drawDashboard(
        const std::vector<std::string>& infoLines,
        const std::vector<std::string>& menuLines,
        int leftWidth = 72,
        int rightWidth = 30
    );

    // 콘솔 정리
    inline void clearConsole() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    // 콘솔 일시 정지
    inline void pauseConsole() {
        std::cout << "\n계속하려면 Enter를 누르세요...";
        std::cin.ignore();
        std::cin.get();
    }

    // 에러 메시지 출력
    inline void printError(const std::string& msg) {
        std::cout << " 오류: " << msg << "\n";
    }

    // 문자열 집합을 한 줄로 연결
    std::string joinStrings(const std::set<std::string>& items, const std::string& delimiter = " ");

} // namespace UIUtils
