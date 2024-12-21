#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

// 클래스와 함수 정의 전에 함수 프로토타입 선언
class Deck;
void RecommendDeck(vector<Deck>& decks);
void ViewDeckDetails(const vector<Deck>& decks);
void RecommendDeckByChampion(const vector<Deck>& decks);
void RecommendItemsByChampion(const unordered_map<string, vector<string>>& champion_items);
void RecommendChampionsByItem(const unordered_map<string, vector<string>>& item_champions);
void AddItemsToDecks(vector<Deck>& decks);
void RecommendDeckByAugment(const vector<Deck>& decks, const unordered_map<string, vector<string>>& augment_decks);
void RecommendAugmentsForDeck(const Deck& deck, const unordered_map<string, vector<string>>& deck_augments);
vector<Deck> LoadDecksFromFile(const string& filename);
void AnalyzeMistakesWithDeck(const string& filename, const vector<Deck>& decks);
void RecommendPosition(const string& deck_name, const unordered_map<string, unordered_map<string, pair<int, int>>>& deck_positions);
void DisplayBoard(const unordered_map<string, pair<int, int>>& positions);

// 아이템 클래스 정의
class Item {
private:
    string name;
    string type;

public:
    Item(string n, string t) : name(n), type(t) {}

    string GetName() const { return name; }
    string GetType() const { return type; }

    void PrintItemInfo() const {
        cout << "아이템 이름: " << name << ", 유형: " << type << "\n";
    }
};

// 덱 클래스 정의
class Deck {
private:
    string name;
    double win_rate;
    double avg_placement;
    double top4_rate;
    double usage_count;
    int score;
    vector<string> champions;
    vector<Item> recommended_items;

public:
    // 생성자
    Deck(string n, double wr, double ap, double t4r, double uc, vector<string> champs)
        : name(n), win_rate(wr), avg_placement(ap), top4_rate(t4r), usage_count(uc), champions(champs), score(0) {}

    // 멤버 변수 접근을 위한 getter 함수
    string GetName() const { return name; }
    double GetWinRate() const { return win_rate; }
    double GetAvgPlacement() const { return avg_placement; }
    double GetTop4Rate() const { return top4_rate; }
    double GetUsageCount() const { return usage_count; }
    int GetScore() const { return score; }
    const vector<string>& GetChampions() const { return champions; }
    const vector<Item>& GetRecommendedItems() const { return recommended_items; }

    // 점수 설정 메서드
    void SetScore(int new_score) {
        score = new_score;
    }

    void SetScore(){
        // 가중치 설정
        const double win_rate_weight = 0.4;
        const double avg_placement_weight = 0.3;
        const double top4_rate_weight = 0.2;
        const double usage_count_weight = 0.1;

        // 기준값
        const double max_win_rate = 100.0; // 승률 최대값 100%
        const double min_avg_placement = 1.0; // 평균 등수 최소값 1.0
        const double max_top4_rate = 100.0; // Top4 비율 최대값 100%
        const double max_usage_count = 10.0; // 사용 비율 최대값 (가정)

        // 점수 계산 (0 ~ 100 범위로 조정)
        double win_rate_score = (win_rate / max_win_rate) * 100 * win_rate_weight;
        double avg_placement_score = ((4.0 - avg_placement) / (4.0 - min_avg_placement)) * 100 * avg_placement_weight;
        double top4_rate_score = (top4_rate / max_top4_rate) * 100 * top4_rate_weight;
        double usage_count_score = (usage_count / max_usage_count) * 100 * usage_count_weight;

        // 총 점수 계산
        score = static_cast<int>(win_rate_score + avg_placement_score + top4_rate_score + usage_count_score);
    }

    // 추천 아이템 추가 메서드
    void AddRecommendedItem(const Item& itm) {
        recommended_items.push_back(itm);
    }

    // 덱 정보 출력 메서드
    void PrintDeckInfo() const {
        string tier;
        if (score >= 13) {
            tier = "S";
        } else if (score >= 9) {
            tier = "A";
        } else if (score >= 6) {
            tier = "B";
        } else {
            tier = "C";
        }
        cout << "덱 이름: " << name << "\n";
        cout << "승률: " << win_rate << "%\n";
        cout << "평균 등수: " << avg_placement << "\n";
        cout << "Top 4 비율: " << top4_rate << "%\n";
        cout << "사용 비율: " << usage_count << "\n";
        cout << "티어: " << tier << "\n";
        cout << "구성 챔피언: ";
        for (size_t i = 0; i < champions.size(); ++i) {
            cout << champions[i];
            if (i < champions.size() - 1) {
                cout << " / ";
            }
        }
        cout << "\n추천 아이템: ";
        for (size_t i = 0; i < recommended_items.size(); ++i) {
            cout << recommended_items[i].GetName();
            if (i < recommended_items.size() - 1) {
                cout << " / ";
            }
        }
        cout << "\n-----------------------\n";
    }
};

