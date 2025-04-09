#pragma once

class MainPage {
public:
    void run(); // 메인 루프 실행
private:
    void displayMenu();
    void handleSelection(int choice);
};
