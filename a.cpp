//
// main c
// snake
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define snake_max_length 20
#define snake_head 'H'
#define snake_body 'X'
#define blank_cell ' '
#define snake_food '$'
#define wall_cell '*'
#define max_distance 10000000

int snakemove(int, int);
int put_money();
int output();
int gameover();

char map[100][100] = {
    "************",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "*          *",
    "************" };

int snakelength = 5, direct = 4;
int move[][3] = {{0,0,0},{0,-1,0},{0,1,0},{0,0,-1},{0,0,1}};
int snake[1000][3] = {{0,1,5},{0,1,4},{0,1,3},{0,1,2},{0,1,1}};
int money[3], n, m, mode;
bool live = true;
char c;

//output() 输出地图及蛇当前状态
int output() {
    system("cls");
    for (int i = 0; i <= n-1; i++)
        for (int j = 0; j <= m-1; j++)if (map[i][j] == 'X' || map[i][j] == 'H') map[i][j] = ' ';
    for (int i = 1; i < snakelength; i++) map[snake[i][1]][snake[i][2]] = snake_body;
    map[snake[0][1]][snake[0][2]] = snake_head;
    map[money[1]][money[2]] = snake_food;
    for (int i = 0; i <= n-1; i++) {
        for (int j = 0; j <= m-1; j++) printf("%c", map[i][j]);
        printf("\n");
    }
    return 0;
}

//  gameover() 判定为死亡
int gameover() {
    printf("Gameover\n");
    system("pause");
}

// win() 判定为胜利
int win() {
    printf("You win\n");
    system("pause");
}

// put_money() 随机地在地图上放置食物且不与蛇或障碍物重叠
int put_money() {
    money[1] = (rand()+(clock()%n))%(n-2)+1;
    money[2] = (rand()+(clock()%m))%(m-2)+1;
    bool flag = true; // flag  判断食物放置位置是否合法
    for (int i = 0; i < snakelength; i++) if (money[1] == snake[i][1] && money[2] == snake[i][2] || map[money[1]][money[2]] == wall_cell) {
        flag = false;
        break;
    }
    if (!flag) put_money();
}

// snakemove() 蛇向direct方向移动一步
int snakemove() {
    for (int i = snakelength; i >= 1; i--) {
        snake[i][1] = snake[i-1][1];
        snake[i][2] = snake[i-1][2];
    }
    snake[0][1] += move[direct][1];
    snake[0][2] += move[direct][2];
}

// check() 监测蛇当前是否因为触壁或触身而死亡
bool check() {
    for (int i = 0; i < snakelength; i++) {
        for (int j = i+1; j < snakelength; j++) {
            if (snake[i][1] == snake[j][1] && snake[i][2] == snake[j][2]) return false;
        }
        if (map[snake[i][1]][snake[i][2]] == wall_cell) return false;
    }
    return true;
}

// distance(x, y) 用宽搜计算点(x,y)通过行走‘ ’或'$'到达money[]的位置的距离
int distance(int x, int y) {
    if (x == money[1] && y == money[2]) return 0;
    int f[100][100];
    for (int i = 0; i <= n-1; i++)
        for (int j = 0; j <= m-1; j++) f[i][j] = 10000;
    f[x][y] = 0;
    int d[10000][3];
    d[1][1] = x;
    d[1][2] = y;
    int i = 0, j = 1;
    while (i != j) {
        i++;
        int t = f[d[i][1]][d[i][2]];
        for (int k = 1; k <= 4; k++) {
            x = d[i][1]+move[k][1];
            y = d[i][2]+move[k][2];
            if (x == money[1] && y == money[2]) return t+1;
            bool flag = true; // 判断通过的路径是否合法
            for (int ll = 0; ll <= snakelength; ll++) 
                if (x == snake[ll][1] && y == snake[ll][2]) flag = false;
            if (!flag) continue;
            if (map[x][y] != wall_cell && f[x][y] > t+1) {
                j++;
                d[j][1] = x;
                d[j][2] = y;
                f[x][y] = t+1;
            }
        }
    }
    return (max_distance);
}

// init() 选择读入，是用原始地图还是用"map.in"来 导入地图
int init() {
    printf("Please input a or b\n");
    printf("a : use the original map\n");
    for (int i = 0; i <= n-1; i++) {
        for (int j = 0; j <= m-1; j++) printf("%c", map[i][j]);
        printf("\n");
    }
    printf("b : freopen map.in as the map.\n     first input n m as rows and cols\n     then input the map with n rows and m cols\n");
    printf("warning : map[1][1..5] should be empty ,\n     and direction is right at the beginning ,\n     and we will ensure the map surrounding by *\n");
    while (1) {
        c = getch();
        if (c == 'a') return 0;
        if (c == 'b') {
            freopen("map.in", "r", stdin);
            scanf("%d %d", &n, &m);
            for (int i = 0; i <= n; i++)
                for (int j = 0; j <= m; j++) map[i][j] = ' ';
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                  //  scanf("%c", &map[i][j]);
                    if ((map[i][j] = getchar()) ==EOF) break;
                    if (i == 0 || j == 0 || i == n-1 || j == m-1) map[i][j] = '*';
                }
                c = getchar();
                while (c != '\n') if ((c = getchar()) == EOF) break;
            }
            while ((c = getchar()) != EOF) c = getchar();
            for (int i = 0; i < n; i++) map[i][0] = '*';
            for (int i = 0; i < n; i++) map[i][m-1] = '*';
            for (int i = 0; i < m; i++) {
                map[0][i] = '*';
                map[n-1][i] = '*';
            }
            fclose(stdin);
            return 0;
        }
        printf("Please input as requirment\n");
    }
}

