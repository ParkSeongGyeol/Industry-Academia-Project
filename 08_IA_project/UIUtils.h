#pragma once
#include <string>
#include <vector>
#include <set>
#include <iostream>  // std::cout, std::cin �� ��� ���� �߰�

namespace UIUtils {

    // �ؽ�Ʈ ��� ����
    std::string centerText(const std::string& text, int width);

    // �ڽ� ���
    void drawBox(const std::string& content, int width = 20);
    void drawRow(const std::vector<std::string>& contents, int width = 20);
    void drawGrid(const std::vector<std::vector<std::string>>& grid, int width = 20);

    // ��ú��� ��� (��/�� ����)
    void drawDashboard(
        const std::vector<std::string>& infoLines,
        const std::vector<std::string>& menuLines,
        int leftWidth = 72,
        int rightWidth = 30
    );

    // �ܼ� ����
    inline void clearConsole() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    // �ܼ� �Ͻ� ����
    inline void pauseConsole() {
        std::cout << "\n����Ϸ��� Enter�� ��������...";
        std::cin.ignore();
        std::cin.get();
    }

    // ���� �޽��� ���
    inline void printError(const std::string& msg) {
        std::cout << " ����: " << msg << "\n";
    }

    // ���ڿ� ������ �� �ٷ� ����
    std::string joinStrings(const std::set<std::string>& items, const std::string& delimiter = " ");

} // namespace UIUtils
