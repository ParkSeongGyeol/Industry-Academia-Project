#pragma once
#include <locale>
#include <ios>

inline void applyCP949Locale(std::ios_base& stream) {
    try {
        stream.imbue(std::locale("ko_KR.eucKR"));
    } catch (const std::exception&) {
        // fallback to default locale if cp949 not available
    }
}
