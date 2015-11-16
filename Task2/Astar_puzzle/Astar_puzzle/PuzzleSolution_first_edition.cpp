
#include <vector>
#include <set>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <queue>
#include <stdlib.h>
#include <time.h>
#include <map>

clock_t _tested_time;

using std::pair;
using std::make_pair;
using std::vector;
using std::set;
using std::cin;
using std::cout;
using std::endl;
using std::queue;
using std::map;

enum TMoveType
{
    MOVE_DOWN,
    MOVE_UP,
    MOVE_RIGHT,
    MOVE_LEFT,
    ERROR_GAME
};

bool EqualBoolOperation(const bool a, const bool b){
    return (a && b) || (!a && !b);
}
/****************************************************/
class GamePosition;
class PositionParams;
class PuzzleSolution;

class PositionParams
{
public:
    PositionParams(const PositionParams & copied_params);
    PositionParams(const GamePosition & game, PuzzleSolution * parent);
    ~PositionParams();
    
    inline int GetPotential() const{
        return potential;
    }
    void SetPotential(const int new_potential){
        potential = new_potential;
    }
    
    inline int GetDistance() const{
        return distance;
    }
    void SetDistance(const int new_distance){
        distance = new_distance;
    }
    
private:
    int potential;
    int distance;
    
};

class GamePosition
{
public:
    GamePosition(const vector<short> & game_position);
    GamePosition(const GamePosition & copied_game);
    GamePosition(const int game_size);
    ~GamePosition(){}
    void SetPotential(PositionParams & position_params,const GamePosition & finish_position ,const PuzzleSolution * parent) const;
    int GetInversions() const;
    
    inline size_t GetVectorSize() const{
        return position.size();
    }
    inline short AccessTo(const short index) const{
        return position.at(index);
    }
    inline short GetSpacePosition() const{
        return space_position;
    }
    inline void SetSpacePosition(short new_index){
        space_position = new_index;
    }
    inline const GamePosition * GetAncestor() const{
        return ancestor;
    }
    inline void SetAncestor(const GamePosition * new_ancestor){
        ancestor = new_ancestor;
    }
    inline void Swap(short first_ind, short second_ind){
        std::swap(position[first_ind], position[second_ind]);
    }
    inline bool IsReverse() const {
        return is_reverse_game_positon;
    }
    inline void SetReverse(bool is_reverse){
        is_reverse_game_positon = is_reverse;
    }
private:
    vector<short> position;
    short space_position;
    const GamePosition  * ancestor;
    bool is_reverse_game_positon; // true if this position was opened in reverse bfs
    
    bool IsNoInNeedRow(const int index , const GamePosition & finish_position, const PuzzleSolution * parent) const;
    bool IsNoInNeedColumn(const int index , const GamePosition & finish_position, const PuzzleSolution * parent) const;
    
    int GetManhattanDistance(const GamePosition & finish_position, const PuzzleSolution * parent) const;
    int GetLineConflictDistance(const GamePosition & finish_position, vector<bool> & used_cells,const PuzzleSolution * parent) const;
    int GetEdgeConflictDistance(const GamePosition & finish_position, vector<bool> & used_cells,const PuzzleSolution * parent) const;
    int GetSemenDistance(const GamePosition & finish_position, const PuzzleSolution * parent) const;
    int CountCornerConflict(const GamePosition & finish_position, const short first,const short second, const short edge,vector<bool> & used_cells) const;
};

class PuzzleSolution
{
public:
    PuzzleSolution(const int game_size);
    ~PuzzleSolution();
    vector<TMoveType> CalcTagSolution(const vector<short> & start_position);
    int GetSizeofGame() const {
        return size_of_game;
    }
    inline const GamePosition & GetStandartFinishPostion() const {
        return standart_finish_position;
    }
    inline short IsSpace() const {
        return _is_space;
    }
private:
    const int STRAIGHT;
    const int REVERSE;
    
    set<pair<PositionParams,GamePosition> > opened_queue[2];
    set<GamePosition> closed_queue;
    