// 최고 승률 덱 추천 기능 구현 함수 정의
void RecommendDeck(vector<Deck>& decks) {
    if (decks.empty()) {
        cout << "덱 정보가 없습니다." << endl;
        return;
    }
    sort(decks.begin(), decks.end(), [](const Deck& a, const Deck& b) {
        return a.GetScore() > b.GetScore();
    });
    cout << "추천 덱 (승률 기준):\n";
    decks[0].PrintDeckInfo();
}

// 모든 덱 정보 보기 기능 구현 함수 정의
void ViewDeckDetails(const vector<Deck>& decks) {
    if (decks.empty()) {
        cout << "덱 정보가 없습니다." << endl;
        return;
    }
    for (const auto& deck : decks) {
        deck.PrintDeckInfo();
    }
}

// 챔피언 기반 덱 추천 기능 구현 함수 정의
void RecommendDeckByChampion(const vector<Deck>& decks) {
    string champion;
    cout << "추천받고 싶은 챔피언 이름을 입력하세요: ";
    cin.ignore();
    getline(cin, champion);

    bool found = false;
    for (const auto& deck : decks) {
        if (find(deck.GetChampions().begin(), deck.GetChampions().end(), champion) != deck.GetChampions().end()) {
            deck.PrintDeckInfo();
            found = true;
        }
    }
    if (!found) {
        cout << "해당 챔피언을 포함한 덱 정보가 없습니다." << endl;
    }
}

// 챔피언에 맞는 아이템 추천 기능 구현 함수 정의
void RecommendItemsByChampion(const unordered_map<string, vector<string>>& champion_items) {
    string champion;
    cout << "추천받고 싶은 챔피언 이름을 입력하세요: ";
    cin.ignore();
    getline(cin, champion);

    auto it = champion_items.find(champion);
    if (it != champion_items.end()) {
        cout << "챔피언 " << champion << "에 추천되는 아이템:\n";
        for (const auto& item : it->second) {
            cout << item << "\n";
        }
    } else {
        cout << "해당 챔피언에 대한 아이템 정보가 없습니다." << endl;
    }
    cout << "-----------------------\n";
}

// 아이템에 맞는 챔피언 추천 기능 구현 함수 정의
void RecommendChampionsByItem(const unordered_map<string, vector<string>>& item_champions) {
    string item;
    cout << "추천받고 싶은 아이템 이름을 입력하세요: ";
    cin.ignore();
    getline(cin, item);

    auto it = item_champions.find(item);
    if (it != item_champions.end()) {
        cout << "아이템 " << item << "에 맞는 챔피언:\n";
        for (const auto& champion : it->second) {
            cout << champion << "\n";
        }
    } else {
        cout << "해당 아이템에 대한 챔피언 정보가 없습니다." << endl;
    }
    cout << "-----------------------\n";
}

// 덱에 어울리는 증강체 추천 기능 구현 함수 정의
void RecommendAugmentsForDeck(const Deck& deck, const unordered_map<string, vector<string>>& deck_augments) {
    auto it = deck_augments.find(deck.GetName());
    if (it != deck_augments.end()) {
        cout << "덱 " << deck.GetName() << "에 추천되는 증강체:\n";
        for (const auto& augment : it->second) {
            cout << augment << "\n";
        }
    } else {
        cout << "해당 덱에 대한 증강체 정보가 없습니다." << endl;
    }
    cout << "-----------------------\n";
}

