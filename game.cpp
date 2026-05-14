#include "stdafx.h"
#include "console.h"
#include <conio.h>
#include <stdio.h>
#include "R32M.h"
#include <iostream>
#include <string>

// комментарии, взаимодействия с пользователем, тип игры, убрать m_position(done)

using namespace std;

// структура точка
struct point {
    int x = -1, y = -1;
    point(int x = 0, int y = 0) : x(x), y(y) {}
};

// класс очереди для поиска в ширину в game::check()
class BFSQueue {
    point* data;
    int head, tail;
    int capacity;

public:
    BFSQueue(int maxSize = 10000) {
        capacity = maxSize;
        data = new point[capacity];
        head = 0;
        tail = 0;
    }

    ~BFSQueue() {
        delete[] data;
    }

    // Добавить точку в конец
    void push(int x, int y) {
        data[tail++] = point(x, y);
    }

    void push(const point& p) {
        data[tail++] = p;
    }

    // Получить первую точку
    point front() {
        return data[head];
    }

    // Удалить первую точку
    void pop() {
        head++;
    }

    // Проверить, пуста ли очередь
    bool empty() {
        return head >= tail;
    }

    // Размер очереди
    int size() {
        return tail - head;
    }

    // Очистить очередь
    void clear() {
        head = tail = 0;
    }
};

//класс клетки на доске
class cell {
private:
    int m_type; // 0 - sea, 1 - island
public:
    cell(int type = 0) { m_type = type; };
    void settype(int type) { m_type = type; };
    int gettype() const { return m_type; };
};

//класс доски
class board {
protected:
    //размеры доски
    int m_row;
    int m_col;
    //матрица из клеток
    cell** m_matrix;
public:
    board(int row = 0, int col = 0);
    board(const board& b);
    ~board();
    int getrow() const { return m_row; };
    int getcol() const { return m_col; };
    void setcelltype(int x, int y, int type); // x in {0,..., row - 1}, y in {0,..., col - 1}
    int getcelltype(int x, int y) const;
};

board::board(const board& b) : m_row(b.m_row), m_col(b.m_col) {
    m_matrix = new cell * [m_row];
    for (int i = 0; i < m_row; ++i) {
        m_matrix[i] = new cell[m_col];
        for (int j = 0; j < m_col; ++j)
            m_matrix[i][j] = b.m_matrix[i][j];
    }
}

board::~board() {
    for (int i = 0; i < m_row; ++i)
        delete[] m_matrix[i];
    delete[] m_matrix;
}

board::board(int row, int col) : m_row(row), m_col(col) {
    m_matrix = new cell * [row];
    for (int i = 0; i < row; ++i)
        m_matrix[i] = new cell[col];
}

void board::setcelltype(int x, int y, int type) {
    m_matrix[x][y].settype(type);
}

int board::getcelltype(int x, int y) const {
    return m_matrix[x][y].gettype();
}

//класс игры собственно
class game {
private:
    point m_graphical_begin; // левый нижний угол доски в графических координатах
    board m_board; // сама доска
    point m_board_position; // последний сделанный ход в логических координатах
    //point m_position; удаленное поле
    point m_cursor_position; // позиция курсора в данный момент
    point* m_solution; // массив из точек решения в логических координатах
    int m_n_solution; // количество точек в решении
    int m_gametype; // собственно тип игры 
public:
    game(board b, point p = { 0,0 }, int type = 0);
    ~game();
    void initboard(); // алгос на создание доски, заполняет m_solution, m_n_solution
    void printboard() const; // вывод доски
    void cursormove(point graphical); // изменение положения курсора
    //void makemove(point graphical); удаленная функция
    int makemove0(point graphical, point& lastmove); // сделать ход в m_gametype = 0
    int makemove1(point graphical); // сделать ход в m_gametype = 1
    void f1(); // нажатие f1(1)
    void f10(); // нажатие f10(0)
    void process_of_game(); // game main loop
    int check(); // смотрит есть ли решение в доске полученной из initboard() и записывает в solution
    int checkmoves(point logical) const; // смотрит валидность перехода(добавления ребенка) в BFS game::check()
    point logical_to_graphical(point logical) const; // конвертирует из логических в графические координаты
    point graphical_to_logical(point graphical) const; // конвертирует из графических в логические координаты
    int checkmakemove(point logical) const; // смотрит валиден ли переход конем в эту клетку (используется в makemove0() makemove1())
    void win() const; // победа
    int islose0(point lastmove) const; // проиграна ли игра в gametype = 0
};

