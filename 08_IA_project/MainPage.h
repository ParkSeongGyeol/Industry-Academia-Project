#pragma once

// MainPage 클래스는 전체 시스템의 메인 대시보드 역할을 하며,
// 대시보드 출력 및 메뉴 선택에 따른 기능 페이지 진입을 처리한다.
class MainPage {
public:
    // 메인 루프 실행 함수 (대시보드 표시 및 사용자 입력 반복)
    void run();

private:
    // 대시보드에 현재 시스템 요약 정보와 메뉴 출력
    void displayDashboard();

    // 사용자 선택에 따라 각 관리 페이지로 이동
    void handleSelection(int choice);
};
