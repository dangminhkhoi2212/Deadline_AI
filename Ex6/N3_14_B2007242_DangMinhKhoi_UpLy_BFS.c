#include <stdio.h>
#include <stdlib.h>
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
int checkGoal(State state)
{
    for (int i = 0; i < Max_glass; i++)
    {
        if (state.Glass[i] != 0)
            return 0;
    }
    return 1;
}
int isChange(State state, int x, int y, int z)
{

    return (state.Glass[x] != up || state.Glass[y] != up || state.Glass[z] != up);
}
int changeState(int status)
{
    return status == up ? down : up;
}
int changeGlass(State cur_state, State *result, int x, int y, int z)
{
    if (isChange(cur_state, x, y, z))
    {
        *result = cur_state;
        result->Glass[x] = changeState(cur_state.Glass[x]);
        result->Glass[y] = changeState(cur_state.Glass[y]);
        result->Glass[z] = changeState(cur_state.Glass[z]);
        return 1;
    }
    return 0;
}
int call_operator(State cur_state, State *result, int opt)
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
        return 0;
    }
}

typedef struct Node
{
    State state;
    struct Node *parent;
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
int empty(Queue queue)
{
    return queue.front == -1;
}
int full_Queue(Queue queue)
{
    return ((queue.rear - queue.front + 1) % Maxlength) == 0;
}
Node *get_Front(Queue queue)
{
    if (empty(queue))
    {
        printf("Queue is empty_p");
        return NULL;
    }
    else
        return queue.Elements[queue.front];
}
void del_Queue(Queue *queue)
{
    if (!empty(*queue))
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
        if (empty(*queue))
        {
            queue->front = 0;
        }
        queue->rear = (queue->rear + 1) % Maxlength;
        queue->Elements[queue->rear] = x;
    }
    else
        printf("Error, Push");
}
int compareState(State state1, State state2)
{
    for (int i = 0; i < Max_glass; i++)
    {
        if (state1.Glass[i] != state2.Glass[i])
            return 0;
    }
    return 1;
}
int findState(State state, Queue queue)
{
    while (!empty(queue))
    {
        if (compareState(state, get_Front(queue)->state))
            return 1;
        del_Queue(&queue);
    }
    return 0;
}
void printState(State state)
{
    printf("\t");
    for (int i = 0; i < Max_glass; i++)
    {
        printf("%d ", state.Glass[i]);
    }
}

int compareStates(State state1, State state2)
{
    for (int i = 0; i < Max_glass; i++)
    {
        if (state1.Glass[i] != state2.Glass[i])
            return 0;
    }
    return 1;
}
int find_State(State state, Queue OpenQueue)
{
    while (!empty(OpenQueue))
    {
        Node *node = get_Front(OpenQueue);
        if (compareStates(node->state, state))
            return 1;
        del_Queue(&OpenQueue);
    }
    return 0;
}
Node *BFS_Algorithm(State state)
{
    Queue Open_BFS;
    Queue Close_BFS;
    makeNull_Queue(&Open_BFS);
    makeNull_Queue(&Close_BFS);
    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;
    push_Queue(root, &Open_BFS);
    while (!empty(Open_BFS))
    {
        Node *node = get_Front(Open_BFS);
        del_Queue(&Open_BFS);
        push_Queue(node, &Close_BFS);
        if (checkGoal(node->state))
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
                newNode->parent = node;
                newNode->no_function = opt;
                push_Queue(newNode, &Open_BFS);
            }
        }
    }
    return NULL;
}
void print_WaysToGetGoal(Node *node)
{
    Node *way[100];
    int i = 0;
    while (node->parent != NULL)
    {
        way[i++] = node;
        node = node->parent;
    }
    way[i] = node;
    int j, no_action = 0;
    for (j = i; j >= 0; j--)
    {
        printf("\nAction %d: %s\n", no_action, action[way[j]->no_function]);
        printState(way[j]->state);
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