    GamePosition standart_finish_position;
    GamePosition start_game_position;
    
    const int size_of_game;
    const short _is_space;
    
    vector<TMoveType> RestoreAnswer(const GamePosition & right_order, const GamePosition & reverse_order) const;
    bool IsSolutionExist(const GamePosition & start_position);
    vector<TMoveType> RepairAnswer(const GamePosition & finish) const;
    vector<TMoveType> DijkstraAlgorithm();
    bool VisitNeighbour(TMoveType move,GamePosition & next_position, const GamePosition & current_position);
    
    inline int NextTurn(const int turn) const {
        return (turn + 1) % 2 ;
    }
};


/****************************************************/
//position params methods
PositionParams::~PositionParams(){}

PositionParams::PositionParams(const PositionParams & copied_params)
{
    potential = copied_params.potential;
    distance = copied_params.distance;
}

PositionParams::PositionParams(const GamePosition & game_position, PuzzleSolution * parent)
{
    game_position.SetPotential(*this,parent->GetStandartFinishPostion(), parent);
    distance = -1;
}

bool operator < (const PositionParams & first , const PositionParams & second)
{
    if(first.GetDistance() + first.GetPotential() < second.GetDistance() + second.GetPotential()) return true;
    if(first.GetDistance() + first.GetPotential() > second.GetDistance() + second.GetPotential()) return false;
    return false;
}

bool operator == (const PositionParams & first, const PositionParams & second)
{
    if(first.GetDistance() + first.GetPotential() ==  second.GetDistance() + second.GetPotential()) return true;
    return false;
}

/****************************************************/
//Game Position methods
GamePosition::GamePosition(const int game_size)
{
    position.resize(game_size * game_size);
    
    for(int i  = 0; i < game_size*game_size; ++ i)
    {
        position[i] = i;
    }
    space_position = game_size * game_size - 1;
    is_reverse_game_positon = false;
    
}
GamePosition::GamePosition(const GamePosition & copied_game)
{
    space_position = copied_game.space_position;
    position = copied_game.position;
    ancestor = copied_game.ancestor;
    is_reverse_game_positon = copied_game.is_reverse_game_positon;
}

GamePosition::GamePosition(const vector<short> & start_position)
{
    short is_space = start_position.size() - 1;
    position = start_position;
    for(int i =0 ;i<position.size();++i)
    {
        if(position[i] == is_space)
        {
            space_position = i;
            break;
        }
    }
    ancestor = nullptr;
    is_reverse_game_positon = false;
}

bool operator == (const GamePosition & first, const GamePosition & second)
{
    if(first.IsReverse() != second.IsReverse()) return false;
    for (int i = 0; i<first.GetVectorSize(); ++i)
    {
        if(first.AccessTo(i) < second.AccessTo(i)) return false;
        if(first.AccessTo(i) > second.AccessTo(i)) return false;
    }
    return true;
}

bool operator < (const GamePosition & first, const GamePosition & second)
{
    for (int i = 0; i<first.GetVectorSize(); ++i)
    {
        if(first.AccessTo(i) < second.AccessTo(i)) return true;
        if(first.AccessTo(i) > second.AccessTo(i)) return false;
    }
    if(first.IsReverse() < second.IsReverse()) return true;
    return false;
}

int GamePosition::GetManhattanDistance(const GamePosition & finish_position, const PuzzleSolution * parent) const
{
    int X_potential = 0;
    int Y_potential = 0;
    for(int i = 0;i<GetVectorSize();++i)
    {
        if(position[i] == parent->IsSpace())
            continue;
        
        Y_potential += abs(finish_position.position[i] % parent->GetSizeofGame() - position[i] % parent->GetSizeofGame());
        X_potential += abs(finish_position.position[i]/parent->GetSizeofGame() - position[i]/ parent->GetSizeofGame());
    }
    int potential = X_potential + Y_potential;
    
    return potential;
}
bool GamePosition::IsNoInNeedColumn(const int index , const GamePosition &finish_position, const PuzzleSolution *parent) const
{
    int size_of_game = parent->GetSizeofGame();
    int column = index % size_of_game;
    for(int i = column; i <size_of_game * size_of_game ; i += size_of_game)
    {
        if(position[index] == finish_position.position[i]) return false;
    }
    return true;
}

