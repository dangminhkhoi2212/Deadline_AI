#include <stdio.h>
#include <stdlib.h>
#define capacity_X 10
#define capacity_Y 5
#define capacity_Z 6
#define goal 8
#define Empty 0
#define Maxlength 100

const char *action[] = {
    "First state",
    "Pour Milk X to Y",
    "Pour Milk X to Z",
    "Pour Milk Y to X",
    "Pour Milk Y to Z",
    "Pour Milk Z to X",
    "Pour Milk Z to Y"
};
typedef struct{
    int x, y, z;
}State;

void makeNullState(State *state){
    state->x=Empty;
    state->y=Empty;
    state->z=Empty;
}

int min(int a, int b){
    return a<b?a:b;
}
int max(int a, int b){
    return a>b?a:b;
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

int call_operator(State cur_state, State *result, int opt){
    switch(opt){
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
                printf("Error operator!");
                return 0;
        }
}
void printState(State state){
    printf("\n    (X:%d)-----(Y:%d)-----(Z:%d)", state.x, state.y, state.z);
}
int checkGoal(State state){
    return state.x==goal || state.y==goal || state.z == goal;
}
typedef struct Node{
    State state;
    struct Node *Parent;
    int no_function;
}Node;
typedef struct{
    Node *Elements[Maxlength];
    int top_index;
}Stack;

void makeNullStack(Stack *stack){
    stack->top_index=-1;
}
int emptyStack(Stack stack){
    return stack.top_index==-1;
}
int fullStack(Stack stack){
    return stack.top_index==Maxlength-1;
}
void push(Node *node, Stack *stack){
    if(!fullStack(*stack)){
        stack->top_index++;
        stack->Elements[stack->top_index]=node;
    }
    else printf("Stack is Full!");
}
Node* top(Stack stack){
    if(!emptyStack(stack)){
        return stack.Elements[stack.top_index];
    }
    else return NULL;
}
void pop(Stack *stack){
    if(!emptyStack(*stack)){
        stack->top_index--;
    }
    else
        printf("Error! Stack is empty");
}

int compareStates(State state1, State state2){
    return state1.x==state2.x&& state1.y==state2.y && state1.z==state2.z;
}
int findState(State state, Stack stack){
    while(!emptyStack(stack)){
        if(compareStates(state, top(stack)->state))
            return 1;
        pop(&stack);
    }
    return 0;
}
Node* BFS_Algorithm(State state){
    Stack Open, Close;
    makeNullStack(&Open);
    makeNullStack(&Close);
    Node *root=(Node*)malloc(sizeof(Node));
    root->state=state;
    root->Parent=NULL;
    root->no_function=0;
    push(root, &Open);
    while(!emptyStack(Open)){
        Node *node;
        node=top(Open);
        pop(&Open);
        push(node,&Close);
        if(checkGoal(node->state))
            return node;
        int opt;
        for(opt=1; opt<=6; opt++){
            State newState;
            makeNullState(&state);
            if(call_operator(node->state, &newState,opt)){
                if(findState(newState, Open)|| findState(newState, Close))
                    continue;
                Node* newNode=(Node*)malloc(sizeof(Node));
                newNode->state=newState;
                newNode->Parent=node;
                newNode->no_function=opt;
                push(newNode, &Open);
            }
        }
    }
    return NULL;
}

void print_WaysToGetGoal(Node* node){
    Stack stack;
    makeNullStack(&stack);
    while(node->Parent!=NULL){
        push(node, &stack);
        node=node->Parent;
    }
    push(node, &stack);
    int no_action=0;
    while(!emptyStack(stack)){
        printf("\nAction %d: %s", no_action, action[top(stack)->no_function]);
        printState(top(stack)->state);
        pop(&stack);
        no_action++;
    }
}
int main(){
    State state={10,0,0};
    Node *node=BFS_Algorithm(state);
    print_WaysToGetGoal(node);
    return 0;
}