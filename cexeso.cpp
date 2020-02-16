#include <iostream> 
#include <string>
#include <list>
#include <vector>
#include <math.h>
#include <random>
#include <algorithm>
#include <chrono>
#include <thread>
#include <map>

void typeface(int glyphs){
    
    std::string glyphs_stringed = std::to_string(glyphs);
    std::map<char, std::vector<std::string> > glyphs_map;
    
    // finish numbers
    glyphs_map['1'] = {"__####___", "_##_##___", "____##___", "____##___", "_#######_"};
    glyphs_map['2'] = {"__#####__", "_##__###_", "____###__", "__###____", "_#######_"};
    glyphs_map['3'] = {"__#####__", "_##___##_", "_____##__", "_##___##_", "__#####__"};
    glyphs_map['4'] = {"___###___", "__###____", "_###_____", "########_", "____##___"};
    glyphs_map['5'] = {"___####__", "__##_____", "_######__", "______##_", "_######__"};
    glyphs_map['6'] = {"___###___", "__###____", "_#######_", "_##____##", "__#####__"};
    glyphs_map['7'] = {"_########", "_##__###_", "_____###_", "____###__", "___###___"};
    
    for (int y=0; y<5; y++){
        for (int x=0; x<glyphs_stringed.size(); x++) {
            std::cout << glyphs_map[glyphs_stringed[x]][y];
        }
        std::cout << '\n';
    }    
};

void clear_screen(){
    // prints 100 new lines
    std::cout << std::string(100, '\n');
};

void sleep(int x){
    std::this_thread::sleep_for(std::chrono::seconds(x));
};

void draw_2d_matrix(std::vector<char> cards){
    
    int i;
    const int length = cards.size();
    const int side = ceil(sqrt(length));
    std::cout << "  ";
    for (int i=0; i<side; i++) std::cout << i+1 << ' ';
    std::cout << '\n';
    for (int row=0; row<side; row++){
        std::cout << row+1 << ' ';
        for (int cell=0; cell<side; cell++) {
            i = row*side+cell;
            if (i > length-1){
                break;
            };
            std::cout << cards[i] << ' ';
        };
        std::cout << '\n';
    };
};

void draw_field(
        std::vector<char> &cards,
        std::vector<char> &backs,
        std::vector<int> &guess)
        {

    std::vector<char> overlapped;
    overlapped = backs;
    if (guess.size()){
        for (int i=0; i<guess.size(); i++){
        overlapped.at(guess[i]) = cards.at(guess[i]);
        }
    };
    draw_2d_matrix(overlapped);

};

int twoD_to_index(int cell, int row, int width) {
    return (cell-1)+(row-1)*width;
    };

int main() {

    int num_players;
    int pairs;
    int guess_x;
    int guess_y;
    int current_player;
    bool match;
    int twoD_pos_cur = -1;
    int twoD_pos_last = -1;

    std::vector<int> guess;
    std::string card_faces;
    std::vector<char> cards;
    std::vector<char> backs;
    std::cout << "number of players: ";
    std::cin >> num_players;
    std::vector<int> score;
    
    for (int i=0; i<num_players; i++) score.push_back(0);
    
    std::cout << "number of pairs: ";
    std::cin >> pairs;
    card_faces = "abcdefghij";
    card_faces = card_faces.substr(0, pairs);
    card_faces += card_faces;

    for (int i=0; i<card_faces.length(); i++){
        cards.push_back(card_faces[i]);
        backs.push_back('#');
        };
    
    std::random_device rd;
    std::shuffle(cards.begin(), cards.end(), rd);
    
    while (std::find(backs.begin(), backs.end(), '#') != backs.end()){
        std::cout << "player \n";
        typeface(current_player+1);
        std::cout << '\n';
        draw_field(cards, backs, guess);
        while (guess.size() < 2) {
            std::cout << "col: ";
            std::cin >> guess_x;
            std::cout << "row: ";
            std::cin >> guess_y;
            std::cout << '\n';
            twoD_pos_cur = twoD_to_index(guess_x, guess_y, ceil(sqrt(pairs*2)));
            // std::cout << guess_x << ' ' << guess_y << "two pos" << twoD_pos_cur << '\n';
            if (twoD_pos_cur != twoD_pos_last && backs[twoD_pos_cur] == '#') {
                guess.push_back(twoD_pos_cur);
                twoD_pos_last = twoD_pos_cur;
                draw_field(cards, backs, guess);
                }
            else {
                std::cout << "choose two different cards or yet hidden cards, please.\n";
            }
        }
        match = cards[guess[0]] == cards[guess[1]];
        if (match) {
            backs[guess[0]] = ' ';
            backs[guess[1]] = ' ';
            score[current_player] += 1;
        }
        else {
            current_player += 1;
            current_player = current_player % num_players;
        }
        sleep(2);
        clear_screen();
        guess.clear();
        twoD_pos_last = -1;
    };
    
    // last player is not the one who wins
    std::string winner;
    int biggestVal = 0;
    for (int i=0; i<score.size(); i++) {
        if (score[i] > biggestVal) {
            biggestVal = i;
        }
    };
    winner = std::to_string(biggestVal+1);

    std::cout << "player number " << winner << " wins!\n";
    
    return 0;
};