// 파일에서 덱 데이터를 읽는 함수
vector<Deck> LoadDecksFromFile(const string& filename) {
    vector<Deck> decks;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "파일을 열 수 없습니다: " << filename << endl;
        return decks;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string name, win_rate_str, avg_placement_str, top4_rate_str, usage_count_str, champions_str;

        // 데이터 읽기
        getline(ss, name, ',');
        getline(ss, win_rate_str, ',');
        getline(ss, avg_placement_str, ',');
        getline(ss, top4_rate_str, ',');
        getline(ss, usage_count_str, ',');
        getline(ss, champions_str, ',');

        // 숫자로 변환
        double win_rate = stod(win_rate_str);
        double avg_placement = stod(avg_placement_str);
        double top4_rate = stod(top4_rate_str);
        double usage_count = stod(usage_count_str);

        // 챔피언 리스트 분리
        vector<string> champions;
        stringstream champs_ss(champions_str);
        string champion;
        while (getline(champs_ss, champion, '/')) {
            champions.push_back(champion);
        }

        // 덱 생성 및 점수 계산
        decks.emplace_back(name, win_rate, avg_placement, top4_rate, usage_count, champions);
        decks.back().SetScore(); // 점수 자동 계산
    }

    file.close();
    return decks;
}



//개인 피드백 제공
void AnalyzeMistakesWithDeck(const string& filename, const vector<Deck>& decks) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "사용자 데이터를 열 수 없습니다: " << filename << endl;
        return;
    }

    unordered_map<string, int> mistake_summary;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string deck_name, champion_name, item_name;

        // CSV 데이터 읽기
        getline(ss, deck_name, ',');
        getline(ss, champion_name, ',');
        getline(ss, item_name, ',');

        // 디버깅: 읽은 데이터 확인
        cout << "읽은 데이터 -> 덱: " << deck_name << ", 챔피언: " << champion_name << ", 아이템: " << item_name << endl;

        // 덱 확인
        auto it = find_if(decks.begin(), decks.end(), [&deck_name](const Deck& d) {
            return d.GetName() == deck_name;
        });

        if (it != decks.end()) {
            const Deck& deck = *it;

            // 챔피언 확인
            bool champion_found = find(deck.GetChampions().begin(), deck.GetChampions().end(), champion_name) != deck.GetChampions().end();
            cout << "챔피언 매칭 결과: " << (champion_found ? "매칭됨" : "매칭되지 않음") << endl;

            // 아이템 확인
            bool item_found = false;
            for (const auto& item : deck.GetRecommendedItems()) {
                if (item.GetName() == item_name) {
                    item_found = true;
                    break;
                }
            }
            cout << "아이템 매칭 결과: " << (item_found ? "매칭됨" : "매칭되지 않음") << endl;

            // 결과 저장
            if (!champion_found) {
                mistake_summary["챔피언 잘못 선택"]++;
            }
            if (!item_found) {
                mistake_summary["아이템 잘못 선택"]++;
            }
        } else {
            mistake_summary["알 수 없는 덱"]++;
            cout << "알 수 없는 덱: " << deck_name << endl;
        }
    }

    // 결과 출력
    cout << "실수 분석 결과:\n";
    if (mistake_summary.empty()) {
        cout << "실수가 없습니다." << endl;
    } else {
        for (const auto& entry : mistake_summary) {
            cout << entry.first << " - " << entry.second << "회\n";
        }
    }
    cout << "-----------------------\n";

    file.close();
}

// 시각적 배치도 출력 함수
void DisplayBoard(const unordered_map<string, pair<int, int>>& positions) {
    const int rows = 4, cols = 7; // 4x7 그리드
    vector<vector<string>> board(rows, vector<string>(cols, " . "));

    for (const auto& [champion, pos] : positions) {
        int x = pos.first - 1; // 1-based to 0-based indexing
        int y = pos.second - 1;
        if (x >= 0 && x < rows && y >= 0 && y < cols) {
            board[x][y] = champion.substr(0, 3); // 챔피언 이름 줄임
        }
    }

    cout << "\n------- 시각적 배치도 -------\n";
    for (const auto& row : board) {
        for (const auto& cell : row) {
            cout << cell << " ";
        }
        cout << "\n";
    }
    cout << "-----------------------------\n";
}

// 덱별 배치 추천 함수
void RecommendPosition(const string& deck_name, const unordered_map<string, unordered_map<string, pair<int, int>>>& deck_positions) {
    auto it = deck_positions.find(deck_name);
    if (it != deck_positions.end()) {
        cout << deck_name << " 배치도 추천:\n";
        DisplayBoard(it->second);
    } else {
        cout << "해당 덱에 대한 배치도 정보가 없습니다.\n";
    }
}
       
