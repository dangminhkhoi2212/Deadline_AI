#include <stdio.h>
#include <stdlib.h>
#define capacity_X 9
#define capacity_Y 4
#define empty_p 0
#define goal 6
#define Maxlength 100

typedef struct
{
    int x, y;
} State;

const char *action[] = {
    "First State",
    "pour Water Full X",
    "pour Water Full Y",
    "pour Water empty_p X",
    "pour Water empty_p Y",
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
        result->x = empty_p;
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
        result->y = empty_p;
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
        result->x = max(cur_state.x - (capacity_Y - cur_state.y), empty_p);
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
        result->y = max(cur_state.y - (capacity_X - cur_state.x), empty_p);
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

typedef struct
{
    Node *Elements[Maxlength];
    int front, rear;
} Queue;
void makeNull_Queue(Queue *queue)
{
    queue->front = -1;
    queue->rear = -1;
}
int empty_Queue(Queue queue)
{
    return queue.front == -1;
}
int full_Queue(Queue queue)
{
    return ((queue.rear - queue.front + 1) % Maxlength) == 0;
}
Node *get_Front(Queue queue)
{
    if (empty_Queue(queue))
    {
        printf("Queue is empty_p");
        return NULL;
    }
    else
        return queue.Elements[queue.front];
}
void del_Queue(Queue *queue)
{
    if (!empty_Queue(*queue))
    {
        if (queue->front == queue->rear)
        {
            makeNull_Queue(queue);
        }
        else
            queue->front = (queue->front + 1) % Maxlength;
    }
    else
        printf("Error, ,Delete");
}
void push_Queue(Node *x, Queue *queue)
{
    if (!full_Queue(*queue))
    {
        if (empty_Queue(*queue))
        {
            queue->front = 0;
        }
        queue->rear = (queue->rear + 1) % Maxlength;
        queue->Elements[queue->rear] = x;
    }
    else
        printf("Error, Push");
}
int compareStates(State state1, State state2)
{
    return (state1.x == state2.x && state1.y == state2.y);
}
int find_State(State state, Queue OpenQueue)
{
    while (!empty_Queue(OpenQueue))
    {
        Node *node = get_Front(OpenQueue);
        if (compareStates(node->state, state))
            return 1;
        del_Queue(&OpenQueue);
    }
    return 0;
}
Node *DFS_Algorithm(State state)
{
    Queue Open_BFS;
    Queue Close_BFS;
    makeNull_Queue(&Open_BFS);
    makeNull_Queue(&Close_BFS);
    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->Parent = NULL;
    root->no_function = 0;
    push_Queue(root, &Open_BFS);
    while (!empty_Queue(Open_BFS))
    {
        Node *node = get_Front(Open_BFS);
        del_Queue(&Open_BFS);
        push_Queue(node, &Close_BFS);
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
                push_Queue(newNode, &Open_BFS);
            }
        }
    }
    return NULL;
}
void print_WaysToGetGoal(Node *node)
{
    Node way[100];
    int i = 0;
    way[0] = *node;
    while (node->Parent != NULL)
    {
        i++;
        way[i] = *node;
        node = node->Parent;
    }
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
    State cur_state = {0, 0};
    Node *p = DFS_Algorithm(cur_state);
    print_WaysToGetGoal(p);
    return 0;
}