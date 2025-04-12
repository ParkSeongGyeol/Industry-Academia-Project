#include "UIUtils.h"
#include <iostream>
#include <iomanip>

namespace UIUtils {

    std::string UIUtils::centerText(const std::string& text, int width) {
        int pad = width - static_cast<int>(text.length());
        if (pad <= 0) {
            return text.substr(0, width); // 길면 잘라냄
        }

        int padLeft = pad / 2;
        int padRight = pad - padLeft;
        return std::string(padLeft, ' ') + text + std::string(padRight, ' ');
    }


    void drawBox(const std::string& content, int width) {
        std::string centered = centerText(content, width - 2);

        std::cout << "+" << std::string(width - 2, '-') << "+\n";
        std::cout << "|" << centered << "|\n";
        std::cout << "+" << std::string(width - 2, '-') << "+\n";
    }

    void drawRow(const std::vector<std::string>& contents, int width) {
        // 상단 테두리
        for (const auto& content : contents)
            std::cout << "+" << std::string(width - 2, '-');
        std::cout << "+\n";

        // 내용 줄
        for (const auto& content : contents)
            std::cout << "|" << centerText(content, width - 2);
        std::cout << "|\n";

        // 하단 테두리
        for (const auto& content : contents)
            std::cout << "+" << std::string(width - 2, '-');
        std::cout << "+\n";
    }

    void drawGrid(const std::vector<std::vector<std::string>>& grid, int width) {
        for (const auto& row : grid)
            drawRow(row, width);
    }
    void UIUtils::drawDashboard(
        const std::vector<std::string>& infoLines,
        const std::vector<std::string>& menuLines,
        int leftWidth,
        int rightWidth
    ) {
        size_t maxLines = std::max(infoLines.size(), menuLines.size());
        std::string leftBorder = "+" + std::string(leftWidth - 2, '-') + "+";
        std::string rightBorder = "+" + std::string(rightWidth - 2, '-') + "+";

        // 상단 테두리
        std::cout << leftBorder << rightBorder << "\n";

        for (size_t i = 0; i < maxLines; ++i) {
            std::string leftContent = (i < infoLines.size()) ? centerText(infoLines[i], leftWidth - 2) : std::string(leftWidth - 2, ' ');
            std::string rightContent = (i < menuLines.size()) ? centerText(menuLines[i], rightWidth - 2) : std::string(rightWidth - 2, ' ');
            std::cout << "|" << leftContent << "|" << rightContent << "|\n";
        }

        // 하단 테두리
        std::cout << leftBorder << rightBorder << "\n";
    }


}
