#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;



class Cell {
public:
    Cell () : rating(0) {
        neighbors.resize(6);
    }

    void input() {
        cin >> cell_index >> richness;
        for (auto& n: neighbors) {
            cin >> n;
        }
    }
    int cell_index;
    int richness;
    int rating;
    vector<int> neighbors;
};


class Tree {
public:
    Tree () = default;
    Tree (int cell_index, int size, bool is_mine, bool is_dormant) :
        cell_index{cell_index}, size{size}, is_mine{is_mine}, is_dormant{is_dormant} {}
    void input() {
        cin >> cell_index >> size >> is_mine >> is_dormant;
    }
    int				cell_index;
    int				size;
    bool			is_mine;
    bool			is_dormant;
	bool operator < (const Tree& rhs) const {
            return this->cell_index < rhs.cell_index;
	}
};

class Fuck {
    public:
        int _pos;
        int _sight;
        int _tree;
        Fuck(int pos, int tree) : _pos(pos), _sight(0), _tree(tree) {}
        bool operator < (const Fuck& rhs) const {
                return this->_sight < rhs._sight;
	}
};

vector<Cell> map;

bool    epicComparison(Fuck &f1, Fuck &f2) {
    if (f1._sight == f2._sight)
        return (map[f1._pos].richness > map[f2._pos].richness);
    return (f1._sight < f2._sight);
}

enum action_type {
    WAIT = 0,
    SEED = 1,
    GROW = 2,
    COMPLETE = 3,
};

enum directions {
    NONE = -1,
    E  = 0,
    NE = 1,
    NW = 2,
    W  = 3,
    SW = 4,
    SE = 6
};

string    action_string(const action_type& type) {
    const static char* str[] = {
            [WAIT] = "WAIT",
            [SEED] = "SEED",
            [GROW] = "GROW",
            [COMPLETE] = "COMPLETE"
    };
    return str[type];
}

struct Action {
    action_type _type;
    int         _targetIdx{},
                _sourceIdx{};

    Action() : _type(WAIT) { }
    explicit Action(const action_type& type) : _type(type) { }
    Action(const action_type& type, const int& targetIdx, const int& sourceIdx = -1) : _type(type), _targetIdx(targetIdx), _sourceIdx(sourceIdx) { }
};

ostream& operator<<(ostream& out, const Action& a) {
    out << action_string(a._type);
    if (a._type == SEED)
        out << ' ' << a._sourceIdx;
    if (a._type != WAIT)
        out << ' ' << a._targetIdx;
    return (out);
}


class Game {
private:
        int day = 0;
        int nutrients = 0;
        vector<Tree> trees;
        vector<Action> actions;
        int mySun;
        int oppSun;
        int score;
        int oppScore;
        int oppIsWaiting;

public:
    void inputInitData() {
        int numberOfCells;
        cin >> numberOfCells;
        for (int i = 0; i < numberOfCells; i++) {
            Cell cell;
            cell.input();
            map.push_back(cell);
        }
    }

    void inputInfo() {
        // input game info
        cin >> day;
        cin >> nutrients;
        cin >> mySun >> score;
        cin >> oppSun >> oppScore >> oppIsWaiting;

        // input trees info
        trees.clear();
        int numberOfTrees;
        cin >> numberOfTrees;
        for (int i = 0; i < numberOfTrees; i++) {
            Tree tree;
            tree.input();
            trees.push_back(tree);
        }

        // input possible actions
        actions.clear();
        int numberOfPossibleMoves;
        cin >> numberOfPossibleMoves;
        for (int i = 0; i < numberOfPossibleMoves; i++) {
            string type;
            int arg1 = -1;
            int arg2 = -1;
            cin >> type;
            
            if (type == "WAIT") {
                actions.push_back(Action(WAIT, arg1, arg2));
            } else if (type == "COMPLETE") {
                cin >> arg1;
                actions.push_back(Action(COMPLETE, arg1, arg2));
            }
            else if (type == "GROW") {
                cin >> arg1;
                actions.push_back(Action(GROW, arg1, arg2));
            }
            else if (type == "SEED") {
                cin >> arg1;
                cin >> arg2;
                actions.push_back(Action(SEED, arg2, arg1));
            }
        }
    }

    /*
    ** My extermely cchaotic functions
    */


    
    int myTrees() {
		int ree = 0;
		for (int i = 0; i < trees.size(); i++) {
			if (trees[i].is_mine == true && trees[i].is_dormant == false)
				ree++;
		}
		return ree;
	}

	int myTrees(int size) {
		int ree = 0;
		for (int i = 0; i < trees.size(); i++) {
			if (trees[i].is_mine == true && trees[i].size == size)
				ree++;
		}
        return ree;
	}

    int isDormant(int pos) {
        for (int i = 0; i < trees.size(); i++) {
            if (trees[i].cell_index == pos && trees[i].is_dormant == false && trees[i].is_mine == true)
                return false;
        }
        return true;
    }

