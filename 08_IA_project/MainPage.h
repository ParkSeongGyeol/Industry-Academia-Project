#pragma once

// MainPage Ŭ������ ��ü �ý����� ���� ��ú��� ������ �ϸ�,
// ��ú��� ��� �� �޴� ���ÿ� ���� ��� ������ ������ ó���Ѵ�.
class MainPage {
public:
    // ���� ���� ���� �Լ� (��ú��� ǥ�� �� ����� �Է� �ݺ�)
    void run();

private:
    // ��ú��忡 ���� �ý��� ��� ������ �޴� ���
    void displayDashboard();

    // ����� ���ÿ� ���� �� ���� �������� �̵�
    void handleSelection(int choice);
};
