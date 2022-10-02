//Đặng Minh Khôi B2007242
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
#define Rows 8
#define Cols 8
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
#define MAX_VALUE 9
#define EMPTY 0
#define AREA_SQUARE_SIZE 3
#define INF 999999

typedef struct{
    int cells[Rows][Cols];
    Constrains constrains;
}Sudoku_8Queen;
void initSudoku_8Queen(Sudoku_8Queen *sudoku){
    int i, j;
    for(i=0; i<Rows; i++)
        for(j=0; j<Cols; j++)
            sudoku->cells[i][j]=EMPTY;
    initConstrains(&sudoku->constrains);
}
void initSudoku_8QueenWithValues(Sudoku_8Queen *sudoku, int inputs[Rows][Cols]){
    int i, j;
    for(i=0; i<Rows; i++)
        for(j=0; j<Cols; j++)
            sudoku->cells[i][j]=inputs[i][j];

    initConstrains(&sudoku->constrains);
}
void printSudoku_8Queen(Sudoku_8Queen sudoku){
    int i, j;
    printf("Sudoku_8Queen:\n");
    printf("----------------\n");
    for(i=0; i<Rows; i++){
        for(j=0; j<Cols; j++){
            printf("%d ", sudoku.cells[i][j]);
        }
        printf("\n");
    }
    printf("----------------\n");
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
    //ràng buộc theo hàng
    for(i=0; i<Rows; i++){
        if(i!=row){
	       	Coord pos={i, column};
	        if(addConstrain(constrains, position,pos))
	            appendListCoord(changeds, pos);
		}     
    }
    //ràng buộc theo cột
    for(i=0; i<Cols; i++){
        if(i!=column){
	       	Coord pos={row, i};
	        if(addConstrain(constrains, position,pos))
	            appendListCoord(changeds, pos);
		}     
    }
    //ràng buộc chéo trên trái
    for(i=row-1, j=column-1; i>=0 && j>=0; i--, j--){
        Coord pos = {i, j};
        if (addConstrain(constrains, position, pos))
            appendListCoord(changeds, pos);
    }
    // ràng buộc chéo dưới trái
    for (i = row+1, j = column-1; i < Rows && j >= 0; i++, j--)
    {
        Coord pos = {i, j};
        if (addConstrain(constrains, position, pos))
            appendListCoord(changeds, pos);
    }
    // ràng buộc chéo trên phải
    for (i = row-1, j = column+1; i >= 0 && j < Cols; i--, j++)
    {
        Coord pos = {i, j};
        if (addConstrain(constrains, position, pos))
            appendListCoord(changeds, pos);
    }
    // ràng buộc chéo dưới phải
    for (i = row+1, j = column+1; i < Rows && j <Cols; i++, j++)
    {
        Coord pos = {i, j};
        if (addConstrain(constrains, position, pos))
            appendListCoord(changeds, pos);
    }
}
int exploredCounter=0;
int checkConstrains(Sudoku_8Queen sudoku,Coord position){
    ListCoord list = getConstrains(sudoku.constrains, position);//danh sách các ô có ràng buộc với position
    int k;
    for (k = 0; k < list.size; k++)
    {
        Coord pos = list.data[k];
        if (sudoku.cells[pos.x][pos.y] == 1)
            return 1;
    }
    return 0;
}
int count=0;
int sudokuBackTracking(Sudoku_8Queen *sudoku, int column){
    // printf("exploredCounter: %d\n", exploredCounter);
    // printSudoku_8Queen(*sudoku);
    if(column>Cols)
        return 1;
    int i;
    for(i=0; i<Cols; i++){
        Coord position={i, column};
        // kiểm tra ô này có nằm trong ràng buộc với những ô khác không
        if(checkConstrains(*sudoku, position)==0){
            sudoku->cells[i][column] = 1;
            exploredCounter++;
            if(sudokuBackTracking(sudoku, column+1))
                return 1;
            sudoku->cells[i][column] = 0;
        }
    }
    return 0;
}
Sudoku_8Queen solveSudoku_8Queen(Sudoku_8Queen sudoku){
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
    exploredCounter=0;
    if(sudokuBackTracking(&sudoku,0))
        printf("Solved\n");
    else printf("Can not solve\n");
    printf("Explored %d states\n", exploredCounter);
    return sudoku;
}
int input1[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
} ;
int main()
{
    Sudoku_8Queen sudoku;
    initSudoku_8QueenWithValues(&sudoku, input1);
    printSudoku_8Queen(sudoku);
    Sudoku_8Queen  result=solveSudoku_8Queen(sudoku);
    printSudoku_8Queen(result);
    return 0;
}
