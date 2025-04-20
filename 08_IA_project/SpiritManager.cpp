#include "SpiritManager.h"
#include "UIUtils.h"
#include <iostream>
#include <algorithm>

std::string SpiritManager::getSummary() {
    size_t count = spirits.size();
    double totalYield = 0, totalAbv = 0;

    for (const auto& s : spirits) {
        totalYield += s.yield_liters;
        totalAbv += s.alcohol_percentage;
    }

    std::string result = "���Ǹ�: " + std::to_string(count) + "��";
    if (count > 0) {
        result += " / ��� ����: " + std::to_string(static_cast<int>(totalAbv / count)) + "%";
    }
    return result;
}

std::vector<std::string> SpiritManager::getPageInfoLines() {
    std::vector<std::string> lines;

    size_t count = spirits.size();
    double totalYield = 0;
    double totalAbv = 0;
    for (const auto& s : spirits) {
        totalYield += s.yield_liters;
        totalAbv += s.alcohol_percentage;
    }

    lines.push_back("��ϵ� ���Ǹ� ��: " + std::to_string(count) + "��");
    if (count > 0) {
        lines.push_back("�� ���귮: " + std::to_string((int)totalYield) + "L");
        lines.push_back("��� ����: " + std::to_string((int)(totalAbv / count)) + "%");
    }
    else {
        lines.push_back("�� ���귮: 0L");
        lines.push_back("��� ����: -");
    }

    return lines;
}

void SpiritManager::showSpiritPage() {
    int choice;
    do {
        system("cls");
        std::cout << "=== ���Ǹ� ���� �޴� ===\n\n";

        std::vector<std::string> infoLines = getPageInfoLines();
        std::vector<std::string> menu = {
            "[1] ���Ǹ� ��� ����",
            "[2] ���Ǹ� �߰�",
            "[3] ���Ǹ� ����",
            "[4] ���Ǹ� ����",
            "[0] �������� ���ư���"
        };

        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        std::cout << "\n�Է� >> ";
        std::cin >> choice;

        switch (choice) {
        case 1: displaySpirits(); break;
        case 2: addSpirit(); break;
        case 3: deleteSpirit(); break;
        case 4: updateSpirit(); break;
        case 0: std::cout << "�������� ���ư��ϴ�...\n"; break;
        default: std::cout << "�߸��� �Է��Դϴ�.\n"; break;
        }

        if (choice != 0) {
            std::cout << "\n����Ϸ��� Enter�� ��������...";
            std::cin.ignore(); std::cin.get();
        }

    } while (choice != 0);
}

void SpiritManager::addSpirit() {
    Spirit spirit;
    std::cout << "\n=== ���Ǹ� �߰� ===\n";
    std::cout << "ID: "; std::cin >> spirit.id;
    std::cin.ignore();
    std::cout << "���� ���: "; std::getline(std::cin, spirit.storage_location);
    std::cout << "�̵� ���: "; std::getline(std::cin, spirit.transfer_history);
    std::cout << "���� ��¥ (YYYY-MM-DD): "; std::getline(std::cin, spirit.production_date);
    std::cout << "����(%): "; std::cin >> spirit.alcohol_percentage;
    std::cout << "���귮(L): "; std::cin >> spirit.yield_liters;
    std::cin.ignore();
    std::cout << "����� �Է�: "; std::getline(std::cin, spirit.raw_material_ratio);
    std::cout << "��ȿ �Ⱓ(��): "; std::cin >> spirit.fermentation_days;
    std::cin.ignore();
    std::cout << "�ʷ� ���� ����: "; std::getline(std::cin, spirit.first_cut_time);
    std::cout << "�ķ� �׸� ���� ����: "; std::getline(std::cin, spirit.last_cut_time);
    std::cout << "���� Ƚ��: "; std::cin >> spirit.distillation_count;

    spirits.push_back(spirit);
    std::cout << "? ���Ǹ� �߰� �Ϸ�!\n";
}

void SpiritManager::deleteSpirit() {
    std::string id;
    std::cout << "������ ���Ǹ� ID �Է�: ";
    std::cin >> id;

    auto it = std::remove_if(spirits.begin(), spirits.end(), [&](const Spirit& s) {
        return s.id == id;
        });

    if (it != spirits.end()) {
        spirits.erase(it, spirits.end());
        std::cout << "? ���� �Ϸ�!\n";
    }
    else {
        std::cout << "�ش� ID�� ���Ǹ��� ã�� �� �����ϴ�.\n";
    }
}

void SpiritManager::displaySpirits() {
    std::cout << "\n=== ���Ǹ� ��� ===\n";
    for (const auto& s : spirits) {
        std::cout << "ID: " << s.id << "\n"
            << "���� ���: " << s.storage_location << "\n"
            << "�̵� ���: " << s.transfer_history << "\n"
            << "���� ��¥: " << s.production_date << "\n"
            << "����: " << s.alcohol_percentage << "%\n"
            << "���귮: " << s.yield_liters << "L\n"
            << "����� �Է�: " << s.raw_material_ratio << "\n"
            << "��ȿ �Ⱓ: " << s.fermentation_days << "��\n"
            << "�ʷ� ���� ����: " << s.first_cut_time << "\n"
            << "�ķ� ���� ����: " << s.last_cut_time << "\n"
            << "���� Ƚ��: " << s.distillation_count << "\n"
            << "--------------------------\n";
    }
}

void SpiritManager::updateSpirit() {
    std::string id;
    std::cout << "\n������ ���Ǹ� ID �Է�: ";
    std::cin >> id;

    for (auto& s : spirits) {
        if (s.id == id) {
            std::cin.ignore();
            std::cout << "=== ���Ǹ� ���� ���� ===\n";

            std::cout << "���� ��� (" << s.storage_location << "): ";
            std::getline(std::cin, s.storage_location);

            std::cout << "�̵� ��� (" << s.transfer_history << "): ";
            std::getline(std::cin, s.transfer_history);

            std::cout << "���� ��¥ (" << s.production_date << "): ";
            std::getline(std::cin, s.production_date);

            std::cout << "���� (%) (" << s.alcohol_percentage << "): ";
            std::cin >> s.alcohol_percentage;

            std::cout << "���귮 (L) (" << s.yield_liters << "): ";
            std::cin >> s.yield_liters;

            std::cin.ignore();
            std::cout << "����� �Է� (" << s.raw_material_ratio << "): ";
            std::getline(std::cin, s.raw_material_ratio);

            std::cout << "��ȿ �Ⱓ (��) (" << s.fermentation_days << "): ";
            std::cin >> s.fermentation_days;
            std::cin.ignore();

            std::cout << "�ʷ� ���� ���� (" << s.first_cut_time << "): ";
            std::getline(std::cin, s.first_cut_time);

            std::cout << "�ķ� ���� ���� (" << s.last_cut_time << "): ";
            std::getline(std::cin, s.last_cut_time);

            std::cout << "���� Ƚ�� (" << s.distillation_count << "): ";
            std::cin >> s.distillation_count;

            std::cout << "? ���Ǹ� ���� �Ϸ�!\n";
            return;
        }
    }

    std::cout << "�ش� ID�� ���Ǹ��� ã�� �� �����ϴ�.\n";
}

