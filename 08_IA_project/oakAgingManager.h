#pragma once
#include <string>
#include <vector>

// -----------------------------------------------------------------------------
// [OakBox] ��ũ�� ���� Ŭ����
// -----------------------------------------------------------------------------
class OakBox {
private:
    std::string boxId;                 // ��ũ�� ���� ���̵�
    std::string name;                  // ��ũ�� ��Ī
    std::string type;                  // ���� (������, �θ��� ��)
    std::string origin;                // �������
    std::string woodType;              // ���� ���� (ȭ��Ʈ ��ũ ��)
    std::string spiritId;              // ������ ���Ǹ� ID 
    std::string agingStartDate;        // ���� ������ 
    std::string agingEndDate;          // ���� ������

    int ripeningPeriod = 0;            // ���� �Ⱓ (�ϼ�)
    int agingCount = 0;                // ���� Ƚ��
    int waterAbsorptionTime = 0;       // ���� �ӱ��� �ð�

    double evaporationRate = 0.0;      // ���߷�
    double temperature = 0.0;          // �µ�
    double humidity = 0.0;             // ����

    bool roasted = false;              // �ν��� ����

public:
    OakBox() = default;

    // ���� ���
    void ShowInfo() const;

    // Getter
    std::string getId() const;
    std::string getName() const;
    std::string getType() const;
    std::string getOrigin() const;
    std::string getWoodType() const;
    std::string getSpiritId() const;
    std::string getAgingStartDate() const;
    std::string getAgingEndDate() const;

    int getRipeningPeriod() const;
    int getAgingCount() const;
    int getWaterAbsorptionTime() const;

    double getEvaporationRate() const;
    double getTemperature() const;
    double getHumidity() const;

    bool isRoasted() const;

    // Setter
    void setId(const std::string& id);
    void setName(const std::string& name);
    void setType(const std::string& t);
    void setOrigin(const std::string& o);
    void setWoodType(const std::string& wood);
    void setSpiritId(const std::string& id);
    void setAgingStartDate(const std::string& date);
    void setAgingEndDate(const std::string& date);

    void setAgingCount(int count);
    void setWaterAbsorptionTime(int t);
    void setRipeningPeriod(int p);

    void setEvaporationRate(double e);
    void setTemperature(double t);
    void setHumidity(double h);

    void setRoasted(bool r);

    // CSV ����ȭ/������ȭ (�ʿ��)
    std::string toCSV() const;
    static OakBox fromCSV(const std::string& line);
};

// -----------------------------------------------------------------------------
// [OakAgingManager] ��ũ ���� ��ü ��� �� ��� ���� Ŭ����
// -----------------------------------------------------------------------------
class RecipeManager; // ���� ����

class OakAgingManager {
public:
    // [1] ������ �����
    void loadOakBoxesFromCSV(const std::string& filename);
    void saveOakBoxesToCSV(const std::string& filename);

    // [2] ������ ��� ��ũ ����
    void produceOakAgingByRecipe(RecipeManager& recipeMgr);

    // [3] ���� ���/��ȸ/���
    std::string getSummary();
    std::vector<std::string> getPageInfoLines();
    void showOakList();

    // [4] CSV ��������
    void exportOakBoxesToCSV(const std::string& filename);

    // [5] �Է�/����/����/�˻�
    void addOakBox();
    void updateOakBox();
    void deleteOakBox();

    // [6] ���� �޴�
    void showOakAgingPage();

    // [7] ESP32 ����
    void receiveOakBoxFromESP32();

private:
    std::vector<OakBox> oakList; // ��ũ�� ����Ʈ (�޸� �� ����)
};
