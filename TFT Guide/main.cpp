#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

// 클래스와 함수 정의 전에 함수 프로토타입 선언
class Deck;
void recommendDeck(vector<Deck>& decks);
void viewDeckDetails(const vector<Deck>& decks);
void recommendDeckByChampion(const vector<Deck>& decks);
void recommendItemsByChampion(const unordered_map<string, vector<string>>& championItems);
void recommendChampionsByItem(const unordered_map<string, vector<string>>& itemChampions);
void addItemsToDecks(vector<Deck>& decks);

// 아이템 클래스 정의
class Item {
private:
    string name;
    string type;

public:
    Item(string n, string t) : name(n), type(t) {}

    string getName() const { return name; }
    string getType() const { return type; }

    void printItemInfo() const {
        cout << "아이템 이름: " << name << ", 유형: " << type << "\n";
    }
};

// 덱 클래스 정의
class Deck {
private:
    string name;
    double winRate;
    double avgPlacement;
    double top4Rate;
    double usageCount;
    int score;
    vector<string> champions;
    vector<Item> recommendedItems;

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
    const vector<Item>& getRecommendedItems() const { return recommendedItems; }

    // 점수 설정 메서드
    void setScore(int newScore) {
        score = newScore;
    }

    // 추천 아이템 추가 메서드
    void addRecommendedItem(const Item& item) {
        recommendedItems.push_back(item);
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
        for (size_t i = 0; i < champions.size(); ++i) {
            cout << champions[i];
            if (i < champions.size() - 1) {
                cout << " / ";
            }
        }
        cout << "\n추천 아이템: ";
        for (size_t i = 0; i < recommendedItems.size(); ++i) {
            cout << recommendedItems[i].getName();
            if (i < recommendedItems.size() - 1) {
                cout << " / ";
            }
        }
        cout << "\n-----------------------\n";
    }
};

// 모든 덱에 기본 아이템 추천 추가 함수 정의
void addItemsToDecks(vector<Deck>& decks) {
    // 아이템 목록 정의
    vector<Item> items = {
        Item("sword", "sword"),
        Item("staff", "staff"),
        Item("mana essence", "mana item"),
        Item("giant belt", "health item"),
        Item("chain vest", "armor"),
        Item("magic cloak", "magic resistance"),
        Item("bow", "attack speed item")
    };

    // 각 덱에 추천 아이템 추가
    for (auto& deck : decks) {
        if (deck.getName() == "차원문 덱") {
            deck.addRecommendedItem(items[1]); // 마법 지팡이
            deck.addRecommendedItem(items[2]); // 마나의 정수
            deck.addRecommendedItem(items[4]); // 쇠사슬 조끼
            deck.addRecommendedItem(items[5]); // 마법 저항 망토
        } else if (deck.getName() == "달콤술사 전사 덱") {
            deck.addRecommendedItem(items[0]); // 롱소드
            deck.addRecommendedItem(items[1]); // 마법 지팡이
            deck.addRecommendedItem(items[3]); // 거인의 허리띠
        } else if (deck.getName() == "아르카나 폭파단 덱") {
            deck.addRecommendedItem(items[0]); // 롱소드
            deck.addRecommendedItem(items[2]); // 마나의 정수
            deck.addRecommendedItem(items[4]); // 쇠사슬 조끼
            deck.addRecommendedItem(items[5]); // 마법 저항 망토
        } else if (deck.getName() == "요정 쇄도자 덱") {
            deck.addRecommendedItem(items[6]); // 곡궁
            deck.addRecommendedItem(items[3]); // 거인의 허리띠
        }
    }
}

