#include <vector>
#include <set>
#include <cassert>
#include <algorithm>
#include <iostream>

#include <stdlib.h>
#include <time.h>


using std::pair;
using std::make_pair;
using std::vector;
using std::set;
using std::cin;
using std::cout;
using std::endl;

//static const int LIMIT = 100;
static const char IS_SPACE = 15; // code of space symbol

enum TMoveType
{
    MOVE_DOWN,
    MOVE_UP,
    MOVE_RIGHT,
    MOVE_LEFT,
    ERROR_GAME
};

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
    ~GamePosition(){}
    void SetPotential(PositionParams & position_params, const PuzzleSolution * parent) const;
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
private:
    vector<short> position;
    short space_position;
    const GamePosition  * ancestor;
    
    int GetManhattanDistance(const PuzzleSolution * parent) const;
    int GetLineConflictDistance(vector<bool> & used_cells,const PuzzleSolution * parent) const;
    int GetEdgeConflictDistance(vector<bool> & used_cells,const PuzzleSolution * parent) const;
    int CountCornerConflict(const short first,const short second, const short edge,vector<bool> & used_cells) const;
    
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
private:
    
    set<pair<PositionParams,GamePosition> > opened_queue;
    set<GamePosition> closed_queue;
    
    const int size_of_game;
    
    bool IsSolutionExist(const GamePosition & start_position);
    vector<TMoveType> RepairAnswer();
    bool DijkstraAlgorithm();
    bool VisitNeighbour(TMoveType move,GamePosition & next_position, const GamePosition & current_position);
};

/****************************************************/

PuzzleSolution::~PuzzleSolution(){}

PuzzleSolution::PuzzleSolution(const int game_size) :
opened_queue(),
closed_queue(),
size_of_game(game_size)
{
    assert(game_size>0);
}

int GamePosition::GetManhattanDistance(const PuzzleSolution * parent) const
{
    int X_potential = 0;
    int Y_potential = 0;
    for(int i = 0;i<GetVectorSize();++i)
    {
        if(position[i] == IS_SPACE)
            continue;
        
        Y_potential += abs(i % parent->GetSizeofGame() - position[i] % parent->GetSizeofGame());
        X_potential += abs(i/parent->GetSizeofGame() - position[i]/ parent->GetSizeofGame());
    }
    int potential = X_potential + Y_potential;
    
    return potential;
}

int GamePosition::GetLineConflictDistance(vector<bool> & used_cells, const PuzzleSolution * parent) const
{
    int answer = 0;
    int size_of_game = parent->GetSizeofGame();
    
    for(int i = 0; i< size_of_game; ++i)
    {
        for(int first = 0; first < size_of_game; ++first)
        {
            // rows distance
            // the right position of this (first)  cell is in this line
            if(position[i*size_of_game + first] >= i*size_of_game && position[i*size_of_game + first] < (i+1)*size_of_game)
            {
                for(int second = first + 1; second < size_of_game; ++second)
                {
                    //if the right position of this (second)  cell is in this line
                    if(position[i*size_of_game + second] >= i*size_of_game && position[i*size_of_game + second] < (i+1)*size_of_game)
                    {
                        if(position[i*size_of_game + first] > position[i*size_of_game + second])
                        {
                            if(position[i*size_of_game + first] != IS_SPACE && position[i*size_of_game + second] != IS_SPACE)
                            {
                                answer += 2;
                                used_cells[i*size_of_game + first] = true;
                                used_cells[i*size_of_game + second] = true;
                            }
                        }
                    }
                }
            }
            
            //column distance
            if(position[first*size_of_game + i] % size_of_game == i) // the right position of this cell is in this column
            {
                for(int second = first + 1; second < size_of_game; ++ second)
                {
                    if(position[second * size_of_game + i] % size_of_game == i)
                    {
                        if(position[first*size_of_game + i] > position[second*size_of_game + i])
                        {
                            if(position[first * size_of_game + i] != IS_SPACE && position[second * size_of_game + i] != IS_SPACE)
                            {
                                answer += 2;
                                used_cells[first * size_of_game + i] = true;
                                used_cells[second * size_of_game + i] = true;
                            }
                        }
                    }
                }
            }
        }
    }
    return answer;
    
}

int GamePosition::CountCornerConflict(const short first,const short second, const short edge,vector<bool> & used_cells) const
{
    if(position[first] == first && position[second] == second && position[edge] != edge
       && !used_cells[first] && !used_cells[second] && !used_cells[edge])
    {
        used_cells[first] = true;
        used_cells[second] = true;
        used_cells[edge] = true;
        return 2;
    }
    return 0;
}
int GamePosition::GetEdgeConflictDistance(vector<bool> & used_cells, const PuzzleSolution * parent) const
{
    int answer = 0;
    const int game_size = parent->GetSizeofGame();
    
    answer += CountCornerConflict(1, game_size, 0, used_cells); // left up corner
    answer += CountCornerConflict(game_size - 2, 2 * game_size - 1, game_size - 1, used_cells); //right up corner
    answer += CountCornerConflict(game_size* (game_size-2), game_size * (game_size-1) + 1 , game_size * (game_size-1) , used_cells); // left down corner
    answer += CountCornerConflict(game_size * (game_size - 1) - 1, game_size * game_size - 2, game_size * game_size - 1, used_cells); // right down corner
    
    return answer;
    
}