bool GamePosition::IsNoInNeedRow(const int index , const GamePosition &finish_position, const PuzzleSolution *parent) const
{
    int size_of_game = parent->GetSizeofGame();
    int row = index / size_of_game;
    for(int i = row; i <size_of_game * (row + 1) ; i += 1)
    {
        if(position[index] == finish_position.position[i]) return false;
    }
    return true;
}

int GamePosition::GetSemenDistance(const GamePosition & finish_position, const PuzzleSolution * parent) const
{
    int potential = 0;
    for(int i = 0; i< position.size(); ++i)
    {
        if(IsNoInNeedRow(i,finish_position,parent)) ++potential;
        if(IsNoInNeedColumn(i,finish_position, parent)) ++potential;
    }

    return potential;
}

int GamePosition::GetLineConflictDistance(const GamePosition & finish_position, vector<bool> & used_cells, const PuzzleSolution * parent) const
{
    int potential = 0;
    int size_of_game = parent->GetSizeofGame();
    
    vector<short> need(finish_position.position.size());
    for(int i  = 0 ; i < need.size(); ++i){
        need[finish_position.position[i]] = i;
    }
    
    for(int str = 0; str < size_of_game ; ++str)
    {
        for(int i = str * size_of_game; i < (str+1) * size_of_game -1; ++i)
        {
            if(position[i] == parent->IsSpace()) continue;
            
            for(int j = i + 1; j < (str+1) * size_of_game; ++j)
            {
                if(position[j] == parent->IsSpace()) continue;
                
                if(need[position[i]] / size_of_game == str &&
                   need[position[j]] / size_of_game == str
                   && need[position[i]] < need[position[j]]) potential += 2;
            }
        }
    }
    
    for(int col = 0; col < size_of_game ; ++col)
    {
        for(int i = col; i < size_of_game * size_of_game; i += size_of_game)
        {
            if(position[i] == parent->IsSpace()) continue;
            
            for(int j = i + size_of_game; j < size_of_game * size_of_game; j += size_of_game)
            {
                if(position[j] == parent->IsSpace()) continue;
                
                if(need[position[i]] % size_of_game == col &&
                   need[position[j]] % size_of_game == col
                   && need[position[i]] < need[position[j]]) potential += 2;
            }
        }
    }
    
    return potential;
}
int GamePosition::CountCornerConflict(const GamePosition & finish_position,
                                      const short first,
                                      const short second,
                                      const short edge,
                                      vector<bool> & used_cells) const
{
    if(position[first] == finish_position.position[first]
       && position[second] == finish_position.position[second]
       && position[edge] != finish_position.position[edge]
       && !used_cells[first] && !used_cells[second] && !used_cells[edge])
    {
        used_cells[first] = true;
        used_cells[second] = true;
        used_cells[edge] = true;
        return 2;
    }
    return 0;
}
int GamePosition::GetEdgeConflictDistance(const GamePosition & finish_position, vector<bool> & used_cells, const PuzzleSolution * parent) const
{
    int answer = 0;
    const int game_size = parent->GetSizeofGame();
    
    answer += CountCornerConflict(finish_position,1, game_size, 0, used_cells); // left up corner
    answer += CountCornerConflict(finish_position,game_size - 2, 2 * game_size - 1, game_size - 1, used_cells); //right up corner
    answer += CountCornerConflict(finish_position,game_size* (game_size-2), game_size * (game_size-1) + 1 , game_size * (game_size-1) , used_cells); // left down corner
    answer += CountCornerConflict(finish_position,game_size * (game_size - 1) - 1, game_size * game_size - 2, game_size * game_size - 1, used_cells); // right down corner
    
    return answer;
    
}

