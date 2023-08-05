#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <queue>
#include <algorithm>

enum CELL_TYPE {
    UNKNOWN, SCRATCHED, FILLED
};

enum NONOGRAM_STATE {
    INCOMPLETE, SOLVED, CONFLICT, PAINTED
};


struct description {
    int type;
    std::vector <int> d;
    std::vector <int> prefix_sum;
    description(int t, std::vector <int> &array) : type(t) {
        d.resize(array.size() + 1);
        prefix_sum.resize(array.size() + 1);
        d[0] = 0;
        prefix_sum[0] = 0;
        for (std::size_t ind = 0; ind < array.size(); ind++) {
            d[ind + 1] = array[ind];
            prefix_sum[ind + 1] = prefix_sum[ind] + d[ind + 1];
        }
    }
};



struct line {
    std::vector<CELL_TYPE> s;
    description line_rules;

    line(int t, std::vector<int> &array) : line_rules(t, array) {
        s.resize(array.size() + 1, UNKNOWN);
        s[0] = SCRATCHED;
        fix.resize(s.size() + 1, std::vector<int>(line_rules.d.size() + 1, false));
        fix0.resize(s.size() + 1, std::vector<int>(line_rules.d.size() + 1, false));
        fix1.resize(s.size() + 1, std::vector<int>(line_rules.d.size() + 1, false));
        paint.resize(s.size() + 1, std::vector<std::vector<CELL_TYPE>>(line_rules.d.size() + 1));
        paint0.resize(s.size() + 1, std::vector<std::vector<CELL_TYPE>>(line_rules.d.size() + 1));
        paint1.resize(s.size() + 1, std::vector<std::vector<CELL_TYPE>>(line_rules.d.size() + 1));
    }

    friend std::ostream &operator<<(std::ostream &fout, line &out_line);
    bool is_fixable();
    std::vector<std::size_t> solve_line();

private:
    std::vector<std::vector<int>> fix;
    std::vector<std::vector<int>> fix0;
    std::vector<std::vector<int>> fix1;
    std::vector<std::vector<std::vector<CELL_TYPE>>> paint;
    std::vector<std::vector<std::vector<CELL_TYPE>>> paint0;
    std::vector<std::vector<std::vector<CELL_TYPE>>> paint1;

    bool sigma_match(int start, int end);
    bool prefix_size_limitations_satisfied(std::size_t i, std::size_t j);
    std::vector<CELL_TYPE> merge_painting01(std::size_t i, std::size_t j);
};


struct nonogram {
    std::size_t width, height;
    NONOGRAM_STATE state;
    std::vector <line> rows_grid;
    std::vector <line> columns_grid;

    nonogram(const nonogram& other) : width(other.width), height(other.height), state(other.state) {
        unpainted_pixels = other.unpainted_pixels;
        rows_grid = other.rows_grid;
        columns_grid = other.columns_grid;
        pixel_grids = other.pixel_grids;
    }
    nonogram(std::size_t w, std::size_t h, std::vector <std::vector <int>>& rows, std::vector <std::vector <int>>& columns) :
            width(w), height(h), state(INCOMPLETE) {
        for (std::size_t ind = 0; ind < height; ind++) {
            rows_grid.push_back(line(0, rows[ind]));
        }
        for (std::size_t ind = 0; ind < width; ind++) {
            columns_grid.push_back(line(1, columns[ind]));
        }
        unpainted_pixels.resize(height);
        pixel_grids.resize(height);
        for (std::size_t x = 0; x < height; x++) {
            for (std::size_t y = 0; y < width; y++) {
                unpainted_pixels[x].insert(y);
                pixel_grids[x].emplace_back(nonogram(*this), nonogram(*this));
                pixel_grids[x][y].first.change_pixel_state(x, y, SCRATCHED);
                pixel_grids[x][y].second.change_pixel_state(x, y, FILLED);
            }
        }
    }

    std::vector <std::vector <CELL_TYPE>> get_grid();
    std::string solve();
    friend std::ostream &operator<<(std::ostream &fout, nonogram &other);
private:
    std::vector <std::set <int>> unpainted_pixels;
    std::vector <std::vector<std::pair <nonogram, nonogram>>> pixel_grids;
    std::set <std::vector <std::vector <CELL_TYPE>>> solutions;

    void update_pixel_grids(std::vector <std::pair<int, int>>& updated_pixels);
    void fp();
    void probe(int x, int y);
    //todo: smarter choose of new pixel
    std::pair <int, int> choose_next_pixel();
    void backtracking();
    void change_pixel_state(int x, int y, CELL_TYPE type);
    std::vector <std::pair<int, int>> propagate();
};