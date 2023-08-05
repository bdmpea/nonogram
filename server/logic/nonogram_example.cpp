#include "nonogram.hpp"

int main() {
    int height = 8, width = 15;
    std::vector <std::vector <int>> rows(height);
    std::vector <std::vector <int>> columns(width);
    rows = { {13},
            {2, 1, 4},
            {1, 1, 3},
            {1, 1, 3},
            {2, 1, 3},
            {5, 3},
            {4},
            {3} };
    columns = { {4},
                {2, 2},
                {1, 1},
                {2, 2},
                {1, 2, 1},
                {1, 1},
                {1, 1},
                {1, 1},
                {1, 1},
                {1, 1},
                {1, 1},
                {2, 1, 1},
                {6, 1},
                {6},
                {4}
            };
    nonogram example(height, width, rows, columns);
    //example.solve();
}