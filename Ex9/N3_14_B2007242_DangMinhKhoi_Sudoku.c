#include <stdio.h>
#include <stdlib.h>
#define Maxlength 100
typedef struct{
    int x, y;
}Coord;
typedef struct{
    Coord data[Maxlength];
    int size;
}ListCoord;
void initListCoord(ListCoord *list){
    list->size=0;
}
void appendListCoord(ListCoord *list, Coord coord){
    list->data[list->size++]=coord;
}
#define Rows 4
#define Cols 4
typedef struct{
    int data [Rows*Cols][Rows*Cols];// lưu các cặp đỉnh có ràng buộc với nhau
    int n;
}Constrains;
void initConstrains(Constrains *constrains){
    int i,j;
    for(i=0; i<Rows*Cols; i++)
        for(j=0; j<Rows*Cols; j++)
            constrains->data[i][j]=0;
    constrains->n=Rows*Cols;
}
int indexOf(Coord coord){
    return coord.x*Rows+coord.y;
}
Coord positionOfVertex(int vertex){
    Coord coord;
    coord.x=vertex/Rows;
    coord.y=vertex%Cols;
    return coord;
}
int addConstrain(Constrains * constrains, Coord source, Coord target){
    int u=indexOf(source);
    int v=indexOf(target);
    if(constrains->data[u][v]==0){
        constrains->data[u][v]=1;
        constrains->data[v][u]=1;
        return 1;
    }
    return 0;
}
ListCoord getConstrains(Constrains constrains, Coord coord){
    int i;
    int v=indexOf(coord);
    ListCoord result;
    initListCoord(&result);
    for(i=0; i<constrains.n; i++){
        if(constrains.data[v][i]==1)
            appendListCoord(&result, positionOfVertex(i));
    }
    return result;
}
#define MAX_VALUE 4
#define EMPTY 0
#define AREA_SQUARE_SIZE 1
#define INF 999999

