#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Deck {
private:
    string name;
    double winRate;
    double avgPlacement;
    double top4Rate;
    double usageCount;
    int score; // 점수 필드
    vector<string> champions; // 덱 구성 챔피언 리스트

public:
    // 생성자
    Deck(string n, double wr, double ap, double t4r, double uc, vector<string> champs)
        : name(n), winRate(wr), avgPlacement(ap), top4Rate(t4r), usageCount(uc), champions(champs), score(0) {}

    // 멤버 변수 접근을 위한 getter 함수
    string getName() const { return name; }
    double getWinRate() const { return winRate; }
    double getAvgPlacement() const { return avgPlacement; }
    double getTop4Rate() const { return top4Rate; }
    double getUsageCount() const { return usageCount; }
    int getScore() const { return score; }
    const vector<string>& getChampions() const { return champions; }

    // 점수 설정 메서드
    void setScore(int newScore) {
        score = newScore;
    }

    // 덱 정보 출력 메서드
    void printDeckInfo() const {
        string tier;
        if (score >= 12) {
            tier = "S";
        } else if (score >= 9) {
            tier = "A";
        } else if (score >= 6) {
            tier = "B";
        } else {
            tier = "C";
        }
        cout << "덱 이름: " << name << "\n";
        cout << "승률: " << winRate << "%\n";
        cout << "평균 등수: " << avgPlacement << "\n";
        cout << "Top 4 비율: " << top4Rate << "%\n";
        cout << "사용 비율: " << usageCount << "\n";
        cout << "티어: " << tier << "\n";
        cout << "구성 챔피언: ";
        for (const auto& champ : champions) {
            cout << champ << " ";
        }
        cout << "\n-----------------------\n";
    }
};

// 덱 추천 기능 구현 함수
void recommendDeck(vector<Deck>& decks) {
    // 승률, 평균 등수, Top 4 비율을 기준으로 각각 순위를 매겨 점수를 부여
    // 승률 기준 점수 부여
    sort(decks.begin(), decks.end(), [](const Deck& a, const Deck& b) {
        return a.getWinRate() > b.getWinRate();
    });
    for (int i = 0; i < decks.size(); ++i) {
        decks[i].setScore(decks[i].getScore() + (decks.size() - i));
    }

    // 평균 등수 기준 점수 부여 (낮은 순위가 좋은 점수)
    sort(decks.begin(), decks.end(), [](const Deck& a, const Deck& b) {
        return a.getAvgPlacement() < b.getAvgPlacement();
    });
    for (int i = 0; i < decks.size(); ++i) {
        decks[i].setScore(decks[i].getScore() + (decks.size() - i));
    }

    // Top 4 비율 기준 점수 부여
    sort(decks.begin(), decks.end(), [](const Deck& a, const Deck& b) {
        return a.getTop4Rate() > b.getTop4Rate();
    });
    for (int i = 0; i < decks.size(); ++i) {
        decks[i].setScore(decks[i].getScore() + (decks.size() - i));
    }

    // 최종 점수를 기준으로 덱 정렬
    sort(decks.begin(), decks.end(), [](const Deck& a, const Deck& b) {
        return a.getScore() > b.getScore();
    });

    // 추천 덱 출력
    cout << "추천 덱 (종합 기준):\n";
    decks[0].printDeckInfo();
}

// 모든 덱의 정보를 출력하는 함수
void listAllDecks(const vector<Deck>& decks) {
    cout << "모든 덱의 종류:\n";
    for (int i = 0; i < decks.size(); ++i) {
        cout << i + 1 << ". " << decks[i].getName() << "\n";
    }
    cout << "-----------------------\n";
}

// 특정 덱의 정보를 출력하는 함수
void viewDeckDetails(const vector<Deck>& decks) {
    int deckChoice;
    listAllDecks(decks);
    cout << "보고 싶은 덱의 번호를 입력하세요: ";
    cin >> deckChoice;

    if (deckChoice >= 1 && deckChoice <= decks.size()) {
        decks[deckChoice - 1].printDeckInfo();
    } else {
        cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
    }
}

// 챔피언을 입력받아 해당 챔피언이 포함된 덱을 추천하는 함수
void recommendDeckByChampion(const vector<Deck>& decks) {
    string champion;
    cout << "추천받고 싶은 챔피언 이름을 입력하세요: ";
    cin >> champion;

    bool found = false;
    for (const auto& deck : decks) {
        if (find(deck.getChampions().begin(), deck.getChampions().end(), champion) != deck.getChampions().end()) {
            deck.printDeckInfo();
            found = true;
        }
    }

    if (!found) {
        cout << "해당 챔피언을 포함한 덱이 없습니다." << endl;
    }
}

int main() {
    // 덱 데이터
    vector<Deck> decks = {
        {"차원문 덱", 15.3, 4.31, 52.5, 0.30, {"Ryze", "Taric", "TahmKench"}},
        {"달콤술사 전사 덱", 16.5, 4.43, 50.1, 0.30, {"Gwen", "Fiora", "Rakan"}},
        {"아르카나 폭파단 덱", 13.0, 4.66, 46.4, 0.17, {"Varus", "TahmKench", "Xerath"}},
        {"요정 쇄도자 덱", 14.1, 4.38, 52.2, 0.48, {"Kalista", "Rakan", "Milio"}}
    };

    int choice;
    while (true) {
        cout << "덱 추천 프로그램입니다. 옵션을 선택하세요:\n";
        cout << "1. 최고 승률 덱 추천\n";
        cout << "2. 모든 덱 정보 보기\n";
        cout << "3. 챔피언 기반 덱 추천\n";
        cout << "4. 종료\n";
        cout << "선택: ";
        cin >> choice;

        if (choice == 1) {
            recommendDeck(decks);
        } else if (choice == 2) {
            viewDeckDetails(decks);
        } else if (choice == 3) {
            recommendDeckByChampion(decks);
        } else if (choice == 4) {
            cout << "프로그램을 종료합니다." << endl;
            break;
        } else {
            cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
        }
    }

    return 0;
}
