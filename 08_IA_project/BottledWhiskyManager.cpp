#define WIN32_LEAN_AND_MEAN      // windows ��� �ּ�ȭ
#define NOMINMAX                 // min/max ��ũ�� �浹 ����
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "BottledWhiskyManager.h"
#include "RecipeManager.h"
#include "Recipe.h"
#include "UIUtils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

// ----------------------------- ��� ���� -----------------------------
namespace {
    constexpr char BOTTLED_CSV[] = "bottledwhisky_dummy.csv";
}

// ----------------------------- ��ƿ��Ƽ �Լ� -----------------------------

// ���� �ý��� ��¥�� "YYYY-MM-DD" �������� ��ȯ
string getCurrentDate() {
    time_t now = time(nullptr);
    tm t;
    localtime_s(&t, &now);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", &t);
    return string(buf);
}

// ������ double �Է� �Լ�
double inputDouble(const string& prompt) {
    double val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cout << "���ڸ� �Է��ϼ���.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ������ int �Է� �Լ�
int inputInt(const string& prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cout << "������ �Է��ϼ���.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ������ string �Է� �Լ�
string inputString(const string& prompt) {
    cout << prompt;
    string val;
    getline(cin, val);
    return val;
}

// ----------------------------- [1] ������ ����� -----------------------------

// CSV���� ���� ����Ű ��� �ε�
void BottledWhiskyManager::loadInventoryFromCSV(const string& filename) {
    inventory.clear();
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[���] ���� CSV ������ �� �� �����ϴ�: " << filename << endl;
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
        if (fields.size() < 11) continue;
        BottledWhisky w;
        w.setProductId(fields[0]);
        w.setName(fields[1]);
        w.setBottleCount(stoi(fields[2]));
        w.setTotalVolume(stod(fields[3]));
        w.setPricePerBottle(stod(fields[4]));
        w.setLabelName(fields[5]);
        w.setOakId(fields[6]);
        w.setExportTarget(fields[7]);
        w.setShipmentDate(fields[8]);
        w.setSerialNumber(fields[9]);
        w.setBottlingManager(fields[10]);
        inventory.push_back(w);
    }
    file.close();
}

// ���� ����Ű ����� CSV�� ����
void BottledWhiskyManager::saveInventoryToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "[����] ���� CSV ���� ����: " << filename << endl;
        return;
    }
    file << "ProductID,ProductName,Quantity,TotalVolume,BottlePrice,LabelName,OakID,ReleaseTarget,ReleaseDate,ManufactureNumber,BottlingManager\n";
    for (const auto& w : inventory) {
        file << w.getProductId() << ","
            << w.getName() << ","
            << w.getBottleCount() << ","
            << w.getTotalVolume() << ","
            << w.getPricePerBottle() << ","
            << w.getLabelName() << ","
            << w.getOakId() << ","
            << w.getExportTarget() << ","
            << w.getShipmentDate() << ","
            << w.getSerialNumber() << ","
            << w.getBottlingManager() << "\n";
    }
    file.close();
}

// ----------------------------- [2] ������ ��� ���� ���� -----------------------------
/**
 * ������ ��� ���� ����
 * - ������ ����� �����ְ�, ����ڰ� ������ ID�� ���� ������ �Է�
 * - ���� ����(�� ��, �뷮, ���� ��) �Է� ��, BottledWhisky ��ü�� ���
 */