    bool    is_available(int pos) {
		for (int i = 0; i < trees.size(); i++) {
			if (trees[i].cell_index == pos)
				return (false);
		}
		return (true);
    }
 
	void	harvest() {
		cerr << "I am about to harvest" << endl;
		for (int i = 0; i < trees.size(); i++) {
			if (trees[i].is_mine == true && trees[i].is_dormant == false && trees[i].size == 3) {
				cout << "COMPLETE " << trees[i].cell_index << endl;
				return ;
			}
		}
        cout <<  "WAIT" << endl;
	}

    bool    gekkeBoom(int i) {
        if (trees[i].is_mine == true && trees[i].is_dormant == false)
            return true;
        return false;
    }

    void	growBadBois() {
		cerr << "I am going to grow some badboys" << endl;
		for (int i = 0; i < trees.size(); i++) {
            if (myTrees(3) > 3 || (day >= 20 && myTrees(3) > 0)) {
                harvest();
                return ;
            }
			else if (gekkeBoom(i) == true && trees[i].size == 0 && day < 20) {
                cout << "GROW " << trees[i].cell_index << endl;
                return ;
            } else if (gekkeBoom(i) == true && trees[i].size == 1 && (myTrees(2) < 3 || (day >= 20 && myTrees(3) == 0))) {
                cout << "GROW " << trees[i].cell_index << endl;
                return ;
            } else if (gekkeBoom(i) == true && trees[i].size == 2 && (myTrees(2) > 2 || (day >= 20 && myTrees(3) == 0))) {
                cout << "GROW " << trees[i].cell_index << endl;
                return ;
            }
        }
		cout << "WAIT" << endl;
	}

    bool    check_pos(int pos) {
        if (pos >= 0 && is_available(pos))
            return true;
        return false;
    }

    bool    checkNeighbors(int pos) {
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < trees.size(); j++) {
                if (trees[j].is_mine == true && trees[j].cell_index == map[pos].neighbors[i])
                    return true;
            }
        }
        return false;
    }

    void     plantHere(int start, int dist) {
        if (checkNeighbors(start) == false && (map[start].rating == -1 || map[start].rating > dist) && map[start].richness > 0 && check_pos(start) == true) {
            map[start].rating = dist;
        }
        else if (map[start].rating == -1)
            map[start].rating = 0;
        for (int i = 0; i < 6; i++) {
            int cur = map[start].neighbors[i];
            if (cur < 0)
                continue ;
            if (dist < 3)
                plantHere(cur, dist + 1);
        }
    }

    void    clearMap(void) {
        for (int i = 0; i < map.size(); i++) {
            map[i].rating = -1;
        }
    }

    int     checkDir(int pos, int dir) {
        int ret = 0;
        for (int i = 0; i < 3; i++) {
            pos = map[pos].neighbors[dir];
            if (pos == -1)
                return ret;
            for (int j = 0; j < trees.size(); j++) {
                if (trees[j].is_mine == true && trees[j].cell_index == pos)
                    ret++;
            }
        }
        return ret;
    }

    void    whereToPlant() {
        cerr << "kut plantjes" << endl;
        vector<Fuck> kutzooi;
        for (int t = 0; t < trees.size(); t++) {
            if (trees[t].is_mine == false || trees[t].is_dormant == true)
                continue ;
            clearMap();
            cerr << "\n\nTESTING TREE ON pos = " << trees[t].cell_index << endl;
            plantHere(trees[t].cell_index, 0);
            for (int i = 0; i < map.size(); i++) {
                if (map[i].rating > 1 && map[i].rating <= trees[t].size) {
                    cerr << "reeting = " << map[i].rating << " idx = " << map[i].cell_index << endl;
                    kutzooi.push_back(Fuck(map[i].cell_index, trees[t].cell_index));
                    // return map[i].cell_index;
                }
            }
        }
        for (int i = 0; i < kutzooi.size(); i++) {
            for (int j = 0; j < 6; j++) {
                kutzooi[i]._sight += checkDir(kutzooi[i]._pos, j);
            }
            cerr << "kz = " << i << ", sight = " << kutzooi[i]._sight << ", pos = " << kutzooi[i]._pos << endl;
        }
        if (kutzooi.size() == 0) {
            growBadBois();
            return ;
        }
        sort(begin(kutzooi), end(kutzooi));
        sort(begin(kutzooi), end(kutzooi), epicComparison);
        cout << "SEED " << kutzooi[0]._tree << ' ' << kutzooi[0]._pos << endl;
        return ;
    }
	

    void	compute_next_action() {
        cerr << "day = " << day << std::endl;
		sort(trees.begin(), trees.end());
        if ((mySun == 0 && myTrees(0) > 0))
            cout << "WAIT" << endl;
        else if (myTrees(0) < 2 && day < 18)
            whereToPlant();
        else
			growBadBois();
    }
};

int main()
{
    Game game;
    game.inputInitData();

    while (true) {
        game.inputInfo();
        
        game.compute_next_action();
    }
}
