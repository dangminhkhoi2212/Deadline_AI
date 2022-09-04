#include <stdio.h>
#include <stdlib.h>
#define Rows 3
#define Cols 3
#define Empty 0
#define Max_operator 4
#define Maxlength 500

typedef struct{
    int eightPuzzle[Rows][Cols];
    int emptyRow;
    int emptyCol;
}State;

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
int compareStates(State state1, State state2){
    if(state1.emptyRow!=state2.emptyRow || state1.emptyCol!=state2.emptyCol)
        return 0;
    int row, col;
    for(row=0; row<Rows; row++)
        for(col=0; col<Cols; col++)
            if(state1.eightPuzzle[row][col]!=state2.eightPuzzle[row][col])
                return 0;
    return 1;
}
int goalCheck(State state, State goal){
    return compareStates(state, goal);
}

int upOperator(State state, State *result){
    *result=state;
    int emptyRowCurrent=state.emptyRow, emptyColCurrent=state.emptyCol;
    if(state.emptyRow>0){
        result->emptyRow=emptyRowCurrent-1;
        result->emptyCol=emptyColCurrent;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent]=state.eightPuzzle[emptyRowCurrent-1][emptyColCurrent];
        result->eightPuzzle[emptyRowCurrent-1][emptyColCurrent]=Empty;
        return 1;
    }
    return 0;
}
int downOperator(State state, State *result){
    *result=state;
    int emptyRowCurrent=state.emptyRow, emptyColCurrent=state.emptyCol;
    if(emptyRowCurrent<Rows-1){
        result->emptyRow=emptyRowCurrent+1;
        result->emptyCol=emptyColCurrent;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent]=state.eightPuzzle[emptyRowCurrent+1][emptyColCurrent];
        result->eightPuzzle[emptyRowCurrent+1][emptyColCurrent]=Empty;
        return 1;
    }
    return 0;
}
int leftOperator(State state, State *result){
    *result=state;
    int emptyRowCurrent=state.emptyRow, emptyColCurrent=state.emptyCol;
    if(emptyColCurrent>0){
        result->emptyRow=emptyRowCurrent;
        result->emptyCol=emptyColCurrent-1;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent]=state.eightPuzzle[emptyRowCurrent][emptyColCurrent-1];
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent-1]=Empty;
        return 1;
    }
    return 0;
}
int rightOperator(State state, State *result){
    *result=state;
    int emptyRowCurrent=state.emptyRow, emptyColCurrent=state.emptyCol;
    if(emptyColCurrent<Cols-1){
        result->emptyRow=emptyRowCurrent;
        result->emptyCol=emptyColCurrent+1;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent]=state.eightPuzzle[emptyRowCurrent][emptyColCurrent+1];
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent+1]=Empty;
        return 1;
    }
    return 0;
}
int callOperators(State state, State *result, int opt){
    switch(opt){
        case 1: return upOperator(state, result);
        case 2: return downOperator(state, result);
        case 3: return leftOperator(state, result);
        case 4: return rightOperator(state , result);
        default: return 0;
    }
}
int heuristic1(State state, State goal){
    int row, col, count=0;
    for(row=0; row<Rows; row++){
        for(col=0; col<Cols; col++)
            if(state.eightPuzzle[row][col]!=goal.eightPuzzle[row][col])
                count++;
    }
    return count;
}
int heuristic2(State state, State goal){
    int row, col, row_g, col_g, count=0;
    for(row=0; row<Rows; row++)
        for(col=0; col<Cols; col++)
            if(state.eightPuzzle[row][col]!=Empty){
                for(row_g=0; row_g<Rows; row_g++)
                    for(col_g=0; col_g<Cols; col_g++)
                        if(state.eightPuzzle[row][col]==goal.eightPuzzle[row_g][col_g]){
                            count+= abs(row-row_g)+abs(col-col_g);
                            row_g=Rows;
                            col_g=Cols;
                        }
            }
    return count;
}

typedef struct Node{
    State state;
    struct Node *parent;
    int no_function;
    int heuristic;
    int f, g, h;
}Node;