int main() {
    // txt 파일에서 덱 데이터 읽기
    string filename = "decks.txt";
    vector<Deck> decks = LoadDecksFromFile(filename);

    if (decks.empty()) {
        cout << "덱 데이터가 없습니다." << endl;
        return 0;
    }

    unordered_map<string, unordered_map<string, pair<int, int>>> deck_positions = {
        {"차원문 덱", {{"Ryze", {4, 1}}, {"Taric", {1, 3}}, {"TahmKench", {1, 5}}}},
        {"달콤술사 전사 덱", {{"Gwen", {1, 2}}, {"Fiora", {1, 3}}, {"Rakan", {1, 4}}}},
        {"아르카나 폭파단 덱", {{"Varus", {4, 1}}, {"Xerath", {4, 7}}, {"TahmKench", {1, 4}}}},
        {"요정 쇄도자 덱", {{"Kalista", {4, 1}}, {"Rakan", {1, 2}}, {"Milio", {4, 3}}}}
    };

    // 추천 아이템 추가
    unordered_map<string, vector<Item>> deck_items = {
        {"차원문 덱", {Item("staff", "magic"), Item("mana essence", "magic")}},
        {"달콤술사 전사 덱", {Item("sword", "physical"), Item("chain vest", "armor")}},
        {"아르카나 폭파단 덱", {Item("staff", "magic"), Item("giant belt", "health")}},
        {"요정 쇄도자 덱", {Item("bow", "ranged"), Item("mana essence", "magic")}}
    };

    for (auto& deck : decks) {
        auto it = deck_items.find(deck.GetName());
        if (it != deck_items.end()) {
            for (const auto& itm : it->second) {
                deck.AddRecommendedItem(itm);
            }
        }
    }

    // 덱별 증강체 추천 데이터 정의
    unordered_map<string, vector<string>> deck_augments = {
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
        cout << "6. 개인 피드백 제공\n";
        cout << "7. 배치도 추천\n";
        cout << "8. 종료\n";
        cout << "선택: ";
        cin >> choice;

        if (choice == 1) {
            RecommendDeck(decks);
        } else if (choice == 2) {
            ViewDeckDetails(decks);
        } else if (choice == 3) {
            RecommendDeckByChampion(decks);
        } else if (choice == 4) {
            int sub_choice;
            cout << "1. 챔피언에 맞는 아이템 추천\n";
            cout << "2. 아이템에 맞는 챔피언 추천\n";
            cout << "선택: ";
            cin >> sub_choice;

            if (sub_choice == 1) {
                unordered_map<string, vector<string>> champion_items = {
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
                RecommendItemsByChampion(champion_items);
            } else if (sub_choice == 2) {
                unordered_map<string, vector<string>> item_champions = {
                    {"staff", {"Ryze", "Gwen", "Xerath"}},
                    {"mana essence", {"Ryze", "Varus", "Xerath", "Milio"}},
                    {"chain vest", {"Taric"}},
                    {"giant belt", {"TahmKench", "Rakan"}},
                    {"sword", {"Fiora", "Varus"}},
                    {"bow", {"Kalista"}}
                };
                RecommendChampionsByItem(item_champions);
            } else {
                cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
            }
        } else if (choice == 5) {
            int deck_choice;
            cout << "증강체를 추천받고 싶은 덱을 선택하세요:\n";
            for (size_t i = 0; i < decks.size(); ++i) {
                cout << i + 1 << ". " << decks[i].GetName() << "\n";
            }
            cout << "선택: ";
            cin >> deck_choice;

            if (deck_choice >= 1 && deck_choice <= decks.size()) {
                RecommendAugmentsForDeck(decks[deck_choice - 1], deck_augments);
            } else {
                cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
            }
        } else if (choice == 6) {
            string user_data_file;
            cout << "사용자 데이터 파일 이름을 입력하세요: ";
            cin >> user_data_file;
            AnalyzeMistakesWithDeck(user_data_file, decks); 
        } else if (choice == 7) {
            cout << "배치도를 추천받고 싶은 덱을 선택하세요:\n";
            for (size_t i = 0; i < decks.size(); ++i) {
                cout << i + 1 << ". " << decks[i].GetName() << "\n";
            }
            cout << "선택: ";
            int deck_choice;
            cin >> deck_choice;

            if (deck_choice >= 1 && deck_choice <= decks.size()) {
                RecommendPosition(decks[deck_choice - 1].GetName(), deck_positions);
            } else {
                cout << "잘못된 입력입니다. 다시 시도하세요.\n";
            }
        } else if (choice == 8) {
            cout << "프로그램을 종료합니다." << endl;
            break;
        } else {
            cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
        }

    }
    return 0;
}
