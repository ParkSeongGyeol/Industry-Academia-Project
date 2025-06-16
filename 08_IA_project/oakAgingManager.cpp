#define WIN32_LEAN_AND_MEAN      // windows ��� �ּ�ȭ
#define NOMINMAX                 // min/max ��ũ�� �浹 ����

#include <windows.h>            // byte �浹 �߻��ϴ� ���
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include "OakAgingManager.h"
#include "UIUtils.h"
#include "RecipeManager.h"
#include "Recipe.h"
#include "CommonUtils.h"

using namespace std;

// ----------------------------- ��� ���� -----------------------------
namespace {
    constexpr char OAKAGING_CSV[] = "oakaging_dummy.csv";
}

// ----------------------------- [1] ������ ����� -----------------------------

// CSV���� ��ũ�� ��� �ε�
void OakAgingManager::loadOakBoxesFromCSV(const string& filename) {
    oakList.clear();
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[���] ��ũ�� CSV ������ �� �� �����ϴ�: " << filename << endl;
        return;
    }
    string line;
    getline(file, line); // ��� ��ŵ
    while (getline(file, line)) {
        if (line.empty()) continue;
        istringstream iss(line);
        string token;
        vector<string> fields;
        while (getline(iss, token, ',')) {
            fields.push_back(token);
        }
        if (fields.size() < 14) continue;
        OakBox box;
        box.setId(fields[0]);
        box.setName(fields[1]);
        box.setType(fields[2]);
        box.setOrigin(fields[3]);
        box.setRipeningPeriod(stoi(fields[4]));
        box.setAgingCount(stoi(fields[5]));
        box.setEvaporationRate(stod(fields[6]));
        box.setTemperature(stod(fields[7]));
        box.setHumidity(stod(fields[8]));
        box.setWaterAbsorptionTime(stoi(fields[9]));
        box.setWoodType(fields[10]);
        box.setSpiritId(fields[11]);
        box.setAgingStartDate(fields[12]);
        box.setAgingEndDate(fields[13]);
        // roasted �ʵ�� CSV�� ������ false�� ��
        box.setRoasted(false);
        oakList.push_back(box);
    }
    file.close();
}

// ��ũ�� ����� CSV�� ����
void OakAgingManager::saveOakBoxesToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "[����] ��ũ�� CSV ���� ����: " << filename << endl;
        return;
    }
    file << "OakID,Name,Type,Origin,AgingPeriod,RefillCount,EvaporationRate,Temperature,Humidity,WaterAbsorptionTime,WoodType,SpiritID,AgingStartDate,AgingEndDate\n";
    for (const auto& b : oakList) {
        file << b.getId() << ","
            << b.getName() << ","
            << b.getType() << ","
            << b.getOrigin() << ","
            << b.getRipeningPeriod() << ","
            << b.getAgingCount() << ","
            << b.getEvaporationRate() << ","
            << b.getTemperature() << ","
            << b.getHumidity() << ","
            << b.getWaterAbsorptionTime() << ","
            << b.getWoodType() << ","
            << b.getSpiritId() << ","
            << b.getAgingStartDate() << ","
            << b.getAgingEndDate() << "\n";
    }
    file.close();
}

// ----------------------------- [OakBox] Getter/Setter/ShowInfo ���� -----------------------------

// Getter
std::string OakBox::getId() const { return boxId; }
std::string OakBox::getName() const { return name; }
std::string OakBox::getType() const { return type; }
std::string OakBox::getOrigin() const { return origin; }
std::string OakBox::getWoodType() const { return woodType; }
std::string OakBox::getSpiritId() const { return spiritId; }
std::string OakBox::getAgingStartDate() const { return agingStartDate; }
std::string OakBox::getAgingEndDate() const { return agingEndDate; }

int OakBox::getRipeningPeriod() const { return ripeningPeriod; }
int OakBox::getAgingCount() const { return agingCount; }
int OakBox::getWaterAbsorptionTime() const { return waterAbsorptionTime; }

double OakBox::getEvaporationRate() const { return evaporationRate; }
double OakBox::getTemperature() const { return temperature; }
double OakBox::getHumidity() const { return humidity; }

bool OakBox::isRoasted() const { return roasted; }

// Setter
void OakBox::setId(const std::string& id) { boxId = id; }
void OakBox::setName(const std::string& n) { name = n; }
void OakBox::setType(const std::string& t) { type = t; }
void OakBox::setOrigin(const std::string& o) { origin = o; }
void OakBox::setWoodType(const std::string& wood) { woodType = wood; }
void OakBox::setSpiritId(const std::string& id) { spiritId = id; }
void OakBox::setAgingStartDate(const std::string& date) { agingStartDate = date; }
void OakBox::setAgingEndDate(const std::string& date) { agingEndDate = date; }

