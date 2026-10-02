// TheGameOfLife.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include <cstdlib>

char** createField(int rows, int cols);
void fillField(char** field, int rows, int cols, std::ifstream* file);
void copyField(char** fieldDst, char** fieldSrc, int rows, int cols);
void stepField(char** fieldNext, char** fieldNow, int rows, int cols);
bool cmpField(char** fieldNext, char** fieldNow, int rows, int cols);
void printField(char** arr, int rows, int cols, int generation, int* aliveCells);
void deleteTwoDimArray(char** arr, int rows, int cols);

int main()
{
    setlocale(LC_ALL, "RUS");
    SetConsoleCP(1251);

    std::ifstream file("..\\playing_field.txt");
    std::string str{};

    int rows{ 0 }, cols{ 0 }, aliveCells{ 0 }, count{ 1 };

    char** field{ NULL };
    char** fieldNext{ NULL };

    if (file.is_open())
    {
        file >> str;
        rows = std::stoi(str);

        file >> str;
        cols = std::stoi(str);

        field = createField(rows, cols);
        fieldNext = createField(rows, cols);

        fillField(field, rows, cols, &file);
    }

    file.close();

    while (true)
    {
        printField(field, rows, cols, count, &aliveCells);
        if (aliveCells == 0)
        {
            count++;
            printField(field, rows, cols, count, &aliveCells);
            break;
        }
        stepField(fieldNext, field, rows, cols);
        if (cmpField(fieldNext, field, rows, cols))
        {
            count++;
            printField(field, rows, cols, count, &aliveCells);
            break;
        }
        copyField(field, fieldNext, rows, cols);
        count++;

        Sleep(500);
    }

    if (aliveCells == 0)
    {
        std::cout << "Все клетки мертвы. Игра окончена" << std::endl;
    }
    else
    {
        std::cout << "Мир статичен. Игра окончена" << std::endl;
    }

    //copyField(fieldNext, field, rows, cols);

    deleteTwoDimArray(field, rows, cols);
    deleteTwoDimArray(fieldNext, rows, cols);
}

char** createField(int rows, int cols)
{
    char** array = new char* [rows];

    for (int i = 0; i < rows; i++)
    {
        array[i] = new char[cols] {};
        for (int j = 0; j < cols; j++)
        {
            array[i][j] = '-';
        }
    }

    return array;
}

void fillField(char** field, int rows, int cols, std::ifstream* file)
{
    std::string strRow{}, strCol{};
    int row{ 0 }, col{ 0 };

    while (*file >> strRow >> strCol)
    {
        row = std::stoi(strRow);
        col = std::stoi(strCol);
        if ((row >= 0) && (col >= 0) && (row < rows) && (col < cols))
        {
            field[row][col] = '*';
        }
    }
}

void copyField(char** fieldDst, char** fieldSrc, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        std::copy(fieldSrc[i], fieldSrc[i] + cols, fieldDst[i]);
    }
}

void stepField(char** fieldNext, char** fieldNow, int rows, int cols)
{
    const int drow[8] = { -1, -1, -1,  0, 0,  1, 1, 1 };
    const int dcol[8] = { -1,  0,  1, -1, 1, -1, 0, 1 };

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            int count = 0;

            for (int i = 0; i < 8; i++)
            {
                int tempRow = row + drow[i];
                int tempCol = col + dcol[i];

                if ((tempRow >= 0) && (tempRow < rows) && (tempCol >= 0) && (tempCol < cols))
                {
                    if (fieldNow[tempRow][tempCol] == '*')
                    {
                        count++;
                    }
                }
            }

            switch (fieldNow[row][col])
            {
            case '*':
                if (count == 2 || count == 3)
                {
                    fieldNext[row][col] = '*';
                }
                else
                {
                    fieldNext[row][col] = '-';
                }
                break;
            case '-':
                if (count == 3)
                {
                    fieldNext[row][col] = '*';
                }
                else
                {
                    fieldNext[row][col] = '-';
                }
                break;
            default:
                break;
            }
        }
    }
}

bool cmpField(char** fieldNext, char** fieldNow, int rows, int cols)
{
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            if (fieldNext[row][col] != fieldNow[row][col])
            {
                return false;
            }
        }
    }

    return true;
}

void printField(char** field, int rows, int cols, int generation, int* aliveCells)
{
    int alive{ 0 };

    std::system("cls");

    for (int i = 0; i < rows; i++)
    {
        for (int j = cols - 1; j >= 0; j--)
        {
            std::cout << field[i][cols - 1 - j] << ' ';
            if (field[i][cols - 1 - j] == '*')
            {
                alive++;
            }
        }

        std::cout << std::endl;
    }
    std::cout << "Генерация: " << generation << " Живых ячеек: " << alive << std::endl;

    *aliveCells = alive;
}

void deleteTwoDimArray(char** field, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        delete[] field[i];
    }

    delete[] field;
}
