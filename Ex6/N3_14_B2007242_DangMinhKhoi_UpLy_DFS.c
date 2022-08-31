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
    int top_index;
} Stack;

void makeNullStack(Stack *stack)
{
    stack->top_index = -1;
}
int emptyStack(Stack stack)
{
    return stack.top_index == -1;
}
int fullStack(Stack stack)
{
    return stack.top_index == Maxlength - 1;
}
void push(Node *node, Stack *stack)
{
    if (!fullStack(*stack))
    {
        stack->top_index++;
        stack->Elements[stack->top_index] = node;
    }
    else
        printf("Stack is Full!");
}
Node *top(Stack stack)
{
    if (!emptyStack(stack))
    {
        return stack.Elements[stack.top_index];
    }
    else
        return NULL;
}
void pop(Stack *stack)
{
    if (!emptyStack(*stack))
    {
        stack->top_index--;
    }
    else
        printf("Error! Stack is empty");
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
int findState(State state, Stack stack)
{
    while (!emptyStack(stack))
    {
        if (compareStates(state, top(stack)->state))
            return 1;
        pop(&stack);
    }
    return 0;
}
Node *DFS_Algorithm(State state)
{
    Stack Open, Close;
    makeNullStack(&Open);
    makeNullStack(&Close);
    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;
    push(root, &Open);
    while (!emptyStack(Open))
    {
        Node *node;
        node = top(Open);
        pop(&Open);
        push(node, &Close);
        if (checkGoal(node->state))
            return node;
        int opt;
        for (opt = 1; opt <= 5; opt++)
        {
            State newState;
            makeNullState(&state);
            if (call_operator(node->state, &newState, opt))
            {
                if (findState(newState, Open) || findState(newState, Close))
                    continue;
                Node *newNode = (Node *)malloc(sizeof(Node));
                newNode->state = newState;
                newNode->parent = node;
                newNode->no_function = opt;
                push(newNode, &Open);
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
    Stack stack;
    makeNullStack(&stack);
    while (node->parent != NULL)
    {
        push(node, &stack);
        node = node->parent;
    }
    push(node, &stack);
    int no_action = 0;
    while (!emptyStack(stack))
    {
        printf("\nAction %d: %s\n", no_action, action[top(stack)->no_function]);
        printState(top(stack)->state);
        pop(&stack);
        no_action++;
    }
}
int main()
{
    State state = {up, down, up, down, up, down};
    Node *node = DFS_Algorithm(state);
    print_WaysToGetGoal(node);
    return 0;
}