point game::logical_to_graphical(point logical) const {
    point graphical = { m_graphical_begin.x + logical.y, m_graphical_begin.y + m_board.getrow() - logical.x };
    return graphical;
}

point game::graphical_to_logical(point graphical) const {
    point logical = { -graphical.y + m_graphical_begin.y + m_board.getrow(), graphical.x - m_graphical_begin.x };
    return logical;
}

// принтит в нужный point
void colorprint(point graphical, WORD color, LPCTSTR format) {
    ColorPrint(graphical.x, graphical.y, color, format);
}

//свечение последней клетки
void game::win() const {
    WORD colors[] = { B_D_BLUE, B_D_GREEN, B_D_RED, B_D_CYAN, B_D_MAGENTA, B_D_YELLOW,
        B_D_WHITE, B_L_BLUE, B_L_GREEN, B_L_RED, B_L_CYAN, B_L_MAGENTA, B_L_YELLOW, B_L_WHITE };
    point graphical = logical_to_graphical(m_board_position);
    for (auto c : colors) {
        colorprint(graphical, c, "I");
        Sleep(100);
    }
}

//принтит число в нужный point
void printnum(point graphical, WORD color, int num) {
    char str[3];
    str[0] = num / 10 + 48;
    str[1] = num % 10 + 48;
    str[2] = 0;
    colorprint(graphical, color, str);
}

// game main loop
void game::process_of_game() {
    m_board_position = { 0, 0 };
    point graphical = logical_to_graphical(m_board_position);
    colorprint(graphical, B_D_MAGENTA, "I");
    cursormove(graphical);
    //m_position = { 20, 10 + m_board.getrow() }; // переделать убрать m_position
    //ColorPrint(20, 10 + m_board.getrow(), B_D_MAGENTA, "I");
    //cursormove(20, 10 + m_board.getrow());
    char x = 7;
    int remain = 0;
    //вывод числа ходов
    if (m_gametype == 1) {
        remain = m_n_solution - 1;
        printnum({ m_graphical_begin.x, m_graphical_begin.y - 1 }, F_D_WHITE, remain);
        cursormove(graphical);
    }
    //это для gametype = 0
    point lastmove = { -1, -1 };
    // цикл
    while (x != '0') {
        x = _getch();
        switch (x) {
        case 'w':
            cursormove({ m_cursor_position.x, m_cursor_position.y - 1 });
            break;
        case 'a':
            cursormove({ m_cursor_position.x - 1, m_cursor_position.y });
            break;
        case 's':
            cursormove({ m_cursor_position.x, m_cursor_position.y + 1 });
            break;
        case 'd':
            cursormove({ m_cursor_position.x + 1, m_cursor_position.y });
            break;
        case '\r':
            if (m_gametype == 0) {
                if (makemove0(m_cursor_position, lastmove)) {
                    if (islose0(lastmove) && !(m_board_position.x == m_board.getrow() - 1 && m_board_position.y == m_board.getcol() - 1)) {
                        VisibleCursor(false);
                        colorprint({ m_graphical_begin.x, m_graphical_begin.y - 1 }, F_D_WHITE, "Game over! No island to move.");
                        f10();
                        colorprint({ m_graphical_begin.x, m_graphical_begin.y - 1 }, F_D_WHITE, "Game over! No island to move. Press any button");
                        x = '0';
                    }
                }
            }
            else {
                if (makemove1(m_cursor_position)) {
                    //вывод оставшихся ходов
                    --remain;
                    printnum({ m_graphical_begin.x, m_graphical_begin.y - 1 }, F_D_WHITE, remain);
                    cursormove(m_cursor_position);
                    //проверка не проиграна ли игра
                    if (!remain && !(m_board_position.x == m_board.getrow() - 1 && m_board_position.y == m_board.getcol() - 1)) {
                        VisibleCursor(false);
                        colorprint({ m_graphical_begin.x, m_graphical_begin.y - 1 }, F_D_WHITE, "Game over!");
                        f10();
                        colorprint({ m_graphical_begin.x, m_graphical_begin.y - 1 }, F_D_WHITE, "Game over! Press any button");
                        x = '0';
                    }
                }
            }
            //случай победы
            if (m_board_position.x == m_board.getrow() - 1 && m_board_position.y == m_board.getcol() - 1) {
                VisibleCursor(false);
                win();
                colorprint({ m_graphical_begin.x, m_graphical_begin.y - 1 }, F_D_WHITE, "Congratulations! Press any button");
                x = '0';
            }
            break;
        case '1':
            f1();
            break;
        case '0':
            VisibleCursor(false);
            f10();
            colorprint({ m_graphical_begin.x, m_graphical_begin.y - 1 }, F_D_WHITE, "Press any button");
            break;
        }
    }
}

