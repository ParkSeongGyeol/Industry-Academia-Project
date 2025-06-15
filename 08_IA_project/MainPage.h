#pragma once

#include "RawMaterialManager.h"
#include "BatchManager.h"
#include "SpiritManager.h"
#include "OakAgingManager.h"
#include "BottledWhiskyManager.h"
#include "RecipeManager.h"

// MainPage Ŭ������ ��ü ����Ű ���� �ý����� ����������,
// �� �Ŵ��� �ν��Ͻ��� ���� �� �����ϰ� ��ú��带 ���� ��� ����Ѵ�.
class MainPage {
public:
    // ���� ���� ����: ��ú��� ��� �� ����� �Է� ó��
    void run();

private:
    // ��ú��� ȭ�� ��� �Լ�
    void displayDashboard(BatchManager& batch, RawMaterialManager& raw,
        SpiritManager& spirit, OakAgingManager& oak,
        BottledWhiskyManager& bottle);

    // ������� ���ÿ� ���� �ش� ���� ������ ����
    void handleSelection(int choice, RawMaterialManager& rawMgr,
        BatchManager& batchMgr, SpiritManager& spiritMgr,
        OakAgingManager& oakMgr, BottledWhiskyManager& bottleMgr,
        RecipeManager& recipeMgr);
};