void GamePosition::SetPotential(PositionParams & position_params, const PuzzleSolution * parent) const
{
    vector<bool>  used_cells(parent->GetSizeofGame(),false);
    int potential = 0;
    
    potential += GetManhattanDistance(parent);
    potential += GetLineConflictDistance(used_cells, parent);
    potential += GetEdgeConflictDistance(used_cells, parent);
    
    position_params.SetPotential(potential);
}

/****************************************************/
//position params methods
PositionParams::~PositionParams(){}

PositionParams::PositionParams(const PositionParams & copied_params)
{
    potential = copied_params.potential;
    distance = copied_params.distance;
}

PositionParams::PositionParams(const GamePosition & game, PuzzleSolution * parent)
{
    game.SetPotential(*this, parent);
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

bool operator == (const GamePosition & first, const GamePosition & second)
{
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
    return false;
}

GamePosition::GamePosition(const GamePosition & copied_game)
{
    space_position = copied_game.space_position;
    position = copied_game.position;
    ancestor = copied_game.ancestor;
}

GamePosition::GamePosition(const vector<short> & start_position)
{
    position = start_position;
    for(int i =0 ;i<position.size();++i)
    {
        if(position[i] == IS_SPACE)
        {
            space_position = i;
            break;
        }
    }
    ancestor = nullptr;
}


int GamePosition::GetInversions() const
{
    int inversions = 0;
    
    for(int i = 0;i<position.size();++i)
    {
        if(position[i] == IS_SPACE) continue;
        for (int j=i+1; j<position.size(); ++j)
        {
            if(position[j] == IS_SPACE) continue;
            if(position[j]<position[i]) ++inversions;
        }
    }
    return inversions;
}

/****************************************************/
//puzzleSolution methods
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

bool PuzzleSolution::DijkstraAlgorithm()
{
    while(1)
    {
        assert(!opened_queue.empty());//something wrong
        set<pair<PositionParams,GamePosition> >::const_iterator current_iter = opened_queue.begin(); // get min
        if(current_iter->first.GetPotential() == 0) return true; //find finish
        
        PositionParams current_params = current_iter->first;
        GamePosition buffer_current_position(current_iter->second); //this node was viewed;
        opened_queue.erase(current_iter);
        const GamePosition & current_position = *(closed_queue.insert(buffer_current_position).first);
        
        //        cout<< current_params.GetDistance() << "\n" << current_params.GetPotential()<<endl;
        //        if(current_params.GetPotential() + current_params.GetDistance() > LIMIT) continue;
        
        TMoveType moves_to_neighbours[4] = {MOVE_LEFT,MOVE_RIGHT,MOVE_UP,MOVE_DOWN};
        for(int move_ind = 0;move_ind < 4;++move_ind)
        {
            
            GamePosition next_position = current_position;
            bool is_new = VisitNeighbour(moves_to_neighbours[move_ind],next_position,current_position);
            
            if(is_new == true)
            {
                if(closed_queue.find(next_position) != closed_queue.end()) continue;
                
                PositionParams next_params(next_position,this);
                next_params.SetDistance(current_params.GetDistance() + 1);
                next_position.SetAncestor(&current_position);
                
                opened_queue.insert(make_pair(next_params,next_position));
            }
        }
    }
}

vector<TMoveType> PuzzleSolution::RepairAnswer()
{
    vector<TMoveType> answer;
    const GamePosition * cur_pos;
    cur_pos = &(opened_queue.begin()->second);
    
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

vector<TMoveType> PuzzleSolution::CalcTagSolution(const vector<short> & const_start_position) // general function
{
    vector<short> start_position(const_start_position);
    
    for(int i = 0 ;i < start_position.size() ; ++ i)
    {
        --start_position[i];
        if(start_position[i] < 0 ) start_position[i] = IS_SPACE;
    }
    
    vector<TMoveType> answer;
    GamePosition start_game_position(start_position);
    PositionParams start_game_params(start_game_position,this);
    start_game_params.SetDistance(0);
    
    opened_queue.insert(make_pair(start_game_params,start_game_position));
    if(!IsSolutionExist(start_game_position)){
        answer.push_back(ERROR_GAME);
    }
    else{
        DijkstraAlgorithm();
        answer = RepairAnswer();
    }
    return  answer;
}

/****************************************************/
void SetRandomGame( vector<short> & game_position, const int size_of_game)
{
    for(int i = 0;i<game_position.size(); ++i)
    {
        game_position[i]= i + 1;
    }
    game_position[game_position.size() - 1] = 0;
    
    short space_position = IS_SPACE;
    srandom((unsigned int)time(0));
    int number_of_steps = 10;
    
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

//#define _RUN_RANDOM_GENERATED_TEST_
int main()
{
    const int game_size = 4;
    vector<short> start_position_array;
    
    
    //cin>>game_size;
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
    
    clock_t time = clock();
    vector<TMoveType> answer = GameSolver.CalcTagSolution(start_position_array);
    time = clock() - time;
    
    if(answer[0] == ERROR_GAME)
    {
        cout << "There is no any solution" << endl;
        return 0;
    }
    
    cout<<answer.size()<<endl;
    for(long long i = answer.size()-1;i>= 0;i--)
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
    
//    cout<<"\ntime: "<<time/CLOCKS_PER_SEC<<endl;
    return 0;
}