//принимает point куда сделать ход
int game::makemove1(point graphical) {
    point graphical_prev_position = logical_to_graphical(m_board_position);
    point logical = graphical_to_logical(graphical);
    int x = checkmakemove(logical); // посмотрите что возращает checkmakemove() в его определении
    //проверка валидности
    if (!x) {
        m_board_position = logical;
        colorprint(graphical_prev_position, F_D_YELLOW, "I");
        colorprint(graphical, B_D_MAGENTA, "I");
        cursormove(graphical);
        return 1;
    }
    //красота
    if (x == -1)
        colorprint(graphical, B_D_RED, "O");
    else
        colorprint(graphical, B_D_RED, "I");
    cursormove(graphical);
    Sleep(1000);
    if (x == -1)
        colorprint(graphical, F_D_BLUE, "O");
    else
        colorprint(graphical, F_D_YELLOW, "I");
    cursormove(graphical);
    return 0;
}

int game::makemove0(point graphical, point& lastmove) {
    point graphical_prev_position = logical_to_graphical(m_board_position);
    point logical = graphical_to_logical(graphical);
    int x = checkmakemove(logical);
    // проверка валидности + что не вернулись назад
    if (!x && !(logical.x == lastmove.x && logical.y == lastmove.y)) {
        lastmove = m_board_position;
        m_board_position = logical;
        colorprint(graphical_prev_position, F_D_YELLOW, "I");
        colorprint(graphical, B_D_MAGENTA, "I");
        cursormove(graphical);
        return 1;
    }
    //еще красота
    if (x == -1)
        colorprint(graphical, B_D_RED, "O");
    else
        colorprint(graphical, B_D_RED, "I");
    cursormove(graphical);
    Sleep(1000);
    if (x == -1)
        colorprint(graphical, F_D_BLUE, "O");
    else
        colorprint(graphical, F_D_YELLOW, "I");
    cursormove(graphical);
}

// проиграна ли игра в типе 0
int game::islose0(point lastmove) const {
    point moves[] = { {-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1} };
    int flag = 0;
    // пытаемся найти все ходы конем по островам, которые не совпадают с предыдущим ходом
    for (auto m : moves) {
        point nw = { m_board_position.x + m.x, m_board_position.y + m.y };
        if (checkmoves(nw) && !(nw.x == lastmove.x && nw.y == lastmove.y)) {
            return 0; //игра не проиграна
        }
    }
    return 1; // игра проиграна
}

void game::f1() {
    point moves[] = { {-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1} };
    point remind[8];
    int cnt = 0;
    // проверяем все возможные ходы, красим их в зеленый, запоминаем что покрасили
    for (auto m : moves) {
        point nw = { m_board_position.x + m.x, m_board_position.y + m.y };
        if (checkmoves(nw)) {
            point graphical_nw = logical_to_graphical(nw);
            colorprint(graphical_nw, B_D_GREEN, "I");
            remind[cnt] = graphical_nw;
            ++cnt;
        }
    }
    cursormove(m_cursor_position);
    Sleep(2000);
    //убираем покраску из запомненных
    for (int i = 0; i < cnt; ++i)
        colorprint(remind[i], F_D_YELLOW, "I");
    cursormove(m_cursor_position);
}

