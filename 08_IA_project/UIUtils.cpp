#include "UIUtils.h"
#include <iostream>
#include <iomanip>
#include <sstream>  // joinStrings ������ ���� �߰�

namespace UIUtils {

    //  �ؽ�Ʈ ��� ����
    std::string centerText(const std::string& text, int width) {
        int pad = width - static_cast<int>(text.length());
        if (pad <= 0) return text.substr(0, width); // ��� �ڸ���
        int padLeft = pad / 2;
        int padRight = pad - padLeft;
        return std::string(padLeft, ' ') + text + std::string(padRight, ' ');
    }

    //  ���� �ڽ� ���
    void drawBox(const std::string& content, int width) {
        std::string centered = centerText(content, width - 2);
        std::cout << "+" << std::string(width - 2, '-') << "+\n";
        std::cout << "|" << centered << "|\n";
        std::cout << "+" << std::string(width - 2, '-') << "+\n";
    }

    //  �� ���� ���� �ڽ� ���
    void drawRow(const std::vector<std::string>& contents, int width) {
        for (const auto& content : contents)
            std::cout << "+" << std::string(width - 2, '-');
        std::cout << "+\n";

        for (const auto& content : contents)
            std::cout << "|" << centerText(content, width - 2);
        std::cout << "|\n";

        for (const auto& content : contents)
            std::cout << "+" << std::string(width - 2, '-');
        std::cout << "+\n";
    }

    //  ���� ���� �ڽ� �׸��� ���
    void drawGrid(const std::vector<std::vector<std::string>>& grid, int width) {
        for (const auto& row : grid)
            drawRow(row, width);
    }

    //  ��ú��� ���·� ���� + �޴� ���
    void drawDashboard(
        const std::vector<std::string>& infoLines,
        const std::vector<std::string>& menuLines,
        int leftWidth,
        int rightWidth
    ) {
        size_t maxLines = std::max(infoLines.size(), menuLines.size());

        std::vector<std::string> left = infoLines;
        std::vector<std::string> right = menuLines;
        left.resize(maxLines, "");
        right.resize(maxLines, "");

        std::string leftBorder = "+" + std::string(leftWidth - 2, '-') + "+";
        std::string rightBorder = "+" + std::string(rightWidth - 2, '-') + "+";

        std::cout << leftBorder << rightBorder << "\n";

        for (size_t i = 0; i < maxLines; ++i) {
            std::string leftContent = centerText(left[i], leftWidth - 2);
            std::string rightContent = centerText(right[i], rightWidth - 2);
            std::cout << "|" << leftContent << "|" << rightContent << "|\n";
        }

        std::cout << leftBorder << rightBorder << "\n";
    }

    //  ���ڿ� ������ �ϳ��� ���ڿ��� ���� (delimiter�� ����)
    std::string UIUtils::joinStrings(const std::set<std::string>& items, const std::string& delimiter) {
        std::ostringstream oss;
        for (auto it = items.begin(); it != items.end(); ++it) {
            oss << *it;
            if (std::next(it) != items.end()) {
                oss << delimiter;
            }
        }
        return oss.str();
    }

} // namespace UIUtils
