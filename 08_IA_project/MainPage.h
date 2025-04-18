#pragma once

class MainPage {
public:
    void run(); // 메인 루프 실행
private:
    void displayDashboard();         // 정보 요약 + 메뉴 출력
    void handleSelection(int choice);
};
