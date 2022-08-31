#include <stdio.h>
#include <stdlib.h>

#define goal 0
#define capacity_X 3
#define capacity_Y 3
#define Empty 0
#define Maxlength 100
typedef struct
{
    int monk, demon;
    char bank;
} State;

void makeNullState(State *state)
{
    state->monk = Empty;
    state->demon = Empty;
    state->bank = ' ';
}

int outGame(State state)
{
    if (state.monk < 0 || state.monk > capacity_X || state.demon < 0 || state.demon > capacity_Y)
        return 1;
    if ((state.monk == 0 && state.demon <= capacity_Y) || (state.monk == capacity_X && state.demon <= capacity_Y) || state.monk == state.demon)
        return 0;
    return 1;
}
int checkGoal(State state)
{
    return (state.monk == 0 && state.demon == 0 && state.bank == 'B');
}
int move_1_monk(State cur_state, State *result)
{
    result->demon = cur_state.demon;
    if (cur_state.bank == 'A')
    {
        result->monk = cur_state.monk - 1;
        result->bank = 'B';
    }
    else
    {
        result->monk = cur_state.monk + 1;
        result->bank = 'A';
    }
    if (!outGame(*result))
        return 1;
    return 0;
}
int move_2_monk(State cur_state, State *result)
{
    result->demon = cur_state.demon;
    if (cur_state.bank == 'A')
    {
        result->monk = cur_state.monk - 2;
        result->bank = 'B';
    }
    else
    {
        result->monk = cur_state.monk + 2;
        result->bank = 'A';
    }
    if (!outGame(*result))
        return 1;
    return 0;
}
int move_1_demon(State cur_state, State *result)
{
    result->monk = cur_state.monk;
    if (cur_state.bank == 'A')
    {
        result->demon = cur_state.demon - 1;
        result->bank = 'B';
    }
    else
    {
        result->demon = cur_state.demon + 1;
        result->bank = 'A';
    }
    if (!outGame(*result))
        return 1;
    return 0;
}
int move_2_demon(State cur_state, State *result)
{
    result->monk = cur_state.monk;
    if (cur_state.bank == 'A')
    {
        result->demon = cur_state.demon - 2;
        result->bank = 'B';
    }
    else
    {
        result->demon = cur_state.demon + 2;
        result->bank = 'A';
    }
    if (!outGame(*result))
        return 1;
    return 0;
}
int move_1monk_1demon(State cur_state, State *result)
{
    if (cur_state.bank == 'A')
    {
        result->monk = cur_state.monk - 1;
        result->demon = cur_state.demon - 1;
        result->bank = 'B';
    }
    else
    {
        result->monk = cur_state.monk + 1;
        result->demon = cur_state.demon + 1;
        result->bank = 'A';
    }
    if (!outGame(*result))
        return 1;
    return 0;
}
int call_operator(State cur_state, State *result, int opt)
{
    switch (opt)
    {

    case 1:
        return move_1_monk(cur_state, result);
    case 2:
        return move_1_demon(cur_state, result);
    case 3:
        return move_2_monk(cur_state, result);
    case 4:
        return move_2_demon(cur_state, result);
    case 5:
        return move_1monk_1demon(cur_state, result);
    default:
        printf("Error call operator\n");
        return 0;
    }
}
void printState(State state)
{
    printf("    (Monk: %d)-----(Demon: %d)-----(Bank: %c)\n", state.monk, state.demon, state.bank);
}
const char *action[] = {
    "First State",
    "Move 1 Monk",
    "Move 1 Demon",
    "Move 2 Monk",
    "Move 2 Demon",
    "Move 1 Monk and 1 Demon"};
typedef struct Node
{
    State state;
    struct Node *Parent;
    int no_function;
} Node;
typedef struct
{
    Node *Element[Maxlength];
    int front, rear;
} Queue;
void makeNullQueue(Queue *queue)
{
    queue->front = -1;
    queue->rear = -1;
}
int emptyQueue(Queue queue)
{
    return queue.front == -1;
}
int fullQueue(Queue queue)
{
    return ((queue.rear - queue.front + 1) % Maxlength) == 0;
}
void pushQueue(Node *node, Queue *queue)
{
    if (!fullQueue(*queue))
    {
        if (emptyQueue(*queue))
            queue->front = 0;
        queue->rear = (queue->rear + 1) % Maxlength;
        queue->Element[queue->rear] = node;
    }
    else
        printf("Error! Push");
}
void deQueue(Queue *queue)
{
    if (!emptyQueue(*queue))
    {
        if (queue->front == queue->rear)
            makeNullQueue(queue);
        else
            queue->front = (queue->front + 1) % Maxlength;
    }
}
Node *front(Queue queue)
{
    if (!emptyQueue(queue))
        return queue.Element[queue.front];
    else
        return NULL;
}
int compareStates(State state1, State state2)
{
    return state1.monk == state2.monk && state1.demon == state2.demon && state1.bank == state2.bank;
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
Node *BFS_Algorithm(State state)
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
        if (checkGoal(node->state))
        {
            return node;
        }
        int opt;
        for (opt = 1; opt <= 5; opt++)
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
    Node a[100];
    int i = 0;
    while (node->Parent != NULL)
    {
        a[i++] = *node;
        node = node->Parent;
    }
    a[i] = *node;
    int no_action = 0;
    while (i >= 0)
    {
        printf("Action %d: %s \n", no_action++, action[a[i].no_function]);
        printState(a[i].state);
        i--;
    }
}
int main()
{
    State state = {3, 3, 'A'};
    Node *node = BFS_Algorithm(state);
    print_WaysToGetGoal(node);
    return 0;
}