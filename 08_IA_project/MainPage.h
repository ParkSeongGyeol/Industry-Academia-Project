#pragma once

class MainPage {
public:
    void run(); // ���� ���� ����
private:
    void displayDashboard();         // ���� ��� + �޴� ���
    void handleSelection(int choice);
    void exportCSVTest();               //  FileExporter��
    void showStorageEnvironmentTest();  //  StorageEnvironment��
};
