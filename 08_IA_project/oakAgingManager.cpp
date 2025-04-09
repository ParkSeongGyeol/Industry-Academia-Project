#include "OakAgingManager.h"
#include <iostream>

// 클래스 항목 헤더 파일로 이동

OakBox::OakBox(std::string id, std::string t, std::string o, int period, int count, double evarate, double temp, double hum, bool roast)
    : boxId(id), type(t), origin(o), ripeningPeriod(period), agingCount(count),
      evaporationRate(evarate), temperature(temp), humidity(hum), roasted(roast) {}

void OakBox::ShowInfo() const {
    std::cout << "오크통 ID : " << boxId << std::endl;
    std::cout << "종류 : " << type << std::endl;
    std::cout << "출신 지역 : " << origin << std::endl;
    std::cout << "숙성 기간: " << ripeningPeriod << "일" << std::endl;
    std::cout << "숙성 횟수: " << agingCount << "차 숙성" << std::endl;
    std::cout << "증발률: " << evaporationRate << "%" << std::endl;
    std::cout << "온도: " << temperature << "℃" << std::endl;
    std::cout << "습도: " << humidity << "%" << std::endl;
    std::cout << "로스팅 여부: " << (roasted ? "로스팅됨" : "로스팅 안됨") << std::endl;
}

void OakAgingManager::showOakList() {
    OakBox box1("OX001", "Sherry Cask", "Spain", 120, 1, 2.5, 18.5, 65.0, true);
    OakBox box2("OX002", "Bourbon Cask", "USA", 90, 2, 3.1, 17.2, 60.5, false);

    std::cout << "\n=== 숙성중인 오크통 정보 ===\n\n";

    std::cout << "[1번 오크통]\n";
    box1.ShowInfo();
    std::cout << "\n";

    std::cout << "[2번 오크통]\n";
    box2.ShowInfo();
    std::cout << "\n";
}


// main 함수를 하나로 만들기 위한 작업

//int main() {
//	OakBox box1("OX001", "Sherry Cask", "Spain", 120, 1, 2.5, 18.5, 65.0, true);
//	OakBox box2("OX002", "Bourbon Cask", "USA", 90, 2, 3.1, 17.2, 60.5, false); // 예시 오크통 1번과 2번
//
//	cout << "숙성중인 오크통 정보" << endl << endl;
//
//	cout << "1번 오크통" << endl;
//	box1.ShowInfo();
//	cout << endl;
//
//	cout << "2번 오크통" << endl;
//	box2.ShowInfo();
//	cout << endl;
//
//	return 0;
//}


