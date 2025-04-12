# IA_project: 위스키 생산 관리 콘솔 시스템

위스키 생산 과정 중 **물류 이동과 생산 관리를 지원하는 콘솔 기반 프로그램**입니다.  
사용자는 콘솔 UI를 통해 **원재료, 발효 배치, 스피릿, 오크통 숙성, 병입 제품**까지 전체 흐름을 한눈에 파악하고 관리할 수 있습니다.

---

## 🔧 기능 구성

- **메인 페이지**: 전체 공정 요약 지표, 메뉴 선택, 입력 창으로 구성된 대시보드 UI
- **원재료 항목 관리**: 재고 목록 확인 (*기능 확장 예정*)
- **발효 배치 관리**: 배치 목록 출력
- **스피릿 관리**: 추가, 삭제, 수정, 목록 보기 기능 포함
- **오크통 숙성 관리**: 오크통 리스트 출력
- **병입 및 완성품 관리**: 재고 목록, 출고 등록, 출고 기록 관리

---

## 🧱 콘솔 UI 레이아웃

전체 콘솔 화면은 **정보 영역(좌측)**과 **선택 메뉴 영역(우측)**으로 나뉘며,  
하단에는 **입력 창이 고정 배치**되어 있어 사용자가 메뉴를 편리하게 선택할 수 있습니다.

```
+------------------------------------------------------------------------+------------------------------+
|                         배치 수: 2개                                  | [1] 원재료 관리              |
|                  원재료: 3종 / 300kg                                  | [2] 배치 관리                |
|          스피릿: 2종 / 평균 도수: 64%                                 | [3] 스피릿 관리              |
|                  숙성통 수: 2개                                       | [4] 오크통 숙성 관리         |
|      병입: 10병 / 평균가: 23000원                                     | [5] 병입 및 완성품 관리      |
+------------------------------------------------------------------------+------------------------------+

입력 >> 
```

- 모든 정보는 **실시간으로 각 매니저 인스턴스에서 계산된 값**을 사용합니다.
- 전체 레이아웃은 고정된 폭(정보 72, 메뉴 30)으로 구성되어 깨짐 없이 출력됩니다.

---

## 🖥️ 실행 방법

### 💡 전제 조건
- Visual Studio 2019 이상
- C++17 이상 설정

### 📦 빌드 및 실행
1. 이 리포지토리를 클론하거나 다운로드합니다.
2. Visual Studio에서 `IA_project.sln`을 엽니다.
3. `main.cpp`가 포함된 프로젝트를 시작 프로젝트로 설정합니다.
4. 실행(F5)하면 콘솔 UI가 뜹니다.

---

## 📁 디렉토리 구조

```
IA_project/
├── IA_project.sln
├── 08_IA_project/
│   ├── main.cpp
│   ├── MainPage.h / .cpp
│   ├── UIUtils.h / .cpp
│   ├── RawMaterialManager.h / .cpp
│   ├── BatchManager.h / .cpp
│   ├── SpiritManager.h / .cpp
│   ├── OakAgingManager.h / .cpp
│   ├── BottledWhiskyManager.h / .cpp
├── README.md
├── .gitignore
```

---

## 📝 TODO

- [ ] 원재료 항목 추가/수정/삭제 기능 구현
- [ ] 병입 제품 삭제 및 수정 기능 구현
- [ ] JSON 파일을 활용한 데이터 저장/불러오기 기능 구현
- [ ] 전체 공정 흐름 기반의 통합 리포트 기능 구현

---

## 📄 라이센스

MIT License © 2025 박성결, 김종호, 김민기
