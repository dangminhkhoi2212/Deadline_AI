#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;

#define Rows 3
#define Cols 3
#define Empty 0
#define Max_operator 4
#define Maxlength 500
const char *action[] = {
    "First State",
    "Move cell EMPTY to UP",
    "Move cell EMPTY to DOWN",
    "Move cell EMPTY to LEFT",
    "Move cell EMPTY to RIGHT"
};
typedef struct
{
    int eightPuzzle[Rows][Cols];
    int emptyRow;
    int emptyCol;
} State;

void printState(State state)
{
    int row, col;
    printf("\n-------------\n");
    for (row = 0; row < Rows; row++)
    {
        for (col = 0; col < Cols; col++)
            printf("|%2d ", state.eightPuzzle[row][col]);
        printf("|\n");
    }
    printf("-------------\n");
}

bool upOperator(State state, State *result)
{
    *result = state;
    int emptyRowCurrent = state.emptyRow, emptyColCurrent = state.emptyCol;
    if (state.emptyRow > 0)
    {
        result->emptyRow = emptyRowCurrent - 1;
        result->emptyCol = emptyColCurrent;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = state.eightPuzzle[emptyRowCurrent - 1][emptyColCurrent];
        result->eightPuzzle[emptyRowCurrent - 1][emptyColCurrent] = Empty;
        return true;
    }
    return false;
}
bool downOperator(State state, State *result)
{
    *result = state;
    int emptyRowCurrent = state.emptyRow, emptyColCurrent = state.emptyCol;
    if (emptyRowCurrent < Rows - 1)
    {
        result->emptyRow = emptyRowCurrent + 1;
        result->emptyCol = emptyColCurrent;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = state.eightPuzzle[emptyRowCurrent + 1][emptyColCurrent];
        result->eightPuzzle[emptyRowCurrent + 1][emptyColCurrent] = Empty;
        return true;
    }
    return false;
}
bool leftOperator(State state, State *result)
{
    *result = state;
    int emptyRowCurrent = state.emptyRow, emptyColCurrent = state.emptyCol;
    if (emptyColCurrent > 0)
    {
        result->emptyRow = emptyRowCurrent;
        result->emptyCol = emptyColCurrent - 1;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = state.eightPuzzle[emptyRowCurrent][emptyColCurrent - 1];
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent - 1] = Empty;
        return true;
    }
    return false;
}
bool rightOperator(State state, State *result)
{
    *result = state;
    int emptyRowCurrent = state.emptyRow, emptyColCurrent = state.emptyCol;
    if (emptyColCurrent < Cols - 1)
    {
        result->emptyRow = emptyRowCurrent;
        result->emptyCol = emptyColCurrent + 1;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = state.eightPuzzle[emptyRowCurrent][emptyColCurrent + 1];
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent + 1] = Empty;
        return true;
    }
    return false;
}
bool callOperators(State state, State *result, int opt)
{
    switch (opt)
    {
    case 1:
        return upOperator(state, result);
    case 2:
        return downOperator(state, result);
    case 3:
        return leftOperator(state, result);
    case 4:
        return rightOperator(state, result);
    default:
        return false;
    }
}
int heuristic1(State state, State goal)
{
    int row, col, count = 0;
    for (row = 0; row < Rows; row++)
    {
        for (col = 0; col < Cols; col++)
            if (state.eightPuzzle[row][col] != goal.eightPuzzle[row][col])
                count++;
    }
    return count;
}
int heuristic2(State state, State goal)
{
    int row, col, row_g, col_g, count = 0;
    for (row = 0; row < Rows; row++)
        for (col = 0; col < Cols; col++)
            if (state.eightPuzzle[row][col] != Empty)
            {
                for (row_g = 0; row_g < Rows; row_g++)
                    for (col_g=0; col_g < Cols; col_g++)
                        if (state.eightPuzzle[row][col] == goal.eightPuzzle[row_g][col_g])
                        {
                            count += abs(row - row_g) + abs(col - col_g);
                            row_g = Rows;
                            col_g = Cols;
                        }
            }
    return count;
}
typedef struct Node{
    State state;
    struct Node* parent;
    int heuristic;
    int no_function;

}Node;
bool compareStates(State state1, State state2){
    if(state1.emptyRow!=state2.emptyRow ||state1.emptyCol!=state2.emptyCol)
        return false;
    int row, col;
    for(row=0; row<Rows; row++)
        for(col=0; col<Cols;col++)
            if(state1.eightPuzzle[row][col]!=state2.eightPuzzle[row][col])
                return false;
    return true;
}
bool compareHeuristic(Node *a, Node *b){
    return a->heuristic>b->heuristic;
}
Node * findState (State state, vector<Node*> v, vector<Node*>::iterator *position){
    vector<Node*>::iterator it=v.begin();
    if(v.size()==0)
        return NULL;
    while(it!=v.end()){
        if(compareStates((*it)->state, state))
        {
            *position =it;
            return *it;
        }
        it=v.erase(it);
    }
    return NULL;
}
bool goalCheck(State state, State goal){
    return compareStates(state, goal);
}
Node *bestFirstSearch(State state, State goal){
    vector<Node*> Open(Maxlength),Close(Maxlength);
    Open.clear();
    Close.clear();
    Node* root=(Node*)malloc(sizeof(Node));
    root->state=state;
    root->parent=NULL;
    root->no_function=0;
    root->heuristic=heuristic2(state, goal);
    Open.push_back(root);
    while(!Open.empty()){
        Node* node=Open.back();
        Open.pop_back();
        Close.push_back(node);
        if(goalCheck(node->state, goal))
            return node;
        for(int opt=1; opt<=Max_operator; opt++){
            State newState;
            newState=node->state;
            if(callOperators(node->state, &newState, opt)){
                Node *newNode=(Node*)malloc(sizeof(Node));
                newNode->state=newState;
                newNode->parent=node;
                newNode->heuristic=heuristic2(newState, goal);
                newNode->no_function=opt;
                vector<Node*>::iterator pos_Open,pos_Close;
                Node *nodeFoundOpen = findState(newNode->state, Open, &pos_Open);
                Node *nodeFoundClose = findState(newNode->state, Close, &pos_Close);
                if(nodeFoundOpen==NULL && nodeFoundClose==NULL)
                    Open.push_back(newNode);
                else if(nodeFoundOpen!=NULL && nodeFoundOpen->heuristic>newNode->heuristic){
                    Open.erase(pos_Open);
                    Open.push_back(newNode);
                }
                else if(nodeFoundClose!=NULL && nodeFoundClose->heuristic> newNode->heuristic){
                    Close.erase(pos_Close);
                    Open.push_back(newNode);
                }
            }
        }
        sort(Open.begin(), Open.end(), compareHeuristic);
    }
    return NULL;
}
void printWaysToGetGoal(Node* node){
    vector<Node*> vectorPrint;
    while(node->parent!=NULL){
        vectorPrint.push_back(node);
        node=node->parent;
    }
    vectorPrint.push_back(node);
    int no_action=0, i;
    for(i=vectorPrint.size()-1; i>=0; i--){
        printf("\nAction %d: %s", no_action, action[vectorPrint[i]->no_function]);
        printState(vectorPrint[i]->state);
        no_action++;
    }
}
int main()
{
    State state;
    state.emptyRow = 1;
    state.emptyCol = 1;
    state.eightPuzzle[0][0] = 3;
    state.eightPuzzle[0][1] = 4;
    state.eightPuzzle[0][2] = 5;
    state.eightPuzzle[1][0] = 1;
    state.eightPuzzle[1][1] = 0;
    state.eightPuzzle[1][2] = 2;
    state.eightPuzzle[2][0] = 6;
    state.eightPuzzle[2][1] = 7;
    state.eightPuzzle[2][2] = 8;
    State goal;
    goal.emptyRow = 0;
    goal.emptyCol = 0;
    goal.eightPuzzle[0][0] = 0;
    goal.eightPuzzle[0][1] = 1;
    goal.eightPuzzle[0][2] = 2;
    goal.eightPuzzle[1][0] = 3;
    goal.eightPuzzle[1][1] = 4;
    goal.eightPuzzle[1][2] = 5;
    goal.eightPuzzle[2][0] = 6;
    goal.eightPuzzle[2][1] = 7;
    goal.eightPuzzle[2][2] = 8;
    Node *p = bestFirstSearch(state, goal);
    printWaysToGetGoal(p);
    return 0;
}
