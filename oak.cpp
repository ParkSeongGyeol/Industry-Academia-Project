#include <iostream>
#include <string>
using namespace std;


class OakBox {
private:
	string boxId; // 오크통 고유 아이디
	string type; // 종류
	string origin; // 출신지역
	int ripeningPeriod; // 숙성 기간
	int agingCount; // 숙성 횟수
	double evaporationRate; // 증발률
	double temperature; // 온도
	double humidity; // 습도
	bool roasted; // 로스팅 여부

public:
	OakBox(string id, string t, string o, int period, int count, double evarate, double temp, double hum, bool roast)
		: boxId(id), type(t), origin(o), ripeningPeriod(period), agingCount(count), evaporationRate(evarate), temperature(temp), humidity(hum), roasted(roast) {
	} // 생성자

	void ShowInfo() const {
		cout << "오크통 ID : " << boxId << endl;
		cout << "종류 : " << type << endl;
		cout << "출신 지역 : " << origin << endl;
		cout << "숙성 기간: " << ripeningPeriod << "일" << endl;
		cout << "숙성 횟수: " << agingCount << "차 숙성" << endl;
		cout << "증발률: " << evaporationRate << "%" << endl;
		cout << "온도: " << temperature << "℃" << endl;
		cout << "습도: " << humidity << "%" << endl;
		cout << "로스팅 여부: " << (roasted ? "로스팅됨" : "로스팅 안됨") << endl;

	}
};

int main() {
	OakBox box1("OX001", "Sherry Cask", "Spain", 120, 1, 2.5, 18.5, 65.0, true);
	OakBox box2("OX002", "Bourbon Cask", "USA", 90, 2, 3.1, 17.2, 60.5, false); // 예시 오크통 1번과 2번

	cout << "숙성중인 오크통 정보" << endl << endl;

	cout << "1번 오크통" << endl;
	box1.ShowInfo();
	cout << endl;

	cout << "2번 오크통" << endl;
	box2.ShowInfo();
	cout << endl;

	return 0;
}