void game::f10() {
    for (int i = 0; i < m_n_solution; ++i) {
        point graphical = logical_to_graphical(m_solution[i]);
        colorprint(graphical, B_D_GREEN, "I");
        cursormove(m_cursor_position);
        Sleep(500);
    }
}

//void game::makemove(point graphical) {
//    point graphical_prev_position = logical_to_graphical(m_board_position);
//    point logical = graphical_to_logical(graphical);
//    int x = checkmakemove(logical);
//    if (!x) {
//        m_board_position = logical;
//        colorprint(graphical_prev_position, F_D_YELLOW, "I");
//        colorprint(graphical, B_D_MAGENTA, "I");
//        cursormove(graphical);
//    }
//    else {
//        if (x == -1)
//            colorprint(graphical, B_D_RED, "O");
//        else
//            colorprint(graphical, B_D_RED, "I");
//        cursormove(graphical);
//        Sleep(1000);
//        if (x == -1)
//            colorprint(graphical, F_D_BLUE, "O");
//        else
//            colorprint(graphical, F_D_YELLOW, "I");
//        cursormove(graphical);
//    }
//}

int game::checkmakemove(point logical) const {
    point moves[] = { {-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1} };
    int is_in_moves = 0;
    // проверяем является ли он одним из ходов коня
    for (auto m : moves) {
        if (m_board_position.x + m.x == logical.x && m_board_position.y + m.y == logical.y) {
            is_in_moves = 1;
            break;
        }
    }
    //предельно ясно
    if (is_in_moves && m_board.getcelltype(logical.x, logical.y))
        return 0; // нужный island
    if (m_board.getcelltype(logical.x, logical.y))
        return 1; // ненужный island
    return -1; // sea
}

// не даем курсору выйти из поля
void game::cursormove(point graphical) {
    point logical = graphical_to_logical(graphical);
    if (logical.x < 0 || logical.y < 0 || logical.x >= m_board.getrow() || logical.y >= m_board.getcol())
        return;
    MoveCursor(graphical.x, graphical.y);
    m_cursor_position = { graphical.x, graphical.y };
}

game::~game() {
    if (m_solution != NULL) {
        delete[] m_solution;
    }
}


void game::printboard() const {
    /*for (int i = 2; i >= 0; --i) {
        for (int j = 0; j < 3; ++j)
            cout << m_board.getcelltype(j, i) << " ";
        cout << endl;
    }*/
    int row = m_board.getrow(), col = m_board.getcol();
    //point pos = { 10, 10 };
    // вывод верхней границы поля
    for (int x = 0; x < col; ++x)
        ColorPrint(m_graphical_begin.x + x, m_graphical_begin.y, F_D_RED, "_");
    //вывод остального
    for (int i = row - 1; i >= 0; --i) {
        //вывод левой границы
        ColorPrint(m_graphical_begin.x - 1, m_graphical_begin.y + row - i, F_D_RED, "|");
        //вывод sea и island в строке
        for (int j = 0; j < col; ++j) {
            if (m_board.getcelltype(i, j)) {
                ColorPrint(m_graphical_begin.x + j, m_graphical_begin.y + row - i, F_D_YELLOW, "I");
            }
            else {
                ColorPrint(m_graphical_begin.x + j, m_graphical_begin.y + row - i, F_D_BLUE, "O");
            }
        }
        //вывод правой границы
        ColorPrint(m_graphical_begin.x + col, m_graphical_begin.y + row - i, F_D_RED, "|");
    }
    //вывод нижней границы
    for (int x = 0; x < col; ++x)
        ColorPrint(m_graphical_begin.x + x, m_graphical_begin.y + row + 1, F_D_RED, "T");
}

game::game(board b, point p, int type) : m_graphical_begin({ 20, 10 }), m_board(b), m_board_position({ 0, 0 }), m_cursor_position({ 0, 0 }),
m_solution(NULL), m_n_solution(0), m_gametype(type) {};

// проверяет не вышел ли ребенок за границы и что он island
int game::checkmoves(point a) const {
    if (a.x < 0 || a.y < 0 || a.x >= m_board.getrow() || a.y >= m_board.getcol())
        return 0;
    if (!m_board.getcelltype(a.x, a.y))
        return 0;
    return 1;
}

