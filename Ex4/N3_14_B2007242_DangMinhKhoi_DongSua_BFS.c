#include <stdio.h>
#include <stdlib.h>
#define capacity_X 8
#define capacity_Y 5
#define capacity_Z 3
#define Empty 0
#define goal 4
#define Maxlength 100
typedef struct
{
    int x, y, z;
} State;

const char *action[] = {
    "First State",
    "pour Milk X to Y",
    "pour Milk X to Z",
    "pour Milk Y to X",
    "pour Milk Y to Z",
    "pour Milk Z to X",
    "pour Milk Z to Y"};
void makeNullState(State *state)
{
    state->x = capacity_X;
    state->y = Empty;
    state->z = Empty;
}
void printState(State state)
{
    printf("\n     (X:%d)-----(Y:%d)------(Z:%d)", state.x, state.y, state.z);
}
int goalCheck(State state)
{
    return state.x == goal || state.y == goal || state.z == goal;
}
int min(int a, int b)
{
    return a < b ? a : b;
}
int max(int a, int b)
{
    return a > b ? a : b;
}
int pourMilk_XY(State cur_state, State *result)
{
    if (cur_state.x > 0 && cur_state.y < capacity_X)
    {
        result->x = max(cur_state.x - (capacity_Y - cur_state.y), Empty);
        result->y = min(cur_state.x + cur_state.y, capacity_Y);
        result->z = cur_state.z;
        return 1;
    }
    return 0;
}
int pourMilk_XZ(State cur_state, State *result)
{
    if (cur_state.x > 0 && cur_state.z < capacity_Z)
    {
        result->x = max(cur_state.x - (capacity_Z - cur_state.z), Empty);
        result->y = cur_state.y;
        result->z = min(cur_state.z + cur_state.x, capacity_Z);
        return 1;
    }
    return 0;
}
int pourMilk_YX(State cur_state, State *result)
{
    if (cur_state.y > 0 && cur_state.x < capacity_X)
    {
        result->y = max(cur_state.y - (capacity_X - cur_state.x), Empty);
        result->x = min(cur_state.x + cur_state.y, capacity_X);
        result->z = cur_state.z;

        return 1;
    }
    return 0;
}
int pourMilk_YZ(State cur_state, State *result)
{
    if (cur_state.y > 0 && cur_state.z < capacity_Z)
    {
        result->x = cur_state.x;
        result->y = max(cur_state.y - (capacity_Z - cur_state.z), Empty);
        result->z = min(cur_state.z + cur_state.y, capacity_Z);
        return 1;
    }
    return 0;
}
int pourMilk_ZX(State cur_state, State *result)
{
    if (cur_state.z > 0 && cur_state.x < capacity_X)
    {
        result->z = max(cur_state.z - (capacity_X - cur_state.x), Empty);
        result->y = cur_state.y;
        result->x = min(cur_state.x + cur_state.z, capacity_X);
        return 1;
    }
    return 0;
}
int pourMilk_ZY(State cur_state, State *result)
{
    if (cur_state.x > 0 && cur_state.y < capacity_Y)
    {
        result->x = cur_state.x;
        result->z = max(cur_state.z - (capacity_Y - cur_state.y), Empty);
        result->y = min(cur_state.y + cur_state.z, capacity_Y);
        return 1;
    }
    return 0;
}
int call_operator(State cur_state, State *result, int option)
{
    switch (option)
    {
    case 1:
        return pourMilk_XY(cur_state, result);
    case 2:
        return pourMilk_XZ(cur_state, result);
    case 3:
        return pourMilk_YX(cur_state, result);
    case 4:
        return pourMilk_YZ(cur_state, result);
    case 5:
        return pourMilk_ZX(cur_state, result);
    case 6:
        return pourMilk_ZY(cur_state, result);
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
typedef struct{
    Node *Element[Maxlength];
    int front, rear;
}Queue;
void makeNullQueue(Queue *queue){
    queue->front=-1;
    queue->rear=-1;
}
int emptyQueue(Queue queue){
    return queue.front==-1;
}
int fullQueue(Queue queue){
    return ((queue.rear-queue.front+1)%Maxlength)==0;
}
void pushQueue(Node *node,Queue *queue){
    if(!fullQueue(*queue)){
        if(emptyQueue(*queue))
            queue->front=0;
        queue->rear=(queue->rear+1)%Maxlength;
        queue->Element[queue->rear]=node;
    }
    else printf("Error! Push");
}
void deQueue(Queue *queue){
    if(!emptyQueue(*queue)){
        if(queue->front==queue->rear)
            makeNullQueue(queue);
        else queue->front=(queue->front+1)%Maxlength;
    }
}
Node* front(Queue queue){
    if(!emptyQueue(queue))
        return queue.Element[queue.front];
    else return NULL;
}
int compareStates(State state1, State state2)
{
    return (state1.x == state2.x && state1.y == state2.y && state1.z == state2.z);
}
int find_State(State state, Queue OpenQueue)
{
    while (!emptyQueue(OpenQueue))
    {
        Node *node = front(OpenQueue);
        if (compareStates(node->state, state))
            return 1;
        deQueue(&OpenQueue);
    }
    return 0;
}
Node *DFS_Algorithm(State state)
{
    Queue Open;
    Queue Close;
    makeNullQueue(&Open);
    makeNullQueue(&Close);
    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->Parent = NULL;
    root->no_function = 0;
    pushQueue(root, &Open);
    while (!emptyQueue(Open))
    {
        Node *node = front(Open);
        deQueue(&Open);
        pushQueue(node, &Close);
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
                if (find_State(newstate, Close) || find_State(newstate, Open))
                    continue;
                Node *newNode = (Node *)malloc(sizeof(Node));
                newNode->state = newstate;
                newNode->Parent = node;
                newNode->no_function = opt;
                pushQueue(newNode, &Open);
            }
        }
    }
    return NULL;
}

void print_WaysToGetGoal(Node *node)
{
    int i = 0;
    Node way[100];
    while (node->Parent != NULL)
    {
        way[i++] = *node;
        node = node->Parent;
    }
    way[i] = *node;
    int j, no_action = 0;
    for (j = i; j >= 0; j--)
    {
        printf("\nAction %d: %s", no_action, action[way[j].no_function]);
        printState(way[j].state);
        no_action++;
    }
}

int main()
{
    State cur_state;
    makeNullState(&cur_state);
    Node *p = DFS_Algorithm(cur_state);
    print_WaysToGetGoal(p);
    return 0;
}