#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h> 
#include <string.h>

#define WIDTH 79
#define HEIGHT 79


void initialize(int grid[HEIGHT][WIDTH]);
void printGrid(int grid[HEIGHT][WIDTH], int prevGrid[HEIGHT][WIDTH]);
void nextGeneration(int grid[HEIGHT][WIDTH]);
int countAliveNeighbors(int grid[HEIGHT][WIDTH], int x, int y);
void setInitialConfiguration(int grid[HEIGHT][WIDTH]);
void clearGrid(int grid[HEIGHT][WIDTH]);
void copyGrid(int source[HEIGHT][WIDTH], int dest[HEIGHT][WIDTH]);
void clearScreen();

int main() {
    int grid[HEIGHT][WIDTH];
    int prevGrid[HEIGHT][WIDTH]; // 用于存储前一帧的网格状态
    unsigned int refreshRate;

    clearScreen();
    printf("欢迎使用生命游戏！\n");
    printf("请输入刷新率（帧每秒，fps）： ");
    scanf("%u", &refreshRate);

    initialize(grid);
    initialize(prevGrid); // 初始化前一帧网格状态
    setInitialConfiguration(grid);

    clearScreen();
    unsigned int sleepDuration = 1000000 / refreshRate; // 每秒钟有 1000000 微秒
    while (1) {
        printGrid(grid, prevGrid);
        copyGrid(grid, prevGrid); // 将当前网格状态复制到前一帧网格状态
        nextGeneration(grid);
        usleep(sleepDuration); // 根据刷新率延迟
    }

    return 0;
}

void initialize(int grid[HEIGHT][WIDTH]) {
    clearGrid(grid);
}

// 优化后的打印网格函数
void printGrid(int grid[HEIGHT][WIDTH], int prevGrid[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j] != prevGrid[i][j]) {
                // 仅当细胞状态发生变化时进行更新
                printf("\033[%d;%dH", i + 1, j + 1); // 光标定位
                if (grid[i][j] == 1) {
                    printf("O");
                } else {
                    printf(".");
                }
                fflush(stdout); // 刷新标准输出，确保立即更新显示
            }
        }
    }
    printf("\033[%d;%dH", HEIGHT + 1, 1); // 将光标移动到网格之后
}

// 计算下一代的状态
void nextGeneration(int grid[HEIGHT][WIDTH]) {
    int newGrid[HEIGHT][WIDTH];

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int aliveNeighbors = countAliveNeighbors(grid, i, j);

            if (grid[i][j] == 1) {  // 当前细胞是活的
                if (aliveNeighbors < 2 || aliveNeighbors > 3) {
                    // 1个或没有邻居，或者有4个或更多邻居，细胞死亡
                    newGrid[i][j] = 0;
                } else {
                    // 2个或3个邻居，细胞继续生存
                    newGrid[i][j] = 1;
                }
            } else {  // 当前细胞是死的
                if (aliveNeighbors == 3) {
                    // 恰好有3个邻居，细胞变为活的
                    newGrid[i][j] = 1;
                } else {
                    // 其它情况，细胞继续死亡
                    newGrid[i][j] = 0;
                }
            }
        }
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            grid[i][j] = newGrid[i][j];
        }
    }
}

// 计算活邻居数量
int countAliveNeighbors(int grid[HEIGHT][WIDTH], int x, int y) {
    int count = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }

            int nx = x + i;
            int ny = y + j;

            if (nx >= 0 && nx < HEIGHT && ny >= 0 && ny < WIDTH) {
                count += grid[nx][ny];
            }
        }
    }

    return count;
}

// 设置初始配置
void setInitialConfiguration(int grid[HEIGHT][WIDTH]) {
    char input[1024];
    int x, y;
    printf("设置初始配置\n");
    printf("输入细胞坐标 (行 列) 以空格分隔，范围为[0~79]，多个坐标以逗号隔开，例如 '1 1, 2 2, 3 3'：\n");

    // 读取输入
    printf("输入坐标: ");
    scanf(" %[^\n]", input);

    char *token = strtok(input, ",");
    while (token != NULL) {
        sscanf(token, " %d %d", &x, &y);
        if (x >= 0 && x < HEIGHT && y >= 0 && y < WIDTH) {
            grid[x][y] = 1;
            printf("细胞 (%d, %d) 已设置为活细胞\n", x, y);
        } else {
            printf("无效的坐标 (%d, %d)，已被忽略\n", x, y);
        }
        token = strtok(NULL, ",");
    }
}

// 清空网格
void clearGrid(int grid[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            grid[i][j] = 0;
        }
    }
}

// 复制网格
void copyGrid(int source[HEIGHT][WIDTH], int dest[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            dest[i][j] = source[i][j];
        }
    }
}

// 清屏函数
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}