void GamePosition::SetPotential(PositionParams & position_params, const GamePosition & finish_position,const PuzzleSolution * parent) const
{
    vector<bool>  used_cells(parent->GetSizeofGame(),false);
    int potential = 0;
    
    potential += GetManhattanDistance(finish_position,parent);
   // potential += GetLineConflictDistance(finish_position,used_cells, parent);
    potential += GetEdgeConflictDistance(finish_position,used_cells, parent);
    potential += GetSemenDistance(finish_position, parent);
    
    position_params.SetPotential(potential);
}

int GamePosition::GetInversions() const
{
    int inversions = 0;
    short is_space = position.size() - 1 ;
    for(int i = 0;i<position.size();++i)
    {
        if(position[i] == is_space) continue;
        for (int j=i+1; j<position.size(); ++j)
        {
            if(position[j] == is_space) continue;
            if(position[j]<position[i]) ++inversions;
        }
    }
    return inversions;
}


/****************************************************/
//puzzleSolution methods


PuzzleSolution::~PuzzleSolution(){}

PuzzleSolution::PuzzleSolution(const int game_size) :
closed_queue(),
STRAIGHT(0),
REVERSE(1),
size_of_game(game_size),
standart_finish_position(game_size),
_is_space(game_size * game_size - 1),
start_game_position(game_size)
{
    assert(game_size>0);
}

bool PuzzleSolution::IsSolutionExist(const GamePosition & start_position)
{
        if(size_of_game != 4) return true; // i can't check
    
        int number_of_space_str;
        int inversions = start_position.GetInversions();
    
        number_of_space_str = start_position.GetSpacePosition() / size_of_game + 1;
    
        return !((inversions + number_of_space_str)%2);
    
}

bool PuzzleSolution::VisitNeighbour(TMoveType move,GamePosition & next_position,const GamePosition & current_position)
{
    bool is_new = false;
    int direction = 0;
    
    if(move == MOVE_UP && current_position.GetSpacePosition() >= size_of_game){
        is_new = true;
        direction = -size_of_game;
    }
    if(move == MOVE_DOWN && current_position.GetSpacePosition() <= size_of_game * (size_of_game-1) - 1){
        is_new = true;
        direction = size_of_game;
    }
    if(move == MOVE_LEFT && current_position.GetSpacePosition() % size_of_game != 0){
        is_new = true;
        direction = -1;
    }
    if(move == MOVE_RIGHT && (current_position.GetSpacePosition() + 1) % size_of_game != 0){
        is_new = true;
        direction = + 1;
    }
    
    if(is_new)
    {
        next_position.Swap(current_position.GetSpacePosition(),current_position.GetSpacePosition() + direction);
        next_position.SetSpacePosition(current_position.GetSpacePosition() + direction);
    }
    
    return is_new;
}
vector<TMoveType> PuzzleSolution::RestoreAnswer(const GamePosition & right_order, const GamePosition & reverse_order) const
{
    GamePosition left_middle_pos(right_order);
    left_middle_pos.SetAncestor(nullptr);
    
    GamePosition right_middle_pos(reverse_order);
    right_middle_pos.SetAncestor(&left_middle_pos);
    
    vector <TMoveType> answers[3];
    answers[0] = RepairAnswer(right_order);
    answers[1] = RepairAnswer(right_middle_pos);
    answers[2] = RepairAnswer(reverse_order);
    
    for(int i = 0; i < answers[2].size(); ++ i)
    {
        switch (answers[2][i])
        {
            case MOVE_DOWN:
                answers[2][i] = MOVE_UP;
                break;
                
            case MOVE_UP:
                answers[2][i] = MOVE_DOWN;
                break;
                
            case MOVE_LEFT:
                answers[2][i] = MOVE_RIGHT;
                break;
                
            case MOVE_RIGHT:
                answers[2][i] = MOVE_LEFT;
                break;
            default:;
        }
    }
    
    for(int i = 0;i < answers[0].size()/2; ++ i)
    {
        TMoveType buf = answers[0][i];
        answers[0][i] = answers[0][answers[0].size() - i];
        answers[0][answers[0].size() - i] = buf;
    }
    
    for(int i = 1; i < 3; ++i)
    {
        for(int j = 0; j < answers[i].size(); ++j)
        {
            if(i == 1)answers[0].push_back(answers[i][j]);
            if(i == 2)answers[0].push_back(answers[i][j]);
        }
    }

    return answers[0];
}

