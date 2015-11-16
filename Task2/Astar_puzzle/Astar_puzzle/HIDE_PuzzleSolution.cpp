
#include <vector>
#include <set>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <unistd.h>
using std::vector;
using std::set;

enum TMoveType
{
    MOVE_DOWN,
    MOVE_UP,
    MOVE_RIGHT,
    MOVE_LEFT,
    ERROR_GAME
};

static const short IS_SPACE = 0;

static int size_of_game;
void set_size_of_game(const int game_size)
{
    size_of_game = game_size;
}

class PositionParams;
class GamePosition;

class GamePosition
{
public:
    GamePosition(const vector<short> & game_position);
    GamePosition(const GamePosition & copied_game);
    ~GamePosition(){}
    
    vector<short> position;
    short space_position;
    
    const GamePosition  * ancestor;
    const PositionParams * params;
    

};

bool operator == (const GamePosition & first, const GamePosition & second)
{
    for (int i = 0; i<first.position.size(); ++i)
    {
        if(first.position[i] < second.position[i]) return false;
        if(first.position[i] > second.position[i]) return false;
    }
    return true;
}

bool operator < (const GamePosition & first, const GamePosition & second)
{
    for (int i = 0; i<first.position.size(); ++i)
    {
        if(first.position[i] < second.position[i]) return true;
        if(first.position[i] > second.position[i]) return false;
    }
    return false;
}

// GamePositon class functions
GamePosition::GamePosition(const GamePosition & copied_game)
{
    space_position = copied_game.space_position;
    position = copied_game.position;
    ancestor = copied_game.ancestor;
    params = copied_game.params;
}
GamePosition::GamePosition(const vector<short> &  start_position)
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
    params = nullptr;
}

//PositionParams methods
class PositionParams
{
public:
    PositionParams(const PositionParams &  copied_params);
    PositionParams(GamePosition * game_to_connect);
    ~PositionParams(){};
    int potential;
    int distance;
    bool is_open;
    const GamePosition * connect_game;
    
    void SetPotential();
};

bool operator < (const PositionParams & first, const PositionParams & second)
{
    if(second.is_open < first.is_open) return true; // open nodes always < than closed
    if(second.is_open > first.is_open) return false;
    if(first.distance + first.potential < second.distance + second.potential) return true;
    if(first.distance + first.potential > second.distance + second.potential) return false;
    if(first.connect_game < second.connect_game) return true;
    return false;
}
bool operator == (const PositionParams & first, const PositionParams & second)
{
    if(first.connect_game != second.connect_game) return false;
    if(second.is_open != first.is_open) return false;
    if(first.distance != second.distance) return false;
    return true;
}
void PositionParams::SetPotential(void)
{
    
    potential = 0;
    for(int i = 0;i<connect_game->position.size();++i)
    {
        potential += abs(i % size_of_game - connect_game->position[i]%size_of_game) + abs(i/size_of_game - connect_game->position[i]/size_of_game);
    }
    //now potenial cannot be 1
    potential /= 2;
}

PositionParams::PositionParams(const PositionParams & copied_params)
{
    is_open = copied_params.is_open;
    potential = copied_params.potential;
    distance = copied_params.distance;
    connect_game = copied_params.connect_game;
}
PositionParams::PositionParams(GamePosition * game_to_connect)
{
    is_open = true;
    distance = 0;
    connect_game  = game_to_connect;
    SetPotential();
    game_to_connect->params = this;
}

// solution functions

int GetInversions(const vector<short> & array)
{
    int inversions = 0;
    
    for(int i = 0;i<array.size();++i)
    {
        if(array[i] == IS_SPACE) continue;
        for (int j=i+1; j<array.size(); ++j)
        {
            if(array[j] == IS_SPACE) continue;
            if(array[j]<array[i]) ++inversions;
        }
    }
    return inversions;
}

bool IsSolutionExist(const GamePosition & start_position)
{
    return true;
    /*
    if(size_of_game != 4) return true; // i can't check
    
    int number_of_space_str;
    int inversions = GetInversions(start_position.position);
    
    number_of_space_str = start_position.space_position / size_of_game + 1;
    
    return !(inversions + number_of_space_str)%2;*/
}


