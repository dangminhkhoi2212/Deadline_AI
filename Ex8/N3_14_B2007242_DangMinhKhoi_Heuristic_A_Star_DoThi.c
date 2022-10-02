#include <stdio.h>
#include <stdlib.h>
#define Max_operator 4
#define Maxlength 500
#define Max_Vertices 5
typedef struct
{
    int neighbor[Max_Vertices];// lưu giá trị g của mỗi đỉnh kề
    int h;
} Vertices;
typedef struct
{
    int num_vertices;
    Vertices v[Max_Vertices];
} Graph;
void initGraph(int num_vertices, Graph *G)
{
    G->num_vertices = num_vertices;
    int i, j;
    for (i = 0; i < num_vertices; i++)
        for (j = 0; j < num_vertices; j++)
        {
            G->v[i].neighbor[j] = 0;
            G->v[i].h = 0;
        }
}
const char name[] = {'A', 'B', 'C', 'D', 'G'};

typedef struct
{
    int vertex;
} State;

int compareStates(State state1, State state2)
{
    return state1.vertex == state2.vertex;
}
typedef struct Node
{
    State state;
    struct Node *parent;
    int f, g, h;
} Node;
typedef struct{
    Node *Elements[Maxlength];
    int size;
}List;
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
int goalCheck(State state, State goal){
    return compareStates(state, goal);
}
Node* A_Star(Graph G, State state, State goal){
    List Open, Close;
    makeNullList(&Open);
    makeNullList(&Close);
    Node *root=(Node*)malloc(sizeof(Node));
    root->state=state;
    root->parent=NULL;
    root->g=0;
    root->h=G.v[state.vertex].h;
    root->f=root->g+root->h;
    pushList(root, Open.size+1, &Open);
    while(!emptyList(Open)){
        Node *node=elementAt(1, Open);
        deleteList(1, &Open);
        pushList(node, Close.size+1, &Close);
        if(goalCheck(node->state, goal))
            return node;
        int opt;
        for(opt=0; opt<G.num_vertices; opt++){
            State newstate;
            if(G.v[node->state.vertex].neighbor[opt]>0){
                Node* newNode=(Node*)malloc(sizeof(Node));
                newNode->state.vertex=opt;
                newNode->parent=node;
                newNode->g=node->g+G.v[node->state.vertex].neighbor[opt];
                newNode->h=G.v[opt].h;
                newNode->f=newNode->g+newNode->h;
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
void printState(State state, Node *node)
{
    printf("%c(f = %d, g = %d, h = %d)", name[state.vertex], node->f, node->g, node->h);
}
void printWaysToGetGoal(Node *node){
    List listPrint;
    makeNullList(&listPrint);
    while(node->parent!=NULL){
        pushList(node, listPrint.size+1, &listPrint);
        node=node->parent;
    }
    pushList(node, listPrint.size+1, &listPrint);
    int no_action=0, i;
    for(i=listPrint.size; i>0; i--){
        printState(elementAt(i, listPrint)->state, elementAt(i, listPrint));
        if(i!=1)
            printf(" ==> ");
        no_action++;
    }
}
int main(){
    int i, j;
    Graph graph;
    freopen("test.txt", "r", stdin);
    initGraph(Max_Vertices, &graph);
    for(i=0; i<Max_Vertices; i++){
        int x;
        scanf("%d", &x);
        graph.v[i].h=x;
        for(j=0; j<Max_Vertices; j++){
            scanf("%d", &x);
            graph.v[i].neighbor[j]=x;
        }
    }
    State A,G;
    A.vertex=0; 
    G.vertex=4;
    Node *result=A_Star(graph, A, G);
    printWaysToGetGoal(result);
    return 0;
}