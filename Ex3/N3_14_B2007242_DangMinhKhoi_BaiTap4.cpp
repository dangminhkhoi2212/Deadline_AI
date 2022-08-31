#include <stdio.h>
#include <stdlib.h>
#define capacity_X 9
#define capacity_Y 4
#define Empty 0
#define goal 6
#define Maxlength 100
#include <stack>
using namespace std;
typedef struct
{
    int x, y;
} State;

const char *action[] = {
    "First State",
    "pour Water Full X",
    "pour Water Full Y",
    "pour Water Empty X",
    "pour Water Empty Y",
    "pour Water X to Y",
    "pour Water Y to X"};
void makeNullState(State *state)
{
    state->x = 0;
    state->y = 0;
}
void printState(State state)
{
    printf("\n     X:%d-----Y:%d", state.x, state.y);
}
int goalCheck(State state)
{
    return state.x == goal || state.y == goal;
}
int pourWaterFullX(State cur_state, State *result)
{
    if (cur_state.x < capacity_X)
    {
        result->x = capacity_X;
        result->y = cur_state.y;
        return 1;
    }
    return 0;
}
int pourWaterFullY(State cur_state, State *result)
{
    if (cur_state.y < capacity_Y)
    {
        result->x = cur_state.x;
        result->y = capacity_Y;
        return 1;
    }
    return 0;
}
int pourWaterEmptyX(State cur_state, State *result)
{
    if (cur_state.x > 0)
    {
        result->x = Empty;
        result->y = cur_state.y;
        return 1;
    }
    return 0;
}
int pourWaterEmptyY(State cur_state, State *result)
{
    if (cur_state.y > 0)
    {
        result->x = cur_state.x;
        result->y = Empty;
        return 1;
    }
    return 0;
}

int max(int a, int b)
{
    return a > b ? a : b;
}
int min(int a, int b)
{
    return a < b ? a : b;
}

int pourWaterXY(State cur_state, State *result)
{
    if (cur_state.x > 0 && cur_state.y < capacity_Y)
    {
        result->x = max(cur_state.x - (capacity_Y - cur_state.y), Empty);
        result->y = min(capacity_Y, cur_state.y + cur_state.x);
        return 1;
    }
    return 0;
}
int pourWaterYX(State cur_state, State *result)
{
    if (cur_state.y > 0 && cur_state.x < capacity_X)
    {
        result->x = min(capacity_X, cur_state.x + cur_state.y);
        result->y = max(cur_state.y - (capacity_X - cur_state.x), Empty);
        return 1;
    }
    return 0;
}
int call_operator(State cur_state, State *result, int option)
{
    switch (option)
    {
    case 1:
        return pourWaterFullX(cur_state, result);
    case 2:
        return pourWaterFullY(cur_state, result);
    case 3:
        return pourWaterEmptyX(cur_state, result);
    case 4:
        return pourWaterEmptyY(cur_state, result);
    case 5:
        return pourWaterXY(cur_state, result);
    case 6:
        return pourWaterYX(cur_state, result);
    default:
        printf("Error calls operators");
        return 0;
    }
}

typedef struct Node
{
    State state;
    struct Node *Parent;
    int no_function;
} Node;

int compareStates(State state1, State state2)
{
    return (state1.x == state2.x && state1.y == state2.y);
}
int find_State(State state, stack<Node *> OpenQueue)
{
    while (!OpenQueue.empty())
    {
        Node *node = OpenQueue.top();
        if (compareStates(node->state, state))
            return 1;
        OpenQueue.pop();
    }
    return 0;
}
Node *DFS_Algorithm(State state)
{
    stack<Node *> Open_BFS;
    stack<Node *> Close_BFS;
    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->Parent = NULL;
    root->no_function = 0;
    Open_BFS.push(root);
    while (!Open_BFS.empty())
    {
        Node *node = Open_BFS.top();
        Open_BFS.pop();
        Close_BFS.push(node);
        if (goalCheck(node->state))
        {
            return node;
        }
        int opt;
        for (opt = 1; opt <= 6; opt++)
        {
            State newstate;
            makeNullState(&newstate);
            if (call_operator(node->state, &newstate, opt))
            {
                if (find_State(newstate, Close_BFS) || find_State(newstate, Open_BFS))
                    continue;
                Node *newNode = (Node *)malloc(sizeof(Node));
                newNode->state = newstate;
                newNode->Parent = node;
                newNode->no_function = opt;
                Open_BFS.push(newNode);
            }
        }
    }
    return NULL;
}

void print_WaysToGetGoal(Node *node)
{
   stack<Node*> stack;
    while (node->Parent != NULL)
    {
        stack.push(node);
        node = node->Parent;
    }
    int no_action = 0;
   while(!stack.empty())
    {
        printf("\nAction %d: %s", no_action, action[stack.top()->no_function]);
        printState(stack.top()->state);
        stack.pop();
        no_action++;
    }
}

int main()
{
    State cur_state = {0, 0};
    Node *p = DFS_Algorithm(cur_state);
    print_WaysToGetGoal(p);
    return 0;
}