typedef struct{
    int cells[Rows][Cols];
    Constrains constrains;
}Sudoku;
void initSudoku(Sudoku *sudoku){
    int i, j;
    for(i=0; i<Rows; i++)
        for(j=0; j<Cols; j++)
            sudoku->cells[i][j]=EMPTY;
    initConstrains(&sudoku->constrains);
}
void initSudokuWithValues(Sudoku *sudoku, int inputs[Rows][Cols]){
    int i, j;
    for(i=0; i<Rows; i++)
        for(j=0; j<Cols; j++)
            sudoku->cells[i][j]=inputs[i][j];

    initConstrains(&sudoku->constrains);
}
void printSudoku(Sudoku sudoku){
    int i, j;
    printf("Sudoku:\n");
    for(i=0; i<Rows; i++){
        if(i%AREA_SQUARE_SIZE==0)
            printf("-------------------------\n");
        for(j=0; j<Cols; j++){
            if(j%AREA_SQUARE_SIZE==0)
                printf("| ");
            printf("%d ", sudoku.cells[i][j]);
        }
        printf("|\n");
    }
    printf("-------------------------\n");
}
int isFilledSudoku(Sudoku sudoku){
    int i, j;
    for(i=0; i<Rows; i++)
        for(j=0; j<Cols; j++)
            if(sudoku.cells[i][j]==EMPTY)
                return 0;
    return 1;
}
void printListCoord(ListCoord listcoord)
{
    int i;
    for (i = 0; i < listcoord.size; i++)
    {
        printf("dinh: %d\n", indexOf(listcoord.data[i]));
    }
}
void spreadConstrainsFrom(Coord position, Constrains *constrains, ListCoord *changeds){
    int row=position.x, column=position.y;
    int i,j;
    for(i=0; i<Rows; i++){
        if(i!=row){
	       	Coord pos={i, column};
	        if(addConstrain(constrains, position,pos))
	            appendListCoord(changeds, pos);
		}
            
    }

    for(i=0; i<Cols; i++){
        if(i!=column){
	       	Coord pos={row, i};
	        if(addConstrain(constrains, position, pos))
	            appendListCoord(changeds, pos);
		}
        
    }

    for(i=0;i<AREA_SQUARE_SIZE; i++){
        for(j=0; j<AREA_SQUARE_SIZE; j++){
            int areaX=(row/AREA_SQUARE_SIZE)*AREA_SQUARE_SIZE;
            int areaY=(column/AREA_SQUARE_SIZE)*AREA_SQUARE_SIZE;
            if(areaX+i!=row || areaY+j!=column){
                Coord pos={areaX+i, areaY+j};
                if(addConstrain(constrains, position, pos))
                    appendListCoord(changeds, pos);
            }
        }
    }
}
typedef struct
{
    int Elements[Maxlength];
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
int elementAt(int p, List list)
{
    return list.Elements[p - 1];
}
void pushList(int x, int position, List *list)
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
List getAvailableValues(Coord position, Sudoku sudoku){
    ListCoord posList=getConstrains(sudoku.constrains, position);
    int availables[MAX_VALUE];
    int i;
    for(i=0; i<=MAX_VALUE; i++)
        availables[i]=1;
    for (i = 0; i < posList.size; i++)
    {
        Coord pos= posList.data[i];
        if(sudoku.cells[pos.x][pos.y]!=EMPTY)
            availables[sudoku.cells[pos.x][pos.y]]=0;
    }
    List result;
    makeNullList(&result);
    for(i=1; i<=MAX_VALUE; i++)
        if(availables[i])
            pushList(i, result.size+1, &result);
    return result;
}
Coord getNextEmptyCell(Sudoku sudoku){
    int i, j;
    for(i=0; i<Rows; i++)
        for(j=0; j<Cols; j++){
            Coord pos={i, j};
            if(sudoku.cells[i][j]==EMPTY)
                return pos;
        }
}
Coord getNextMinDomainCell(Sudoku sudoku){
    int minLength=INF;
    int i, j;
    Coord result;
    for(i=0; i<Rows; i++)
        for(j=0; j<Cols; j++){
            if(sudoku.cells[i][j]==EMPTY){
                Coord pos={i,j};
                int availablesLength=getAvailableValues(pos, sudoku).size;
                if(availablesLength<minLength){
                    minLength=availablesLength;
                    result=pos;
                }
            }
        }
    return result;
}

int exploredCounter=0;

int sudokuBackTracking(Sudoku *sudoku){
    if(isFilledSudoku(*sudoku))
        return 1;
    Coord position=getNextMinDomainCell(*sudoku);
    List availables=getAvailableValues(position, *sudoku);
    if(availables.size==0){
        return 0;
    }
    int j;
    for(j=0; j<availables.size; j++){
        int value=availables.Elements[j];
        sudoku->cells[position.x][position.y] = value;
        exploredCounter++;
        if(sudokuBackTracking(sudoku)){
            return 1;
        }
        sudoku->cells[position.x][position.y]=EMPTY;
    }
    return 0;
}

Sudoku solveSudoku(Sudoku sudoku){
    int i , j;
    initConstrains(&sudoku.constrains);
    for(i=0; i<Rows; i++){
        for(j=0; j<Cols; j++){
                ListCoord history;
                initListCoord(&history);
                Coord pos = {i, j};
                spreadConstrainsFrom(pos, &sudoku.constrains, &history);
        }
    }
    for(i=0; i<Rows; i++){
        for(j=0; j<Cols; j++)
            printf("%d ", sudoku.constrains.data[i][j]);
        printf("\n");
    }
    exploredCounter=0;
    if(sudokuBackTracking(&sudoku))
        printf("Solved\n");
    else printf("Can not solve\n");
    printf("Explored %d states\n", exploredCounter);
    return sudoku;
}
int input1[9][9]={
    {5,3,0,0,7,0,0,0,0},
    {6,0,0,1,9,5,0,0,0},
    {0,9,8,0,0,0,0,6,0},
    {8,0,0,0,6,0,0,0,3},
    {4,0,0,8,0,3,0,0,1},
    {7,0,0,0,2,0,0,0,6},
    {0,6,0,0,0,0,2,8,0},
    {0,0,0,4,1,9,0,0,5},
    {0,0,0,0,8,0,0,7,9}
};
int input2[9][9]={
    {0,6,1,0,0,7,0,0,3},
    {0,9,2,0,0,3,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,8,5,3,0,0,0,0},
    {0,0,0,0,0,0,5,0,4},
    {5,0,0,0,0,8,0,0,0},
    {0,4,0,0,0,0,0,0,1},
    {0,0,0,1,6,0,8,0,0},
    {6,0,0,0,0,0,0,0,0}
};
int input3[4][4] = {
    {3, 2, 0, 0},
    {0, 0, 0, 2},
    {0, 0, 0, 0},
    {0, 1, 0, 0}
};
int main()
{
    Sudoku sudoku;
    initSudokuWithValues(&sudoku, input3);
    printSudoku(sudoku);
    Sudoku  result=solveSudoku(sudoku);
    printSudoku(result);
    return 0;
}
