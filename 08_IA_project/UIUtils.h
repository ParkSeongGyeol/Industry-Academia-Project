#pragma once
#include <string>
#include <vector>
#include <set>
#include <iostream>  // std::cout, std::cin �� ��� ���� �߰�

namespace UIUtils {

    //  �ؽ�Ʈ ��� ����
    // ���ڿ��� ������ �ʺ� ���� ��� ������ ����� ��ȯ
    std::string centerText(const std::string& text, int width);

    //  �ؽ�Ʈ ���� ����
    // ���ڿ��� ������ �ʺ� ���� ���� ������ ����� ��ȯ
    std::string leftAlignText(const std::string& text, int width);

    //  ���� �ڽ� ���
    // ���ڿ��� �ڽ� ���·� ���
    void drawBox(const std::string& content, int width = 40);

    //  �� �ٿ� ���� �ڽ� ���
    void drawRow(const std::vector<std::string>& contents, int width = 40);

    // 2���� �׸��� �ڽ� ���
    void drawGrid(const std::vector<std::vector<std::string>>& grid, int width = 40);

    //  ��ú��� ��� (����/�޴��� �������� ������ ���)
    void drawDashboard(
        const std::vector<std::string>& infoLines,
        const std::vector<std::string>& menuLines,
        int leftWidth = 72,
        int rightWidth = 55
    );

    //  �ܼ� ȭ�� ����
    // �÷����� ���� �ܼ��� �ʱ�ȭ (������/������ ȣȯ)
    inline void clearConsole() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    //  �ܼ� �Ͻ� ����
    // ����� �Է��� ���� ������ ���
    inline void pauseConsole() {
        std::cout << "\n����Ϸ��� Enter�� ��������...";
        std::cin.ignore();
        std::cin.get();
    }

    //  ���� �޽��� ���
    inline void printError(const std::string& msg) {
        std::cout << " ����: " << msg << "\n";
    }

    //  ���ڿ� ������ �����ڷ� �����Ͽ� �ϳ��� ���ڿ��� ��ȯ
    std::string joinStrings(const std::set<std::string>& items, const std::string& delimiter = " ");

} // namespace UIUtils
