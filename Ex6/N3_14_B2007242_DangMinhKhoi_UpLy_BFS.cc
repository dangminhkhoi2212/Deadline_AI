#include <iostream>
#include <stdlib.h>
#include <queue>
using namespace std;
#define Max_glass 6
#define up 0
#define down 1
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
bool compareState(State state1, State state2)
{
    for (int i = 0; i < Max_glass; i++)
    {
        if (state1.Glass[i] != state2.Glass[i])
            return false;
    }
    return true;
}
bool findState(State state, queue<Node *> queue)
{
    while (!queue.empty())
    {
        if (compareState(state, queue.front()->state))
            return true;
        queue.pop();
    }
    return false;
}
void printState(State state)
{
    cout << "\t";
    for (int i = 0; i < Max_glass; i++)
    {
        printf("%d ", state.Glass[i]);
    }
}

Node *BFS_Algorithm(State state)
{
    queue<Node *> Open, Close;
    Node *root;
    root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;
    root->no_function;
    Open.push(root);
    int k = 1;
    while (!Open.empty())
    {
        Node *node;
        node = Open.front();
        Open.pop();
        Close.push(node);
        if (checkGoal(node->state))
            return node;

        for (int opt = 1; opt <= 4; opt++)
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

void print_WaysToGetGoal(Node *node)
{
    Node *a[100];
    int i = 0;
    while (node->parent != NULL)
    {
        a[i++] = node;
        node = node->parent;
    }
    a[i] = node;
    int no_action = 0;
    for (; i >= 0; i--)
    {
        printf("Action %d: %s\n", no_action, action[a[i]->no_function]);
        printState(a[i]->state);
        printf("\n");
        no_action++;
    }
}
int main()
{
    State state = {up, down, up, down, up, down};
    Node *node = BFS_Algorithm(state);
    print_WaysToGetGoal(node);
    return 0;
}
