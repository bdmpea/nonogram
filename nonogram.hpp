#ifndef NONOGRAM_H
#define NONOGRAM_H

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include "common_vars.hpp"

struct description {
    std::vector <int> d;
    std::vector <int> prefix_sum;

    description(std::vector <int> &array) {
        d.resize(array.size() + 1);
        prefix_sum.resize(array.size() + 1);
        d[0] = 0;
        prefix_sum[0] = 0;
        for (std::size_t ind = 0; ind < array.size(); ind++) {
            d[ind + 1] = array[ind];
            prefix_sum[ind + 1] = prefix_sum[ind] + d[ind + 1];
        }
    }

    friend bool operator==(const description& lhs, const description& rhs) {
        return (lhs.d == rhs.d);
    }

};

struct line {
    int type;
    std::vector<CELL_TYPE> s;
    description line_rules;

    line(std::size_t line_len, std::vector<int> &array, int t = 0) : line_rules(array), type(t) {
        s.resize(line_len + 2, UNKNOWN);
        number_of_unknowns = line_len;
        s[0] = SCRATCHED;
        s[1] = SCRATCHED;
        fix.resize(s.size(), std::vector<int>(line_rules.d.size(), false));
        fix0.resize(s.size(), std::vector<int>(line_rules.d.size(), false));
        fix1.resize(s.size(), std::vector<int>(line_rules.d.size(), false));
        paint.resize(s.size(), std::vector<std::vector<CELL_TYPE>>(line_rules.d.size(), {SCRATCHED}));
        paint0.resize(s.size(), std::vector<std::vector<CELL_TYPE>>(line_rules.d.size(), {SCRATCHED}));
        paint1.resize(s.size(), std::vector<std::vector<CELL_TYPE>>(line_rules.d.size(), {SCRATCHED}));
    }

    friend std::ostream &operator<<(std::ostream &fout, line &out_line);
    bool is_fixable();
    std::vector <int> solve();
    friend bool operator==(const line& lhs, const line& rhs) {
        return (lhs.s == rhs.s && lhs.line_rules == rhs.line_rules);
    }
private:
    int number_of_unknowns;
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

    nonogram() {}

    std::string convert_field_into_str();

    nonogram(const nonogram& other) : width(other.width), height(other.height), state(other.state) {
        unpainted_pixels = other.unpainted_pixels;
        rows_grid = other.rows_grid;
        columns_grid = other.columns_grid;
        pixel_grids = other.pixel_grids;
    }
    nonogram(std::vector <std::vector <int>>& rows, std::vector <std::vector <int>>& columns) :
        width(columns.size()), height(rows.size()), state(INCOMPLETE) {
        for (std::size_t x = 0; x < width; x++) {
            columns_grid.emplace_back(height, columns[x], 1);
        }
        for (std::size_t y = 0; y < height; y++) {
            rows_grid.emplace_back(width, rows[y], 0);
        }
        unpainted_pixels.resize(height);
        //pixel_grids.resize(height);
        for (std::size_t y = 0; y < height; y++) {
            for (std::size_t x = 0; x < width; x++) {
                unpainted_pixels[y].insert(x);
                //pixel_grids[y].emplace_back(nonogram(*this), nonogram(*this));
                //pixel_grids[y][x].first.change_pixel_state(y, x, SCRATCHED);
                //pixel_grids[y][x].second.change_pixel_state(y, x, FILLED);
            }
        }
    }

    std::vector <std::vector <CELL_TYPE>> get_grid();
    //std::string solve();
    friend std::ostream &operator<<(std::ostream &fout, nonogram &other);

    void change_pixel_state(int y, int x, CELL_TYPE type);
    std::vector <std::pair<int, int>> propagate();
    std::vector<std::vector<int>> get_xAxisClue();
    std::vector<std::vector<int>> get_yAxisClue();
    std::string convert_xAxis_into_str();
    std::string convert_yAxis_into_str();
    std::vector <std::set <int>> unpainted_pixels;
    std::vector <std::vector<std::pair <nonogram, nonogram>>> pixel_grids;
    std::set <std::vector <std::vector <CELL_TYPE>>> solutions;

    void generate_random(int height, int width) {

    }


    //void update_pixel_grids(std::vector <std::pair<int, int>>& updated_pixels);
    //void fp();
    //void probe(int x, int y);
    //todo: smarter choose of new pixel
    //std::pair <int, int> choose_next_pixel();
    //void backtracking();

};


#endif // NONOGRAM_H
