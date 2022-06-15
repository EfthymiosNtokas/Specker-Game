#include <iostream>
#include <bits/stdc++.h>
#include <stdexcept>
using namespace std;
class Move{
public:
    //Take sc coins from heap sh and put tc coins to heap th
    Move(int sh, int sc, int th, int tc): sh(sh), sc(sc), th(th), tc(tc){}
    int getSource() const{return sh;}
    int getSourceCoins() const{return sc;}
    int getTarget() const{return th;}
    int getTargetCoins() const{return tc;}
    friend ostream &operator <<(ostream &out, const Move &move){
        out << "takes " << move.sc << " coins from heap " << move.sh << " and ";
        out << "puts ";
        if(move.tc!=0){
            out << move.tc << " coins to heap " << move.th;
        }else{
            out << "nothing";
        }
        return out;
    }
private:
    int sh, sc, th, tc;
};
class State{
public:
    //State with h heaps, where the i-th heap stars with c[i] coins
    //A total of n players are in the game, numbered from 0 to n-1
    // and player 0 is the first to play
    State(int h, const int c[], int n){
        heaps = h;
        a = new int[h];
        players = n;
        sum = 0;
        current = 0;
        for(int i = 0; i < h; i++){a[i] = c[i]; sum+=c[i];}
    }
    ~State(){
        delete [] a;
    };
    void next(const Move &move) throw(logic_error){//go to next state based on move made
        int from = move.getSource(); int to = move.getTarget();
        int fromco = move.getSourceCoins(); int toco = move.getTargetCoins();
        if(from < 0 || from >=heaps ||  to < 0 || to >= heaps ){
            //current=(current+1)%players;
            throw logic_error("invalid heap");
        }else{
            if(fromco > a[from] || fromco <= 0 || toco >= fromco || toco < 0){
                //current=(current+1)%players; //maybe not
                throw logic_error("invalid heap");}
            else{
                a[from]-=fromco;
                a[to]+=toco;
                sum= sum+toco-fromco;
                current=(current+1)%players; //maybe not
            }
        }
    };
    bool winning() const{return sum == 0;};
    int getHeaps() const{return heaps;};
    int getCoins(int h) const throw(logic_error){

        if(h < 0 || h >= heaps){throw logic_error("invalid heap!");}
        else{
            return a[h];
        }
    }

    int getPlayers() const{return players;}
    int getPlaying() const{return current;}
    friend ostream &operator <<(ostream &out, const State &state ) {
        for (int i = 0; i < state.heaps - 1; i++) {
            out << state.a[i] << ", ";
        }
        out << state.a[state.heaps-1] << " with " << state.current << "/" << state.players;
        out << " playing next";
        return out;
    }
private:
    int players;
    int heaps;
    int *a;
    int sum;
    int current;
};
class Player{
public:
    Player(const string &n):name(n){}
    virtual ~Player(){

    };
    virtual const string &getType() const = 0;
    virtual Move play(const State &s) = 0;