void BottledWhiskyManager::produceBottledByRecipe(RecipeManager& recipeMgr) {
    recipeMgr.listRecipes();
    string recipeId = inputString("\n���Կ� ����� ������ ID �Է�: ");
    Recipe recipe;
    if (!recipeMgr.getRecipeById(recipeId, recipe)) {
        cout << "�ش� ID�� �����Ǹ� ã�� �� �����ϴ�.\n";
        UIUtils::pauseConsole();
        return;
    }

    // ���� ���� �Է�
    int bottleCount = inputInt("�� ��: ");
    double bottleVolume = inputDouble("���� �뷮(ml): ");
    double bottlePrice = inputDouble("���� ����(��): ");
    string labelName = inputString("�󺧸�: ");
    string oakId = inputString("��ũ�� ID: ");
    string exportTarget = inputString("��� ���: ");
    string shipmentDate = getCurrentDate();
    string serialNumber = inputString("���� ��ȣ: ");
    string bottlingManager = inputString("���� �����: ");

    // ������ ���� ���� ���
    recipe.bottleCount = bottleCount;
    recipe.bottleVolume = bottleVolume;
    recipe.bottlePrice = bottlePrice;
    recipe.bottledName = labelName;
    recipe.bottleProduct();

    BottledWhisky w;
    w.setProductId("P" + to_string(inventory.size() + 1));
    w.setName(recipe.name);
    w.setBottleCount(bottleCount);
    w.setTotalVolume(recipe.totalBottledVolume * 1000); // L �� ml
    w.setPricePerBottle(bottlePrice);
    w.setLabelName(labelName);
    w.setOakId(oakId);
    w.setExportTarget(exportTarget);
    w.setShipmentDate(shipmentDate);
    w.setSerialNumber(serialNumber);
    w.setBottlingManager(bottlingManager);
    w.setLabeled(true);

    inventory.push_back(w);
    saveInventoryToCSV(BOTTLED_CSV);

    cout << "������ ��� ���� ����Ű�� ��ϵǾ����ϴ�. (��ǰID: " << w.getProductId() << ")\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [3] ���� ���/��ȸ/��� -----------------------------

// ��ü ���� ����Ű ��� ����(���� ��) ��ȯ
string BottledWhiskyManager::getSummary() {
    int totalCount = 0;
    double totalPrice = 0;
    for (const auto& w : inventory) {
        totalCount += w.getBottleCount();
        totalPrice += w.getPricePerBottle();
    }
    string result = "����: " + to_string(totalCount) + "��";
    if (!inventory.empty()) {
        result += " / ��հ�: " + to_string(static_cast<int>(totalPrice / inventory.size())) + "��";
    }
    return result;
}

// ��ú���/�޴��� ���� ��� ���� ��ȯ
vector<string> BottledWhiskyManager::getPageInfoLines() {
    vector<string> lines;
    int totalCount = 0;
    double totalVolume = 0, totalValue = 0;
    for (const auto& w : inventory) {
        totalCount += w.getBottleCount();
        totalVolume += w.getTotalVolume();
        totalValue += w.getBottleCount() * w.getPricePerBottle();
    }
    lines.push_back("��ü �� ����: " + to_string(totalCount) + "��");
    lines.push_back("��ü �뷮: " + to_string((int)totalVolume) + "ml");
    if (totalCount > 0) {
        lines.push_back("��� ���� ����: " + to_string((int)(totalValue / totalCount)) + "��");
    }
    else {
        lines.push_back("��� ���� ����: -");
    }
    return lines;
}

// ��ü ���� ����Ű ��� ���
void BottledWhiskyManager::showInventory() {
    if (inventory.empty()) {
        cout << "���� ��� �����ϴ�.\n";
        return;
    }
    cout << "\n[���� ����Ű ��� ���]\n";
    for (const auto& whisky : inventory) {
        whisky.ShowInfo();
        cout << "-----------------------------\n";
    }
    UIUtils::pauseConsole();
}

// ----------------------------- [4] CSV �������� -----------------------------

// ���� ����Ű ��� CSV ���Ϸ� ��������
void BottledWhiskyManager::exportInventoryToCSV(const string& filename) {
    saveInventoryToCSV(filename);
    cout << "[ " << filename << " ] ���Ϸ� ���� �Ϸ�!\n";
    UIUtils::pauseConsole();
}

// ----------------------------- [5] �Է�/����/����/�˻� -----------------------------

// ���� ����Ű �ű� �߰�
void BottledWhiskyManager::addWhisky() {
    BottledWhisky w;
    w.setProductId(inputString("��ǰ ID: "));
    w.setName(inputString("��ǰ��: "));
    w.setLabelName(inputString("�󺧸�: "));
    w.setBatchNumber(inputString("��ġ ��ȣ: "));
    w.setExportTarget(inputString("��� ���: "));
    w.setOakId(inputString("��ũ�� ID: "));
    w.setShipmentDate(inputString("��� ���� (YYYY-MM-DD): "));
    w.setSerialNumber(inputString("���� ��ȣ: "));
    w.setBottlingManager(inputString("���� �����: "));
    w.setBottleCount(inputInt("����(��): "));
    w.setTotalVolume(inputDouble("�� �뷮(ml): "));
    w.setPricePerBottle(inputDouble("���� ����: "));
    w.setLabeled(inputInt("�� ���� ���� (1: O, 0: X): ") == 1);

    inventory.push_back(w);
    saveInventoryToCSV(BOTTLED_CSV);
    cout << "��ǰ�� �߰��Ǿ����ϴ�.\n";
    UIUtils::pauseConsole();
}

// ��ǰ ���� ����
void BottledWhiskyManager::updateWhisky() {
    string name = inputString("������ ��ǰ�� �Է�: ");
    for (auto& w : inventory) {
        if (w.getName() == name) {
            cout << "=== ��ǰ ���� ���� ===\n";
            w.setName(inputString("��ǰ�� (" + w.getName() + "): "));
            w.setLabelName(inputString("�󺧸� (" + w.getLabelName() + "): "));
            w.setBatchNumber(inputString("��ġ ��ȣ (" + w.getBatchNumber() + "): "));
            w.setExportTarget(inputString("��� ��� (" + w.getExportTarget() + "): "));
            w.setBottleCount(inputInt("����(��) (" + to_string(w.getBottleCount()) + "): "));
            w.setTotalVolume(inputDouble("�� �뷮(ml) (" + to_string(w.getTotalVolume()) + "): "));
            w.setPricePerBottle(inputDouble("���� ���� (" + to_string(w.getPricePerBottle()) + "): "));
            w.setLabeled(inputInt("�� ���� ���� (" + string(w.isLabeled() ? "1" : "0") + "): ") == 1);
            w.setOakId(inputString("��ũ�� ID (" + w.getOakId() + "): "));
            w.setShipmentDate(inputString("��� ���� (" + w.getShipmentDate() + "): "));
            w.setSerialNumber(inputString("���� ��ȣ (" + w.getSerialNumber() + "): "));
            w.setBottlingManager(inputString("���� ����� (" + w.getBottlingManager() + "): "));
            w.setProductId(inputString("��ǰ ID (" + w.getProductId() + "): "));
            saveInventoryToCSV(BOTTLED_CSV);
            cout << "���� �Ϸ�.\n";
            UIUtils::pauseConsole();
            return;
        }
    }
    cout << "�ش� ��ǰ�� ã�� �� �����ϴ�.\n";
    UIUtils::pauseConsole();
}

// ��ǰ ����
void BottledWhiskyManager::deleteWhisky() {
    string name = inputString("������ ��ǰ�� �Է�: ");
    auto it = remove_if(inventory.begin(), inventory.end(), [&](const BottledWhisky& w) { return w.getName() == name; });
    if (it != inventory.end()) {
        inventory.erase(it, inventory.end());
        saveInventoryToCSV(BOTTLED_CSV);
        cout << "�����Ǿ����ϴ�.\n";
    }
    else {
        cout << "�ش� ��ǰ�� ã�� �� �����ϴ�.\n";
    }
    UIUtils::pauseConsole();
}

// ��� ��� ���� �� ��� ����
void BottledWhiskyManager::shipWhisky() {
    string name = inputString("����� ��ǰ��: ");
    int qty = inputInt("��� ����(��): ");
    string date = inputString("��� ��¥ (YYYY-MM-DD): ");

    for (auto& whisky : inventory) {
        if (whisky.getName() == name) {
            if (qty > whisky.getBottleCount()) {
                cout << "��� ������ ����� �����ϴ�.\n";
                UIUtils::pauseConsole();
                return;
            }
            double totalPrice = qty * whisky.getPricePerBottle();

            ShipmentRecord record;
            record.setProductName(name);
            record.setDate(date);
            record.setQuantity(qty);
            record.setTotalPrice(totalPrice);
            shipmentLog.push_back(record);

            whisky.decreaseStock(qty);

            saveInventoryToCSV(BOTTLED_CSV);
            cout << "��� ����� ��ϵǾ����ϴ�.\n";
            UIUtils::pauseConsole();
            return;
        }
    }
    cout << "�ش� ��ǰ�� ã�� �� �����ϴ�.\n";
    UIUtils::pauseConsole();
}

// ��� ��� ���
void BottledWhiskyManager::showShipmentLog() {
    if (shipmentLog.empty()) {
        cout << "��� ����� �����ϴ�.\n";
        return;
    }
    cout << "\n[��� ��� ���]\n";
    for (const auto& record : shipmentLog) {
        record.ShowInfo();
        cout << "-----------------------------\n";
    }
    UIUtils::pauseConsole();
}

// ----------------------------- [6] ���� �޴� ���� -----------------------------

// ���� ����Ű ���� ���� �޴� (������ ���� ����)
void BottledWhiskyManager::showBottledWhiskyPage() {
    loadInventoryFromCSV(BOTTLED_CSV);

    RecipeManager recipeMgr;
    recipeMgr.loadRecipesFromCSV("recipe_list.csv");

    int choice;
    do {
        system("cls");
        vector<string> infoLines = getPageInfoLines();
        vector<string> menu = {
            "[1] ��� ��� ����",
            "[2] ��ǰ �߰�",
            "[3] ��ǰ ���",
            "[4] ��� ��� ����",
            "[5] ����ǰ ����",
            "[6] ����ǰ ����",
            "[7] CSV�� ����",
            "[8] ������ ��� ���� ����",
            "[9] ESP32 ������ ����",
            "[0] ���� �޴��� ���ư���"
        };
        UIUtils::drawDashboard(infoLines, menu, 72, 30);
        cout << "\n�Է� >> ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: showInventory(); break;
        case 2: addWhisky(); break;
        case 3: shipWhisky(); break;
        case 4: showShipmentLog(); break;
        case 5: updateWhisky(); break;
        case 6: deleteWhisky(); break;
        case 7: exportInventoryToCSV(BOTTLED_CSV); break;
        case 8: produceBottledByRecipe(recipeMgr); break;
        case 9: receiveWhiskyFromESP32(); break;
        case 0: cout << "���� �޴��� ���ư��ϴ�...\n"; break;
        default: cout << "�߸��� �Է��Դϴ�.\n"; break;
        }

        if (choice != 0) {
            cout << "\n����Ϸ��� Enter�� ��������...";
            cin.get();
        }
    } while (choice != 0);
}

