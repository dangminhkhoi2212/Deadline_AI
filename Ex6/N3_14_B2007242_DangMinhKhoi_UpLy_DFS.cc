#include <stdio.h>
#include <stdlib.h>
#include <stack>
using namespace std;
#define Max_glass 6
#define up 0
#define down 1
#define Maxlength 100
typedef struct
{
    int Glass[Max_glass];
} State;
const char *action[] = {
    "First state",
    "Change glass 1 2 3",
    "Change glass 2 3 4",
    "Change glass 3 4 5",
    "Change glass 4 5 6"};
void makeNullState(State *state)
{
    for (int i = 0; i < Max_glass; i++)
    {
        state->Glass[i] = down;
    }
}
bool checkGoal(State state)
{
    for (int i = 0; i < Max_glass; i++)
    {
        if (state.Glass[i] != 0)
            return false;
    }
    return true;
}
bool isChange(State state, int x, int y, int z)
{

    return (state.Glass[x] != up || state.Glass[y] != up || state.Glass[z] != up);
}
int changeState(int status)
{
    return status == up ? down : up;
}
bool changeGlass(State cur_state, State *result, int x, int y, int z)
{
    if (isChange(cur_state, x, y, z))
    {
        *result = cur_state;
        result->Glass[x] = changeState(cur_state.Glass[x]);
        result->Glass[y] = changeState(cur_state.Glass[y]);
        result->Glass[z] = changeState(cur_state.Glass[z]);
        return true;
    }
    return false;
}
bool call_operator(State cur_state, State *result, int opt)
{
    switch (opt)
    {
    case 1:
        return changeGlass(cur_state, result, 0, 1, 2);
    case 2:
        return changeGlass(cur_state, result, 1, 2, 3);
    case 3:
        return changeGlass(cur_state, result, 2, 3, 4);
    case 4:
        return changeGlass(cur_state, result, 3, 4, 5);
    default:
        return false;
    }
}

typedef struct Node
{
    State state;
    struct Node *parent;
    int no_function;
} Node;

bool compareStates(State state1, State state2)
{
    for (int i = 0; i < Max_glass; i++)
    {
        if (state1.Glass[i] != state2.Glass[i])
            return false;
    }
    return true;
}
bool findState(State state, stack<Node*> stack)
{
    while (!stack.empty())
    {
        if (compareStates(state, stack.top()->state))
            return true;
        stack.pop();
    }
    return false;
}
Node *DFS_Algorithm(State state)
{
    stack<Node *> Open, Close;
    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;
    Open.push(root);
    while (!Open.empty())
    {
        Node *node = Open.top();
        Open.pop();
        Close.push(node);
        if (checkGoal(node->state))
            return node;
        int opt;
        for (opt = 1; opt <= 5; opt++)
        {
            State newState;
            makeNullState(&newState);
            if (call_operator(node->state, &newState, opt))
            {
                if (findState(newState, Open) || findState(newState, Close))
                    continue;
                Node *newNode = (Node *)malloc(sizeof(Node));
                newNode->state = newState;
                newNode->parent = node;
                newNode->no_function = opt;
                Open.push(newNode);
            }
        }
    }
    return NULL;
}
void printState(State state)
{
    printf("\t");
    for (int i = 0; i < Max_glass; i++)
    {
        printf("%d ", state.Glass[i]);
    }
}
void print_WaysToGetGoal(Node *node)
{
    stack<Node *> stack;
    while (node->parent != NULL)
    {
        stack.push(node);
        node = node->parent;
    }
    stack.push(node);
    int no_action = 0;
    while (!stack.empty())
    {
        printf("\nAction %d: %s \n", no_action++, action[stack.top()->no_function]);
        printState(stack.top()->state);
        stack.pop();
    }
}


int main()
{
    State state = {up, down, up, down, up, down};
    Node *node = DFS_Algorithm(state);
    print_WaysToGetGoal(node);
    return 0;
}