//алгоритм проверки поля на решаемость
int game::check() {
    int row = m_board.getrow(), col = m_board.getcol();
    //инициализация массива посещений и родителей
    int** used = new int* [row];
    point** parent = new point * [row];
    for (int i = 0; i < row; ++i) {
        used[i] = new int[col];
        parent[i] = new point[col];
        for (int j = 0; j < col; ++j) {
            used[i][j] = 0;
            parent[i][j] = { -1, -1 };
        }
    }
    point moves[] = { {-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1} };
    //стандартный BFS без приоритетной очереди(тк расстояния нам и не нужны)
    BFSQueue q;
    q.push({ 0, 0 });
    while (!q.empty()) {
        point cur = q.front();
        q.pop();
        if (cur.x == row - 1 && cur.y == col - 1)
            break;
        if (used[cur.x][cur.y])
            continue;
        used[cur.x][cur.y] = 1;
        for (auto m : moves) {
            point nw = { cur.x + m.x, cur.y + m.y };
            if (!checkmoves(nw))
                continue;
            if (!used[nw.x][nw.y]) {
                q.push(nw);
                parent[nw.x][nw.y] = cur;
            }
        }
    }
    //проверяем нашли ли в итоге путь
    if (parent[row - 1][col - 1].x == -1)
        return 0;
    //считаем количество point в решении
    point cur = { row - 1, col - 1 };
    int n_solution = 0;
    while (cur.x != -1) {
        n_solution++;
        cur = parent[cur.x][cur.y];
    }
    // заполняем решение
    m_solution = new point[n_solution];
    cur = { row - 1, col - 1 };
    for (int i = n_solution - 1; i >= 0; --i) {
        m_solution[i] = cur;
        cur = parent[cur.x][cur.y];
    }
    m_n_solution = n_solution;
    //чистим, эх а вот можно было бы если пользоваться вектором 
    // (это шутка: он бы здесь не работал как и любой контейнер STL)))
    for (int i = 0; i < row; ++i) {
        delete[] used[i];
        delete[] parent[i];
    }
    delete[] used;
    delete[] parent;
    return 1;
}

//заполняем доску с нужной вероятностью островами
void game::initboard() {
    int row = m_board.getrow(), col = m_board.getcol();
    int mean = (row * col / 2 + (row + col) / 3) / 2; // среднее между половиной островов и минимально возможной для решения
    double prob = (double)mean / row / col;
    //max row * col / 4  min (row + col) / 3;
    //цикл пока не найдем доску с существующим решением
    do {
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if (rnunif() < prob)
                    m_board.setcelltype(i, j, 1);
                else
                    m_board.setcelltype(i, j, 0);
            }
        }
        m_board.setcelltype(0, 0, 1);
        m_board.setcelltype(row - 1, col - 1, 1);
    } while (!check());
}
// хотел сделать наследование от game но вспомнил, что мы пока не умеем:)
//class movegame {
//private: 
//    game m_game;
//    int m_limmoves;
//    int m_curmoves;
//};
//
//class nobackgame {
//private:
//    game m_game;
//    cell* last;
//};

int main(int argc, char* argv[]) {
    rninit(time(NULL));
    // спрашиваем тип игры и размеры
    int type;
    cout << "type of game(0 - no back, 1 - moves): ";
    cin >> type;
    int row, col;
    cout << "how many rows: ";
    cin >> row;
    cout << "how many cols: ";
    cin >> col;
    //инициализируем то что нужно
    InitConsole("game", 400, 400);
    //ClearConsole();
    cout << "moving: w - up, s - down, a - left, d - right\nmake move: enter\n1 - show cells to move\n0 - show solution\n";
    VisibleCursor(TRUE);
    /*ColorPrint(1, 1, F_D_WHITE, "X");
    ColorPrint(1, 5, F_D_RED, "X");
    ColorPrint(5, 1, F_D_BLUE, "X");
    ColorPrint(5, 5, F_D_YELLOW, "X");*/
    board b(row, col);
    game g(b, { 0, 0 }, type);
    g.initboard();
    g.printboard();
    g.process_of_game();
    _getch();
    return 0;
}