// ok() 检查snake到尾部的距离是否符合要求
bool ok(int step) {
    if (!check()) return false;
    money[1] = snake[snakelength][1];
    money[2] = snake[snakelength][2]; // 为了方便，暂时吧money作为记录蛇尾的坐标
    if (snake[0][1] == money[1] && snake[0][2] == money[2]) return false;
    if (distance(snake[0][1], snake[0][2]) >= 1 && distance(snake[0][1], snake[0][2]) != max_distance) 
        return true;
}

// findtail(dir) snake向dir方向移动之后是否可以找到尾部 
bool findtail(int dir) {
    int t_snake[1000][3], t_money[3];
    for (int i = 0; i <= snakelength; i++) for (int j = 1; j <= 2; j++) t_snake[i][j] = snake[i][j];
    for (int i = 1; i <= 2; i++) t_money[i] = money[i];
    int temp = direct;
    direct = dir;
    snakemove();
    direct = temp;
    bool flag = ok(1);
    // 下面为还原
    for (int i = 0; i <= snakelength; i++) for (int j = 1; j <= 2; j++) snake[i][j] = t_snake[i][j];
    for (int i = 1; i <= 2; i++) money[i] = t_money[i];
    return flag;
}

// ai() 通过算法为蛇确定前进方向
int ai() {
    int x, y;
    int dis = distance(snake[0][1], snake[0][2]);
    if (dis == max_distance) {//如果蛇不能到达食物所在位置
        for (int i = 1; i <= 4; i++) {//在四个方向中找到一个可以追到尾部且合法的方向
            x = snake[0][1]+move[i][1];
            y = snake[0][2]+move[i][2];
            if (map[x][y] == ' ' && findtail(i)) return i;
        }
    }
    else {//蛇能够到达食物所在位置
        bool weight = true;
        dis = n*m;
        int dir = 0;
        for (int i = 1; i <= 4; i++) {//枚举上下左右
            x = snake[0][1]+move[i][1];
            y = snake[0][2]+move[i][2];
            int temp = distance(x, y);            
            //weight 是我设定的一个加权随机数，在多个同样优的方案中随机一个作为蛇前进的方向，避免死循环
            if (dis == n*m || temp < dis) weight = true;
            else weight = (clock()%3);
            //在合法的状态下选择下一步离食物尽量近的方向
            if ((map[x][y] == ' ' || map[x][y] == '$')&& findtail(i) && temp<=dis && temp != max_distance && weight) {
                dir = i;
                dis = distance(x, y);
            }
        }
        if (dis != n*m) return dir;
        dis = 0;
        //若是没有合法的方案
        for (int i = 1; i <= 4; i++) {
            x = snake[0][1]+move[i][1];
            y = snake[0][2]+move[i][2];
            //选择离食物尽量远的方向
            if ((map[x][y] == ' ')&& findtail(i) && distance(x, y)>dis) {
                dir = i;
                dis = distance(x, y);
            }
        }
        return dir;
    }
}

int main() {
    clock_t time; 
    int temp = direct;
    n = 12;
    m = 12;
    init();//选择地图
    money[1] = n+1;
    money[2] = m+1;
    output();
    put_money();//放置食物
    output();//输出初始状态
    
    printf("press p to continue\n");
    while (1) {
        c = getch();
        if (c == 'p') break;
        printf("press p to continue\n");
    }
    
    mode = 0;//mode为选择的模式 ai或者人手动
    while (mode != 1 && mode != 2) {
        printf("select mode\ninput\t 1:AI (It has some bugs when the snake grows very long)\n\t2:play\n");
        c = getch();
        mode = c-'0';
    }
    
    printf("press p to star\n");
    while (1) {
        c = getch();
        if (c == 'p') break;
        printf("press p to star\n");
    }
    // mo为我设定的刷新的频率
    int mo = 250;
    if (mode != 2) mo /= 2;
    
    while (live) {//live 判断蛇是否存活
        if (kbhit()) c = getch();
        if (c == 'w' && direct != 2) temp = 1;
        if (c == 's' && direct != 1) temp = 2;
        if (c == 'a' && direct != 4) temp = 3;
        if (c == 'd' && direct != 3) temp = 4;
        //temp 为了记录人工读入的方向，并且避免蛇往反方向走直接死亡
        time = clock();
        if (time% mo == 0) {//到达一定频率进行刷新
            if (mode == 2) direct = temp;
            else direct = ai();
            //通过不同的模式为蛇确定移动的方向
            if (direct == 0) //防bug
                for (direct = 1; direct <= 4; direct++) 
                    if (map[snake[0][1]+move[direct][1]][snake[0][2]+move[direct][2]] == ' ') break;
            snakemove();//蛇朝direct方向移动
            if (snake[0][1] == money[1] && snake[0][2] == money[2]) snakelength++;
            if (snake[0][1] == money[1] && snake[0][2] == money[2]) put_money();  
            //判断是否吃掉了食物
            live = check();// 判断蛇是否存活
            output();//输出当前状态
            if (mode != 2) printf("Speed up in case of you feeling boring!\n");
        }
    }
    live = true;
    for (int i = 0; i <= n-1; i++)
        for (int j = 0; j <= m-1; j++) if (map[i][j] == ' ') live = false;
    if (!live) gameover();
    else win();
    
}

