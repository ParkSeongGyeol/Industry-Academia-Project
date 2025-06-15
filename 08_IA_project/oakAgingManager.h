#pragma once
#include <string>
#include <vector>

// -----------------------------------------------------------------------------
// [OakBox] 오크통 정보 클래스
// -----------------------------------------------------------------------------
class OakBox {
private:
    std::string boxId;                 // 오크통 고유 아이디
    std::string name;                  // 오크통 별칭
    std::string type;                  // 종류 (버번통, 셰리통 등)
    std::string origin;                // 출신지역
    std::string woodType;              // 나무 종류 (화이트 오크 등)
    std::string spiritId;              // 숙성된 스피릿 ID 
    std::string agingStartDate;        // 숙성 시작일 
    std::string agingEndDate;          // 숙성 종료일

    int ripeningPeriod = 0;            // 숙성 기간 (일수)
    int agingCount = 0;                // 숙성 횟수
    int waterAbsorptionTime = 0;       // 물을 머금은 시간

    double evaporationRate = 0.0;      // 증발률
    double temperature = 0.0;          // 온도
    double humidity = 0.0;             // 습도

    bool roasted = false;              // 로스팅 여부

public:
    OakBox() = default;

    // 정보 출력
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

    // CSV 직렬화/역직렬화 (필요시)
    std::string toCSV() const;
    static OakBox fromCSV(const std::string& line);
};

// -----------------------------------------------------------------------------
// [OakAgingManager] 오크 숙성 전체 목록 및 기능 관리 클래스
// -----------------------------------------------------------------------------
class RecipeManager; // 전방 선언

class OakAgingManager {
public:
    // [1] 데이터 입출력
    void loadOakBoxesFromCSV(const std::string& filename);
    void saveOakBoxesToCSV(const std::string& filename);

    // [2] 레시피 기반 오크 숙성
    void produceOakAgingByRecipe(RecipeManager& recipeMgr);

    // [3] 정보 요약/조회/출력
    std::string getSummary();
    std::vector<std::string> getPageInfoLines();
    void showOakList();

    // [4] CSV 내보내기
    void exportOakBoxesToCSV(const std::string& filename);

    // [5] 입력/수정/삭제/검색
    void addOakBox();
    void updateOakBox();
    void deleteOakBox();

    // [6] 메인 메뉴
    void showOakAgingPage();

    // [7] ESP32 연동
    void receiveOakBoxFromESP32();

private:
    std::vector<OakBox> oakList; // 오크통 리스트 (메모리 상 저장)
};