void OakBox::setAgingCount(int count) { agingCount = count; }
void OakBox::setWaterAbsorptionTime(int t) { waterAbsorptionTime = t; }
void OakBox::setRipeningPeriod(int p) { ripeningPeriod = p; }

void OakBox::setEvaporationRate(double e) { evaporationRate = e; }
void OakBox::setTemperature(double t) { temperature = t; }
void OakBox::setHumidity(double h) { humidity = h; }

void OakBox::setRoasted(bool r) { roasted = r; }

// ���� ���
void OakBox::ShowInfo() const {
    std::cout << "��ũ�� ID: " << boxId << "\n"
        << "��Ī: " << name << "\n"
        << "����: " << type << "\n"
        << "�������: " << origin << "\n"
        << "���� ����: " << woodType << "\n"
        << "���Ǹ� ID: " << spiritId << "\n"
        << "���� ������: " << agingStartDate << "\n"
        << "���� ������: " << agingEndDate << "\n"
        << "���� �Ⱓ: " << ripeningPeriod << "��\n"
        << "���� Ƚ��: " << agingCount << "\n"
        << "���� �ӱ��� �ð�: " << waterAbsorptionTime << "�ð�\n"
        << "���߷�: " << evaporationRate << "%\n"
        << "�µ�: " << temperature << "��\n"
        << "����: " << humidity << "%\n"
        << "�ν��� ����: " << (roasted ? "����" : "����") << "\n";
}

// ----------------------------- [2] ������ ��� ��ũ ���� -----------------------------
/**
 * ������ ��� ��ũ ����
 * - ������ ����� �����ְ�, ����ڰ� ������ ID�� ���� ������ �Է�
 * - ���� ����(���߷�, �Ⱓ ��) �Է� ��, OakBox ��ü�� ���
 */
