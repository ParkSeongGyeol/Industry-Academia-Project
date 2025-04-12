#pragma once
#include <string>
#include <vector>

namespace UIUtils {
    // 1�� �ڽ� �׸���
    void drawBox(const std::string& content, int width = 20);
    void drawRow(const std::vector<std::string>& contents, int width = 20);
    void drawGrid(const std::vector<std::vector<std::string>>& grid, int width = 20);
    std::string centerText(const std::string& text, int width);
    // 2�� ��ǥ�� �ڽ� �׸���
    void drawDashboard(
        const std::vector<std::string>& infoLines,
        const std::vector<std::string>& menuLines,
        int leftWidth = 72,
        int rightWidth = 30
    );
}