// 덱 추천 기능 구현 함수 정의
void recommendDeck(vector<Deck>& decks) {
    // 덱 점수를 초기화
    for (auto& deck : decks) {
        deck.setScore(0);
    }

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

// 모든 덱의 정보를 출력하는 함수 정의
void viewDeckDetails(const vector<Deck>& decks) {
    int deckChoice;
    cout << "모든 덱의 종류:\n";
    for (int i = 0; i < decks.size(); ++i) {
        cout << i + 1 << ". " << decks[i].getName() << "\n";
    }
    cout << "-----------------------\n";
    cout << "보고 싶은 덱의 번호를 입력하세요: ";
    cin >> deckChoice;

    if (deckChoice >= 1 && deckChoice <= decks.size()) {
        decks[deckChoice - 1].printDeckInfo();
    } else {
        cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
    }
}

void recommendDeckByChampion(const vector<Deck>& decks) {
    string champion;
    cout << "추천받고 싶은 챔피언 이름을 입력하세요: ";
    cin >> champion;

    cout << "챔피언 " << champion << "과(와) 관련된 덱:\n";
    for (const auto& deck : decks) {
        const auto& champions = deck.getChampions();
        if (find(champions.begin(), champions.end(), champion) != champions.end()) {
            deck.printDeckInfo();
        }
    }
}


// 챔피언을 입력받아 해당 챔피언에 맞는 아이템을 추천하는 함수 정의
void recommendItemsByChampion(const unordered_map<string, vector<string>>& championItems) {
    string champion;
    cout << "추천받고 싶은 챔피언 이름을 입력하세요: ";
    cin >> champion;

    auto it = championItems.find(champion);
    if (it != championItems.end()) {
        cout << "챔피언 " << champion << "에게 추천되는 아이템:\n";
        for (const auto& item : it->second) {
            cout << item << "\n";
        }
    } else {
        cout << "해당 챔피언에 대한 아이템 정보가 없습니다." << endl;
    }
}

// 아이템을 입력받아 해당 아이템에 맞는 챔피언을 추천하는 함수 정의
void recommendChampionsByItem(const unordered_map<string, vector<string>>& itemChampions) {
    string item;
    cout << "추천받고 싶은 아이템 이름을 입력하세요: ";
    cin.ignore();
    getline(cin, item);

    auto it = itemChampions.find(item);
    if (it != itemChampions.end()) {
        cout << "아이템 " << item << "에 맞는 챔피언:\n";
        for (const auto& champion : it->second) {
            cout << champion << "\n";
        }
    } else {
        cout << "해당 아이템에 대한 챔피언 정보가 없습니다." << endl;
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

    // 덱에 아이템 추가
    addItemsToDecks(decks);

    // 챔피언별 아이템 추천 데이터 정의
    unordered_map<string, vector<string>> championItems = {
        {"Ryze", {"staff", "mana essence"}},
        {"Taric", {"chain vest", "magic cloak"}},
        {"TahmKench", {"giant belt"}},
        {"Gwen", {"staff"}},
        {"Fiora", {"sword"}},
        {"Rakan", {"giant belt"}},
        {"Varus", {"sword", "mana essence"}},
        {"Xerath", {"staff", "mana essence"}},
        {"Kalista", {"bow"}},
        {"Milio", {"mana essence"}}
    };

    // 아이템별 챔피언 추천 데이터 정의
    unordered_map<string, vector<string>> itemChampions = {
        {"staff", {"Ryze", "Gwen", "Xerath"}},
        {"mana essence", {"Ryze", "Varus", "Xerath", "Milio"}},
        {"chain vest", {"Taric", "아르카나 폭파단 덱의 챔피언"}},
        {"giant belt", {"TahmKench", "Rakan", "요정 쇄도자 덱의 챔피언"}},
        {"sword", {"Fiora", "Varus"}},
        {"bow", {"Kalista"}}
    };

    int choice;
    while (true) {
        cout << "덱 추천 프로그램입니다. 옵션을 선택하세요:\n";
        cout << "1. 최고 승률 덱 추천\n";
        cout << "2. 모든 덱 정보 보기\n";
        cout << "3. 챔피언 기반 덱 추천\n";
        cout << "4. 아이템 추천\n";
        cout << "5. 종료\n";
        cout << "선택: ";
        cin >> choice;

        if (choice == 1) {
            recommendDeck(decks);
        } else if (choice == 2) {
            viewDeckDetails(decks);
        } else if (choice == 3) {
            recommendDeckByChampion(decks);
        } else if (choice == 4) {
            int subChoice;
            cout << "1. 챔피언에 맞는 아이템 추천\n";
            cout << "2. 아이템에 맞는 챔피언 추천\n";
            cout << "선택: ";
            cin >> subChoice;

            if (subChoice == 1) {
                recommendItemsByChampion(championItems);
            } else if (subChoice == 2) {
                recommendChampionsByItem(itemChampions);
            } else {
                cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
            }
        } else if (choice == 5) {
            cout << "프로그램을 종료합니다." << endl;
            break;
        } else {
            cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
        }
    }

    return 0;
}