vector<TMoveType> PuzzleSolution::DijkstraAlgorithm()
{
    int turn = STRAIGHT; // STRAIGHT / REVERSE
    while(1)
    {
        assert(!opened_queue[turn].empty());//something wrong
        set<pair<PositionParams,GamePosition> >::const_iterator current_iter = opened_queue[turn].begin(); // get min
        
        //check that this position wasn't viewed already
        if(closed_queue.find(current_iter->second) != closed_queue.end())
        {
            opened_queue[turn].erase(current_iter);
            continue;
        }
        
        //check if we find the route
        GamePosition reverse_of_find_position(current_iter->second);
        reverse_of_find_position.SetReverse(!reverse_of_find_position.IsReverse());
        auto reverse_iter = closed_queue.find(reverse_of_find_position);
        if(reverse_iter != closed_queue.end())
        {
            if(turn == STRAIGHT)
                return RestoreAnswer(current_iter->second, *reverse_iter);
            return RestoreAnswer(*reverse_iter, current_iter->second);
        }
        
        
        PositionParams current_params = current_iter->first;
        GamePosition buffer_current_position(current_iter->second); //this node was viewed;
        opened_queue[turn].erase(current_iter);
        const GamePosition & current_position = *(closed_queue.insert(buffer_current_position).first);
        
        TMoveType moves_to_neighbours[4] = {MOVE_LEFT,MOVE_RIGHT,MOVE_UP,MOVE_DOWN};
        for(int move_ind = 0;move_ind < 4;++move_ind)
        {
            
            GamePosition next_position = current_position;
            bool is_new = VisitNeighbour(moves_to_neighbours[move_ind],next_position,current_position);
            
            if(is_new == true)
            {
                if(closed_queue.find(next_position) != closed_queue.end()) continue;
                
                PositionParams next_params(next_position,this);
                if(next_position.IsReverse()){
                    next_position.SetPotential(next_params,start_game_position, this); // set reverse potential
//                    next_params.SetPotential(20);
                }
                
                next_params.SetDistance(current_params.GetDistance() + 1);
                next_position.SetAncestor(&current_position);
                
                opened_queue[turn].insert(make_pair(next_params,next_position));
            }
        }
        
        turn = NextTurn(turn);
    }
}

vector<TMoveType> PuzzleSolution::RepairAnswer(const GamePosition & finish) const
{
    vector<TMoveType> answer;
    const GamePosition * cur_pos;
    cur_pos = &finish;
    
    short last_space_position = cur_pos->GetSpacePosition();
    short delta_space_position;
    while(cur_pos->GetAncestor() != nullptr)
    {
        cur_pos = cur_pos->GetAncestor();
        delta_space_position = last_space_position - cur_pos->GetSpacePosition();
        
        if(delta_space_position == 1) answer.push_back(MOVE_RIGHT);
        if(delta_space_position == -1) answer.push_back(MOVE_LEFT);
        if(delta_space_position == size_of_game) answer.push_back(MOVE_DOWN);
        if(delta_space_position == - size_of_game) answer.push_back(MOVE_UP);
        
        last_space_position = cur_pos->GetSpacePosition();
    }
    return answer;
}