// ----------------------------- [7] ESP32 ���� �� ȯ�� ���� ���� -----------------------------

// ESP32�κ��� ���� ����Ű ������(TCP) ���� �� ���
void BottledWhiskyManager::receiveWhiskyFromESP32() {
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
    serverAddr.sin_port = htons(5001); // BottledWhisky ���� ��Ʈ
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

        if (fields.size() == 13) {
            BottledWhisky whisky;
            whisky.setProductId(fields[0]);
            whisky.setName(fields[1]);
            whisky.setLabelName(fields[2]);
            whisky.setBatchNumber(fields[3]);
            whisky.setExportTarget(fields[4]);
            whisky.setOakId(fields[5]);
            whisky.setShipmentDate(fields[6]);
            whisky.setSerialNumber(fields[7]);
            whisky.setBottlingManager(fields[8]);
            whisky.setBottleCount(stoi(fields[9]));
            whisky.setTotalVolume(stod(fields[10]));
            whisky.setPricePerBottle(stod(fields[11]));
            whisky.setLabeled(fields[12] == "1" || fields[12] == "Yes");

            inventory.push_back(whisky);
            cout << "���� ����Ű ���� �Ϸ�!\n";
            saveInventoryToCSV(BOTTLED_CSV);
        }
        else {
            cout << "�ʵ� �� ����: " << fields.size() << "��" << endl;
        }
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}