typedef struct
{
    Node *Elements[Maxlength];
    int size;
} List;
void makeNullList(List *list)
{
    list->size = 0;
}
int emptyList(List list)
{
    return list.size == 0;
}
int fullList(List list)
{
    return list.size == Maxlength;
}
Node *elementAt(int p, List list)
{
    return list.Elements[p - 1];
}
void pushList(Node *x, int position, List *list)
{
    if (!fullList(*list))
    {
        int p;
        for (p = list->size; p >= position; p--)
        {
            list->Elements[p] = list->Elements[p - 1];
        }
        list->Elements[position - 1] = x;
        list->size++;
    }
    else
        printf("List is full");
}
void deleteList(int position, List *list)
{
    if (emptyList(*list))
        printf("List is empty");
    else if (position < 1 || position > list->size)
        printf("Position is not possible to delete");
    else
    {
        int p;
        for (p = position - 1; p < list->size; p++)
            list->Elements[p] = list->Elements[p + 1];
        list->size--;
    }
}
Node* findState(State state, List list, int *position){
    int i;
    for(i=1; i<=list.size; i++){
        if(compareStates(state, elementAt(i, list)->state))
        {
            *position=i;
            return elementAt(i, list);
        }
    }
    return NULL;
}
void swap(Node *a, Node *b){
    Node temp=*a;
    *a=*b;
    *b=temp;
}
void sortList(List *list)
{
    int i, j;
    for (i = 0; i < list->size; i++)
        for (j = i + 1; j < list->size; j++)
            if (list->Elements[i]->f > list->Elements[j]->f)
                swap(list->Elements[i], list->Elements[j]);
}
Node* A_Star(State state, State goal){
    List Open, Close;
    makeNullList(&Open);
    makeNullList(&Close);
    Node *root=(Node*)malloc(sizeof(Node));
    root->state=state;
    root->parent=NULL;
    root->g=0;
    root->h=heuristic2(state, goal);
    root->f=root->g+root->h;
    root->no_function=0;
    pushList(root, Open.size+1, &Open);
    while(!emptyList(Open)){
        Node *node=elementAt(1, Open);
        deleteList(1, &Open);
        pushList(node, Close.size+1, &Close);
        if(goalCheck(node->state, goal))
            return node;
        int opt;
        for(opt=1; opt<=Max_operator; opt++){
            State newstate;
            if(callOperators(node->state, &newstate, opt)){
                Node* newNode=(Node*)malloc(sizeof(Node));
                newNode->state=newstate;
                newNode->parent=node;
                (newNode->g)++;
                newNode->h=heuristic2(newstate, goal);
                newNode->f=newNode->g+newNode->h;
                newNode->no_function=opt;
                int pos_Open, pos_Close;
                Node *nodeFoundOpen=findState(newNode->state, Open, &pos_Open);
                Node *nodeFoundClose = findState(newNode->state, Close, &pos_Close);
                if(nodeFoundOpen==NULL && nodeFoundClose==NULL)
                    pushList(newNode, Open.size+1, &Open);
                else if(nodeFoundOpen!=NULL && nodeFoundOpen->g > newNode->g){
                    deleteList(pos_Open, &Open);
                    pushList(newNode,pos_Open, &Open);
                }
                else if (nodeFoundClose != NULL && nodeFoundClose->g > newNode->g)
                {
                    deleteList(pos_Close, &Close);
                    pushList(newNode, Open.size+1, &Open);
                }
            }
        }
        sortList(&Open);
    }
    return NULL;
}
const char *action[] = {
    "First State",
    "Move cell EMPTY to UP",
    "Move cell EMPTY to DOWN",
    "Move cell EMPTY to LEFT",
    "Move cell EMPTY to RIGHT"};
void printWaysToGetGoal(Node *node)
{
    List listPrint;
    makeNullList(&listPrint);
    while (node->parent != NULL)
    {
        pushList(node, listPrint.size + 1, &listPrint);
        node = node->parent;
    }
    pushList(node, listPrint.size + 1, &listPrint);
    int no_action = 0, i;
    for (i = listPrint.size; i > 0; i--)
    {
        printf("\nAction %d: %s", no_action, action[elementAt(i, listPrint)->no_function]);
        printState(elementAt(i, listPrint)->state);
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
    Node *p = A_Star(state, goal);
    printWaysToGetGoal(p);
    return 0;
}