vector<TMoveType> PuzzleSolution::CalcTagSolution(const vector<short> & users_start_position) // general function
{
    assert (users_start_position.size() == size_of_game * size_of_game);
    
    vector<short> start_position(users_start_position);
    for(int i = 0;i< start_position.size(); ++ i)
    {
        if(start_position[i] == 0) start_position[i] = _is_space;
        else --start_position[i];
        
        assert(start_position[i]>=0 && start_position[i] < start_position.size());
    }
    
    GamePosition start_game_position(start_position);
    PositionParams start_position_params(start_game_position,this);
    start_position_params.SetDistance(0);
    start_game_position.SetReverse(false);
    
    this -> start_game_position = start_game_position;
    
    vector<short> finish_position(size_of_game * size_of_game);
    for(int i = 0;i <finish_position.size() ; ++i){
        finish_position[i] = i;
    }
    GamePosition finish_game_position(finish_position);
    PositionParams finish_position_params(finish_game_position,this);
    finish_game_position.SetReverse(true);
    finish_position_params.SetDistance(0);
    finish_game_position.SetPotential(finish_position_params, start_game_position, this);
    
    opened_queue[STRAIGHT].insert(make_pair(start_position_params,start_game_position));
    opened_queue[REVERSE].insert(make_pair(finish_position_params, finish_game_position));
    
    vector<TMoveType> answer;
    if(!IsSolutionExist(start_game_position)){
        answer.push_back(ERROR_GAME);
    }
    else{
        answer = DijkstraAlgorithm();
    }
    return  answer;
}

/****************************************************/
void SetRandomGame( vector<short> & game_position, const int size_of_game)
{
    int is_space = size_of_game * size_of_game - 1;
    for(int i = 0;i<game_position.size(); ++i)
    {
        game_position[i]= i+1;
    }
    short space_position = is_space;
    game_position[space_position] = 0;
    srandom((unsigned int)time(0));
    int number_of_steps = 20;
    
    for(int i = 0; i < number_of_steps; ++i)
    {
        int rand_step = random()%4;
        switch (rand_step)
        {
            case 0:  //move right
                if((space_position+1) % size_of_game != 0)
                {
                    std::swap(game_position[space_position],game_position[space_position+1]);
                    ++space_position;
                }
                break;
            case 1:
                if(space_position < size_of_game * (size_of_game-1)) //move down
                {
                    std::swap(game_position[space_position],game_position[space_position+size_of_game]);
                    space_position += size_of_game;
                }
                
                break;
            case 2:
                if(space_position % size_of_game != 0) // move left
                {
                    std::swap(game_position[space_position],game_position[space_position-1]);
                    --space_position;
                }
                break;
            case 3:
                if(space_position >= size_of_game)
                {
                    std::swap(game_position[space_position],game_position[space_position - size_of_game]);
                    space_position -= size_of_game;
                }
                
                break;
                
            default:
                std::cerr<<"\nERROR RANDOM"<<endl;
                exit(1);
        }
    }
}
/****************************************************/

#define _RUN_RANDOM_GENERATED_TEST_
int main()
{
    int game_size;
    vector<short> start_position_array;
    
    
    cin>>game_size;
    start_position_array.resize(game_size*game_size);
    
#ifdef _RUN_RANDOM_GENERATED_TEST_
    
    SetRandomGame(start_position_array, game_size);
    for(int i =0; i < game_size; ++i)
    {
        for(int j = 0; j < game_size; ++j)
        {
            cout<<start_position_array[i*game_size + j]<<" ";
        }
        cout<<endl;
    }
    
    
#else
    
    for(int i = 0;i<start_position_array.size();++i)
    {
        std::cin>>start_position_array[i];
    }
    
#endif
    
    PuzzleSolution GameSolver(game_size);
    
    _tested_time = clock();
    vector<TMoveType> answer = GameSolver.CalcTagSolution(start_position_array);
    _tested_time = clock() - _tested_time;
    
    cout<<answer.size()<<endl;
    for(long long i = 0;i < answer.size();++i)
    {
        /*interpretation answer*/
        switch (answer[i])
        {
            case MOVE_UP:
                cout<<"U";
                break;
            case MOVE_DOWN:
                cout<<"D";
                break;
            case MOVE_LEFT:
                cout<<"L";
                break;
            case MOVE_RIGHT:
                cout<<"R";
                break;
                
            default:
                break;
        }
    }
    cout<<endl;
    
    cout<<"\ntime: "<<_tested_time/(CLOCKS_PER_SEC*1.0)<<endl;
    return 0;
}