bool DijkstraAlgorithm(set<GamePosition> & game_state, set<PositionParams> & state_params)
{
    while(1)
    {
        set<PositionParams>::const_iterator cur_params_iter = state_params.begin(); // get min
        
        assert(cur_params_iter->is_open); //something wrong (tragic case)
        if(cur_params_iter->potential == 0) return true; //find finish
        
        PositionParams current_params(*cur_params_iter); //this node was viewed
        current_params.is_open = false;
        
        set<GamePosition>::const_iterator cur_position_iter = game_state.find(*current_params.connect_game);
        GamePosition current_position(*cur_position_iter);
        current_position.params = &current_params;
        current_params.connect_game = &current_position;
        
        state_params.erase(cur_params_iter);
        state_params.insert(current_params);
        game_state.erase(cur_position_iter);
        game_state.insert(current_position);
        
        for(int check = 0;check < 4;++check)
        {
            GamePosition next_position(current_position.position);
            bool is_new = false;
            if(check == 0 && current_position.space_position >= size_of_game) // move space up
            {
                is_new = true;
                std::swap(next_position.position[current_position.space_position],next_position.position[current_position.space_position - size_of_game]);
                next_position.space_position = current_position.space_position - size_of_game;
            }
            if(check == 1 && current_position.space_position <= size_of_game * (size_of_game-1) - 1) // move down
            {
                is_new = true;
                std::swap(next_position.position[current_position.space_position],next_position.position[current_position.space_position + size_of_game]);
                next_position.space_position = current_position.space_position + size_of_game;
            }
            if(check == 2 && current_position.space_position % size_of_game != 0) // move left
            {
                is_new = true;
                std::swap(next_position.position[current_position.space_position] , next_position.position[current_position.space_position - 1]);
                next_position.space_position = current_position.space_position - 1;
            }
            if(check == 3 && (current_position.space_position + 1) % size_of_game != 0) // move right
            {
                is_new = true;
                std::swap(next_position.position[current_position.space_position] , next_position.position[current_position.space_position + 1]);
                next_position.space_position = current_position.space_position + 1;
            }
            
           if(is_new == true)
            {
                set<GamePosition>::const_iterator position_iterator = game_state.find(next_position);
                
                PositionParams next_params(&next_position);
                next_params.distance = current_params.distance + 1;
                next_position.ancestor = &current_position;
                
                if(position_iterator == game_state.end())
                {
                    game_state.insert(next_position);
                    state_params.insert(next_params);
                }
                else if(position_iterator->params->is_open)
                {
                    set<PositionParams>::const_iterator params_iterator = state_params.find(*current_position.params);
                    assert(params_iterator != state_params.end());
                    
                    if(next_params.distance < params_iterator->distance)
                    {
                        game_state.erase(position_iterator);
                        state_params.erase(params_iterator);
                        
                        
                        game_state.insert(next_position);
                        state_params.insert(next_params);
                    }
                }
            }
        }
    }
}

vector<TMoveType> CalcTagSolution(const vector<short> & start_position,const int game_size) // general function
{
    assert(game_size>0);
    
    set_size_of_game(game_size);
    
    vector<TMoveType> answer;
    GamePosition & start_game_state = *(new GamePosition(start_position));
    PositionParams & start_state_params = *(new PositionParams(&start_game_state));
    
    set<GamePosition> game_state; // for check existing
    set<PositionParams> state_params; // to get min in Dijkstra algorithm
    game_state.insert(start_game_state);
    state_params.insert(start_state_params);
    
    if(!IsSolutionExist(start_game_state))
    {
        answer.push_back(ERROR_GAME);
    }
    else
    {
        DijkstraAlgorithm(game_state,state_params);
        
        const GamePosition * cur_pos;
        cur_pos = &(*game_state.begin());
        
        short last_space_position = cur_pos->space_position;
        short delta_space_position;
        while(cur_pos->ancestor != nullptr)
        {
            cur_pos = cur_pos->ancestor;
            delta_space_position = last_space_position - cur_pos->space_position;
            
            if(delta_space_position == 1) answer.push_back(MOVE_RIGHT);
            if(delta_space_position == -1) answer.push_back(MOVE_LEFT);
            if(delta_space_position == size_of_game) answer.push_back(MOVE_DOWN);
            if(delta_space_position == - size_of_game) answer.push_back(MOVE_UP);
            
            last_space_position = cur_pos->space_position;
        }
    }
    game_state.clear();
    state_params.clear();
    
    return  answer;
}

int main()
{
    int game_size;
    vector<short> start_position_array;

    std::cin>>game_size;
    if(game_size != 3)
    {
        std::cout<<0<<std::endl;
        return 0;
    }
    
    start_position_array.resize(game_size*game_size);
    for(int i = 0;i<start_position_array.size();++i)
    {
        std::cin>>start_position_array[i];
    }
    vector<TMoveType> answer = CalcTagSolution(start_position_array, game_size);
    
    std::cout<<answer.size()<<std::endl;
    for(long long i = answer.size()-1;i>= 0;i--)
    {
        /*interpretation answer*/
        switch (answer[i])
        {
            case MOVE_UP:
                std::cout<<"UP"<<std::endl;
                break;
            case MOVE_DOWN:
                std::cout<<"DOWN"<<std::endl;
                break;
            case MOVE_LEFT:
                std::cout<<"LEFT"<<std::endl;
                break;
            case MOVE_RIGHT:
                std::cout<<"RIGHT"<<std::endl;
                break;
                
            default:
                break;
        }
    }
    
    return 0;
}


