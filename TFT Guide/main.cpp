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
void recommendDeckByAugment(const vector<Deck>& decks, const unordered_map<string, vector<string>>& augmentDecks);
void recommendAugmentsForDeck(const Deck& deck, const unordered_map<string, vector<string>>& deckAugments);

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

// 증강체에 어울리는 덱 추천 기능 구현 함수 정의
void recommendDeckByAugment(const vector<Deck>& decks, const unordered_map<string, vector<string>>& augmentDecks) {
    string augment;
    cout << "추천받고 싶은 증강체 이름을 입력하세요: ";
    cin.ignore();
    getline(cin, augment);

    auto it = augmentDecks.find(augment);
    if (it != augmentDecks.end()) {
        cout << "증강체 " << augment << "에 맞는 덱:\n";
        for (const auto& deckName : it->second) {
            for (const auto& deck : decks) {
                if (deck.getName() == deckName) {
                    deck.printDeckInfo();
                }
            }
        }
    } else {
        cout << "해당 증강체에 대한 덱 정보가 없습니다." << endl;
    }
}

// 덱에 어울리는 증강체 추천 기능 구현 함수 정의
void recommendAugmentsForDeck(const Deck& deck, const unordered_map<string, vector<string>>& deckAugments) {
    auto it = deckAugments.find(deck.getName());
    if (it != deckAugments.end()) {
        cout << "덱 " << deck.getName() << "에 추천되는 증강체:\n";
        for (const auto& augment : it->second) {
            cout << augment << "\n";
        }
    } else {
        cout << "해당 덱에 대한 증강체 정보가 없습니다." << endl;
    }
}

// 최고 승률 덱 추천 기능 구현 함수 정의
void recommendDeck(vector<Deck>& decks) {
    if (decks.empty()) {
        cout << "덱 정보가 없습니다." << endl;
        return;
    }
    sort(decks.begin(), decks.end(), [](const Deck& a, const Deck& b) {
        return a.getWinRate() > b.getWinRate();
    });
    cout << "추천 덱 (승률 기준):\n";
    decks[0].printDeckInfo();
}

// 모든 덱 정보 보기 기능 구현 함수 정의
void viewDeckDetails(const vector<Deck>& decks) {
    if (decks.empty()) {
        cout << "덱 정보가 없습니다." << endl;
        return;
    }
    for (const auto& deck : decks) {
        deck.printDeckInfo();
    }
}

// 챔피언 기반 덱 추천 기능 구현 함수 정의
void recommendDeckByChampion(const vector<Deck>& decks) {
    string champion;
    cout << "추천받고 싶은 챔피언 이름을 입력하세요: ";
    cin.ignore();
    getline(cin, champion);

    bool found = false;
    for (const auto& deck : decks) {
        if (find(deck.getChampions().begin(), deck.getChampions().end(), champion) != deck.getChampions().end()) {
            deck.printDeckInfo();
            found = true;
        }
    }
    if (!found) {
        cout << "해당 챔피언을 포함한 덱 정보가 없습니다." << endl;
    }
}

// 챔피언에 맞는 아이템 추천 기능 구현 함수 정의
void recommendItemsByChampion(const unordered_map<string, vector<string>>& championItems) {
    string champion;
    cout << "추천받고 싶은 챔피언 이름을 입력하세요: ";
    cin.ignore();
    getline(cin, champion);

    auto it = championItems.find(champion);
    if (it != championItems.end()) {
        cout << "챔피언 " << champion << "에 추천되는 아이템:\n";
        for (const auto& item : it->second) {
            cout << item << "\n";
        }
    } else {
        cout << "해당 챔피언에 대한 아이템 정보가 없습니다." << endl;
    }
}

// 아이템에 맞는 챔피언 추천 기능 구현 함수 정의
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

    // 덱별 증강체 추천 데이터 정의
    unordered_map<string, vector<string>> deckAugments = {
        {"차원문 덱", {"작은 친구들", "차원문 문장"}},
        {"달콤술사 전사 덱", {"캐러멜을 바른 아늑함", "달콤술사 문장", "큰 꾸러미"}},
        {"아르카나 폭파단 덱", {"아르카나 전달자", "아르카나 문장", "단결된 의지"}},
        {"요정 쇄도자 덱", {"왕실 근위대", "판도라의 아이템", "요정 문장"}}
    };

    int choice;
    while (true) {
        cout << "덱 추천 프로그램입니다. 옵션을 선택하세요:\n";
        cout << "1. 최고 승률 덱 추천\n";
        cout << "2. 모든 덱 정보 보기\n";
        cout << "3. 챔피언 기반 덱 추천\n";
        cout << "4. 아이템 추천\n";
        cout << "5. 덱에 맞는 증강체 추천\n";
        cout << "6. 종료\n";
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
                recommendItemsByChampion(championItems);
            } else if (subChoice == 2) {
                unordered_map<string, vector<string>> itemChampions = {
                    {"staff", {"Ryze", "Gwen", "Xerath"}},
                    {"mana essence", {"Ryze", "Varus", "Xerath", "Milio"}},
                    {"chain vest", {"Taric"}},
                    {"giant belt", {"TahmKench", "Rakan"}},
                    {"sword", {"Fiora", "Varus"}},
                    {"bow", {"Kalista"}}
                };
                recommendChampionsByItem(itemChampions);
            } else {
                cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
            }
        } else if (choice == 5) {
            int deckChoice;
            cout << "증강체를 추천받고 싶은 덱을 선택하세요:\n";
            for (size_t i = 0; i < decks.size(); ++i) {
                cout << i + 1 << ". " << decks[i].getName() << "\n";
            }
            cout << "선택: ";
            cin >> deckChoice;

            if (deckChoice >= 1 && deckChoice <= decks.size()) {
                recommendAugmentsForDeck(decks[deckChoice - 1], deckAugments);
            } else {
                cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
            }
        } else if (choice == 6) {
            cout << "프로그램을 종료합니다." << endl;
            break;
        } else {
            cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
        }
    }

    return 0;
}