void OakAgingManager::produceOakAgingByRecipe(RecipeManager& recipeMgr) {
    recipeMgr.listRecipes();
    string recipeId = inputString("\n��ũ ������ ����� ������ ID �Է�: ");
    Recipe recipe;
    if (!recipeMgr.getRecipeById(recipeId, recipe)) {
        cout << "�ش� ID�� �����Ǹ� ã�� �� �����ϴ�.\n";
        UIUtils::pauseConsole();
        return;
    }

    // ���� ���߷�/�Ⱓ/Ƚ�� �� �Է�
    double evaporationRate = inputDouble("���� ���߷�(%): ");
    int agingMonths = inputInt("���� �Ⱓ(��): ");
    int refillCount = inputInt("���� Ƚ��(���� ����): ");
    string oakType = inputString("��ũ�� ����: ");
    string woodType = inputString("���� ����: ");
    string origin = inputString("��ũ�� ��� ����: ");
    double temp = inputDouble("���� �µ�(��): ");
    double hum = inputDouble("���� ����(%): ");
    int waterAbsorption = inputInt("���� �ӱ��� �ð�(�ð�): ");
    string spiritId = recipe.spiritId.empty() ? inputString("���Ǹ� ID: ") : recipe.spiritId;
    string startDate = getCurrentDate();
    string endDate = inputString("���� ������ (YYYY-MM-DD): ");
    string name = inputString("��ũ�� ��Ī: ");

    // ������ ���� ���� ���
    recipe.oakType = oakType;
    recipe.agingMonths = agingMonths;
    recipe.ageSpirit(evaporationRate);

    OakBox box;
    box.setId("OAK_" + to_string(oakList.size() + 1));
    box.setName(name);
    box.setType(oakType);
    box.setOrigin(origin);
    box.setRipeningPeriod(agingMonths * 30); // �� �� �� ȯ��
    box.setAgingCount(refillCount);
    box.setEvaporationRate(evaporationRate);
    box.setTemperature(temp);
    box.setHumidity(hum);
    box.setWaterAbsorptionTime(waterAbsorption);
    box.setWoodType(woodType);
    box.setSpiritId(spiritId);
    box.setAgingStartDate(startDate);
    box.setAgingEndDate(endDate);
    box.setRoasted(false);

    oakList.push_back(box);
    saveOakBoxesToCSV(OAKAGING_CSV);

    cout << "������ ��� ��ũ ���� ������ ��ϵǾ����ϴ�. (��ũ��ID: " << box.getId() << ")\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [3] ���� ���/��ȸ/��� -----------------------------

// ��ü ��ũ�� ��� ����(���� ��) ��ȯ
string OakAgingManager::getSummary() {
    return "������ ��: " + to_string(oakList.size()) + "��";
}

// ��ú���/�޴��� ���� ��� ���� ��ȯ
vector<string> OakAgingManager::getPageInfoLines() {
    vector<string> lines;
    size_t count = oakList.size();
    int totalPeriod = 0;
    double totalEvap = 0;
    for (const auto& box : oakList) {
        totalPeriod += box.getRipeningPeriod();
        totalEvap += box.getEvaporationRate();
    }
    lines.push_back("��ϵ� ��ũ�� ��: " + to_string(count));
    if (count > 0) {
        lines.push_back("��� ���� �Ⱓ: " + to_string(totalPeriod / (int)count) + "��");
        lines.push_back("��� ���߷�: " + to_string((int)(totalEvap / count)) + "%");
    }
    else {
        lines.push_back("��� ���� �Ⱓ: -");
        lines.push_back("��� ���߷�: -");
    }
    return lines;
}

// ��ü ��ũ�� ��� ���
void OakAgingManager::showOakList() {
    if (oakList.empty()) {
        cout << "��ϵ� ��ũ���� �����ϴ�.\n";
        return;
    }
    for (const auto& oak : oakList) {
        oak.ShowInfo();
        cout << "-----------------------------\n";
    }
    UIUtils::pauseConsole();
}

// ----------------------------- [4] CSV �������� -----------------------------

// ��ũ�� ����� CSV�� ��������
void OakAgingManager::exportOakBoxesToCSV(const string& filename) {
    saveOakBoxesToCSV(filename);
    cout << "[ " << filename << " ] ���Ϸ� ���� �Ϸ�!\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [5] �Է�/����/����/�˻� -----------------------------

// ��ũ�� �ű� �߰�
void OakAgingManager::addOakBox() {
    OakBox box;
    box.setId(inputString("��ũ�� ID: "));
    box.setName(inputString("��ũ�� ��Ī: "));
    box.setType(inputString("����: "));
    box.setOrigin(inputString("��� ����: "));
    box.setRipeningPeriod(inputInt("���� �Ⱓ(��): "));
    box.setAgingCount(inputInt("���� Ƚ��: "));
    box.setEvaporationRate(inputDouble("���߷�(%): "));
    box.setTemperature(inputDouble("�µ�(��): "));
    box.setHumidity(inputDouble("����(%): "));
    box.setWaterAbsorptionTime(inputInt("���� �ӱ��� �ð�(�ð�): "));
    box.setWoodType(inputString("���� ����: "));
    box.setSpiritId(inputString("���Ǹ� ID: "));
    box.setAgingStartDate(inputString("���� ������ (YYYY-MM-DD): "));
    box.setAgingEndDate(inputString("���� ������ (YYYY-MM-DD): "));
    box.setRoasted(inputInt("�ν��� ���� (1: ����, 0: ����): ") == 1);

    oakList.push_back(box);
    saveOakBoxesToCSV(OAKAGING_CSV);
    cout << "��ũ���� �߰��Ǿ����ϴ�!\n";
    UIUtils::pauseConsole();
}

// ��ũ�� ���� ����
void OakAgingManager::updateOakBox() {
    string id = inputString("������ ��ũ�� ID �Է�: ");
    for (auto& box : oakList) {
        if (box.getId() == id) {
            cout << "=== ��ũ�� ���� ���� ===\n";
            box.setOrigin(inputString("��� ���� (" + box.getOrigin() + "): "));
            box.setAgingCount(inputInt("���� Ƚ�� (" + to_string(box.getAgingCount()) + "): "));
            box.setWaterAbsorptionTime(inputInt("���� �ӱ��� �ð� (" + to_string(box.getWaterAbsorptionTime()) + "): "));
            box.setRoasted(inputInt("�ν��� ���� (" + string(box.isRoasted() ? "1" : "0") + "): ") == 1);
            box.setWoodType(inputString("���� ���� (" + box.getWoodType() + "): "));
            box.setType(inputString("���� (" + box.getType() + "): "));
            box.setRipeningPeriod(inputInt("���� �Ⱓ(��) (" + to_string(box.getRipeningPeriod()) + "): "));
            box.setEvaporationRate(inputDouble("���߷�(%) (" + to_string(box.getEvaporationRate()) + "): "));
            box.setTemperature(inputDouble("�µ�(��) (" + to_string(box.getTemperature()) + "): "));
            box.setHumidity(inputDouble("����(%) (" + to_string(box.getHumidity()) + "): "));
            box.setId(inputString("��ũ�� ID (" + box.getId() + "): "));
            box.setName(inputString("��ũ�� ��Ī (" + box.getName() + "): "));
            box.setSpiritId(inputString("���Ǹ� ID (" + box.getSpiritId() + "): "));
            box.setAgingStartDate(inputString("���� ������ (" + box.getAgingStartDate() + "): "));
            box.setAgingEndDate(inputString("���� ������ (" + box.getAgingEndDate() + "): "));
            saveOakBoxesToCSV(OAKAGING_CSV);
            cout << "���� �Ϸ�.\n";
            UIUtils::pauseConsole();
            return;
        }
    }
    cout << "�ش� ID�� ���� ��ũ���� �����ϴ�.\n";
    UIUtils::pauseConsole();
}

// ��ũ�� ����
void OakAgingManager::deleteOakBox() {
    string id = inputString("������ ��ũ�� ID �Է�: ");
    auto it = remove_if(oakList.begin(), oakList.end(), [&](const OakBox& b) { return b.getId() == id; });
    if (it != oakList.end()) {
        oakList.erase(it, oakList.end());
        saveOakBoxesToCSV(OAKAGING_CSV);
        cout << "��ũ���� �����Ǿ����ϴ�.\n";
    }
    else {
        cout << "�ش� ID�� ã�� �� �����ϴ�.\n";
    }
    UIUtils::pauseConsole();
}

// ----------------------------- [6] ���� �޴� ���� -----------------------------

// ��ũ�� ���� ���� ���� �޴� (������ ���� ����)
void OakAgingManager::showOakAgingPage() {
    loadOakBoxesFromCSV(OAKAGING_CSV);

    RecipeManager recipeMgr;
    recipeMgr.loadRecipesFromCSV("recipe_list.csv");

    int choice;
    do {
        system("cls");
        vector<string> infoLines = getPageInfoLines();
        vector<string> menu = {
            "[1] ��ũ�� ��� ����",
            "[2] ��ũ�� �߰�",
            "[3] ��ũ�� ����",
            "[4] ��ũ�� ����",
            "[5] CSV�� ����",
            "[6] ������ ��� ��ũ ����",
            "[7] ESP32 ȯ�� ������ ����",
            "[0] �������� ���ư���"
        };
        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        cout << "\n�Է� >> ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: showOakList(); break;
        case 2: addOakBox(); break;
        case 3: updateOakBox(); break;
        case 4: deleteOakBox(); break;
        case 5: exportOakBoxesToCSV(OAKAGING_CSV); break;
        case 6: produceOakAgingByRecipe(recipeMgr); break;
        case 7: receiveOakBoxFromESP32(); break;
        case 0: cout << "�������� ���ư��ϴ�.\n"; break;
        default: cout << "�߸��� �Է��Դϴ�.\n"; break;
        }

        if (choice != 0) {
            cout << "\n����Ϸ��� Enter�� ��������...";
            cin.get();
        }
    } while (choice != 0);
}

// ----------------------------- [7] ESP32 ���� �� ȯ�� ���� ���� -----------------------------

// ESP32�κ��� ��ũ�� ȯ�� ������(TCP) ���� �� ���
void OakAgingManager::receiveOakBoxFromESP32() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    SOCKADDR_IN serverAddr, clientAddr;
    int clientSize = sizeof(clientAddr);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup ����" << endl;
        return;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cout << "���� ���� ����" << endl;
        WSACleanup();
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000); // OakAging ���� ��Ʈ
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "bind ����" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    listen(serverSocket, 1);
    cout << "[TCP ����] ESP32 ���� ��� ��...\n";

    clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientSize);
    if (clientSocket == INVALID_SOCKET) {
        cout << "accept ����" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    char buffer[1024] = { 0 };
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

    if (bytesReceived > 0) {
        string data(buffer);
        cout << "[���� ������] " << data << endl;

        istringstream ss(data);
        string token;
        vector<string> fields;

        while (getline(ss, token, ',')) {
            fields.push_back(token);
        }

        if (fields.size() == 14) {
            OakBox box;
            box.setId(fields[0]);
            box.setName(fields[1]);
            box.setType(fields[2]);
            box.setOrigin(fields[3]);
            box.setRipeningPeriod(stoi(fields[4]));
            box.setAgingCount(stoi(fields[5]));
            box.setEvaporationRate(stod(fields[6]));
            box.setTemperature(stod(fields[7]));
            box.setHumidity(stod(fields[8]));
            box.setWaterAbsorptionTime(stoi(fields[9]));
            box.setWoodType(fields[10]);
            box.setSpiritId(fields[11]);
            box.setAgingStartDate(fields[12]);
            box.setAgingEndDate(fields[13]);
            box.setRoasted(false);

            oakList.push_back(box);
            cout << "��ũ�� ȯ�� ������ ���� �Ϸ�!\n";
            saveOakBoxesToCSV(OAKAGING_CSV);
        }
        else {
            cout << "�ʵ� �� ����: " << fields.size() << "��" << endl;
        }
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}