    friend ostream &operator << (ostream &out, const Player &player){
        out << player.getType() << " player " << player.name;
        return out;
    };

private:
    string type;
    string name;
};
class GreedyPlayer:public Player{
public:
    GreedyPlayer(const string &n): Player(n) {}
    const string &getType() const override{
        return s;
    }
    Move play(const State &s) override{
        int num = s.getHeaps();
        int min_ind = 0;
        int maxi = s.getCoins(0);
        for(int i = 1; i < num; i++){
            if(s.getCoins(i) > maxi){
                maxi = s.getCoins(i);
                min_ind = i;
            }
        }
        Move m(min_ind, maxi, 0, 0);
        return m;

    }
private:
    string s = "Greedy";
};
class SpartanPlayer:public Player{
public:
    SpartanPlayer(const string &n): Player(n) {}
    const string &getType() const override{
        return s;
    }
    Move play(const State &s) override{
        int num = s.getHeaps();
        int min_ind = 0;
        int maxi = s.getCoins(0);
        for(int i = 1; i < num; i++){
            if(s.getCoins(i) > maxi){
                maxi = s.getCoins(i);
                min_ind = i;
            }
        }
        Move m(min_ind, 1, 0, 0);
        return m;

    }
private:
    string s = "Spartan";
};
class SneakyPlayer:public Player{
public:
    SneakyPlayer(const string &n): Player(n) {}
    const string &getType() const override{
        return s;
    }
    Move play(const State &s) override{
        int num = s.getHeaps();
        int min_ind = 0;
        int mini = s.getCoins(0);
        for(int i = 1; i < num; i++){
            if(mini == 0){
                min_ind = i;
                mini = s.getCoins(i);
            }
            if(s.getCoins(i) < mini && s.getCoins(i)!=0){
                mini = s.getCoins(i);
                min_ind = i;
            }
        }
        Move m(min_ind, mini,   0, 0);
        return m;

    }
private:
    string s = "Sneaky";
};
class RighteousPlayer:public Player{
public:
    RighteousPlayer(const string &n): Player(n) {}
    const string &getType() const override{
        return s;
    }
    Move play(const State &s) override{
        int num = s.getHeaps();
        int max_ind = 0;

        int maxi = s.getCoins(0);
        int mini = maxi;
        int min_ind =0;
        for(int i = 1; i < num; i++){
            if(s.getCoins(i) > maxi){
                maxi = s.getCoins(i);
                max_ind = i;
            }

            if(s.getCoins(i) < mini){
                mini = s.getCoins(i);
                min_ind = i;
            }

        }
        Move m( max_ind,(maxi+1)/2,  min_ind, (maxi+1)/2-1);
        return m;

    }
private:
    string s = "Righteous";
};
class Game{
public:
    Game(int heaps, int players){
        heapps = heaps; playerss = players;
        heaps_my = new int[heapps];

        iter = 0;
        cur_heap = 0;
        cur_player = 0;
    }
    ~Game(){ };
    void addHeap(int coins) throw(logic_error){
        if(cur_heap >= heapps){throw logic_error("invalid option");}
        else{
            heaps_my[cur_heap] =coins;
            cur_heap++;
            if(cur_heap == heapps){

                gamee = new State(heapps, heaps_my, playerss);

            }
        }

    }
    void addPlayer(Player *player) throw(logic_error){
        if(cur_player >= playerss){throw logic_error("invalid option");}
        else{
            players_my.push_back(player) ;
            cur_player++;
        }
    }
    void play(ostream &out) throw(logic_error){
        if(cur_player!=playerss || cur_heap!=heapps){throw logic_error("invalid option");}
        else{
            while(!gamee->winning()){
                out << "State: " << *gamee << "\n"; // << "\n" ?
                Move m = players_my[iter]->play(*gamee);
                out << *players_my[iter]<< " " << m << "\n";
                gamee->next(m);
                iter = (iter+1)%playerss;
            }
            iter = (iter+playerss-1)%playerss;
            out << "State: " << *gamee << "\n";
            out << *players_my[iter] << " " << "wins" << "\n";

        }
    }
    int getPlayers() const{return playerss;}
    const Player *getPlayer(int p) const throw(logic_error){
        if(p < 0 || p >= cur_player){throw logic_error("invalid thing");}
        else {
            return players_my[p];
        }
    }
private:
    int heapps;
    int playerss;
    int cur_heap;
    int cur_player;
    int iter;
    int *heaps_my;
    vector<Player*> players_my;
    State *gamee;
};
int main(){
    //example of a game
    Game specker(3, 4);
    specker.addHeap(10);
    specker.addHeap(20);
    specker.addHeap(17);
    specker.addPlayer(new SneakyPlayer("Tom"));
    specker.addPlayer(new SpartanPlayer("Mary"));
    specker.addPlayer(new GreedyPlayer("Alan"));
    specker.addPlayer(new RighteousPlayer("Robin"));
    specker.play(cout);
    return 0;
}