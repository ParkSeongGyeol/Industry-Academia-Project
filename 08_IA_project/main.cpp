#include "MainPage.h"
#include <locale>
#include <clocale>

int main() {
    std::setlocale(LC_ALL, "ko_KR.eucKR");
    std::locale::global(std::locale(""));
    MainPage mainPage;
    mainPage.run();
    return 0;
}
