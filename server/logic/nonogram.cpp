#include "nonogram.hpp"

std::ostream &operator<<(std::ostream &fout, line &out_line) {
    for (std::size_t ind = 1; ind <= out_line.s.size(); ind++) {
        if (out_line.s[ind] == UNKNOWN) {
            fout << ".";
        } else if (out_line.s[ind] == SCRATCHED) {
            fout << "X";
        } else {
            fout << "O";
        }
    }
    fout << std::endl;
    return fout;
}

bool line::sigma_match(int start, int end)  {
    bool ans = true;
    if (0 <= start && start <= s.size() - 1 &&
        0 <= end && end <= s.size() - 1) {
        ans = (s[start] == SCRATCHED);
        for (int ind = start + 1; ind <= end; ind++) {
            ans = (s[ind] == FILLED);
        }
    } else {
        ans = false;
    }
    return ans;
}

bool line::prefix_size_limitations_satisfied(std::size_t i, std::size_t j) {
    return (i < (line_rules.prefix_sum[j] + j));
}

bool line::is_fixable() {
    fix[0][0] = true;
    for (std::size_t i = 0; i <= s.size(); i++) {
        for (std::size_t j = 0; j <= line_rules.d.size(); j++) {
            if (i >= 1 && prefix_size_limitations_satisfied(i, j)) {
                if (s[i] == UNKNOWN || s[i] == SCRATCHED) {
                    fix0[i][j] = fix[i - 1][j];
                }
                if (line_rules.d[j] + 1 <= i && 1 <= j &&
                    sigma_match(i - line_rules.d[j], i)) {
                    fix1[i][j] = fix[i - line_rules.d[j] - 1][j - 1];
                }
            }
        }
    }
    return fix[s.size()][line_rules.d.size()];
}

std::vector<CELL_TYPE> line::merge_painting01(std::size_t i, std::size_t j) {
    std::vector<CELL_TYPE> ans(paint0[i][j].size(), UNKNOWN);
    for (std::size_t ind = 0; ind < ans.size(); ind++) {
        if (paint0[i][j][ind] == paint1[i][j][ind] && paint0[i][j][ind] != UNKNOWN) {
            ans[ind] = paint0[i][j][ind];
        }
    }
    return ans;
}

std::vector <std::size_t> line::solve_line() {
    if (!is_fixable()) {
        return {};
    }
    for (std::size_t i = 1; i <= s.size(); i++) {
        for (std::size_t j = 0; j <= line_rules.d.size(); j++) {
            if (fix[i][j]) {
                if (fix0[i][j]) {
                    paint0[i][j] = paint[i - 1][j];
                    paint0[i][j].push_back(SCRATCHED);
                }
                if (fix1[i][j]) {
                    paint1[i][j] = paint[i - line_rules.d[j] - 1][j - 1];
                    paint1[i][j].push_back(SCRATCHED);
                    for (std::size_t counter = 0; counter < line_rules.d[j]; counter++) {
                        paint1[i][j].push_back(FILLED);
                    }
                }

                if (fix0[i][j] && !fix1[i][j]) {
                    paint[i][j] = paint0[i][j];
                } else if (fix1[i][j] && !fix0[i][j]) {
                    paint[i][j] = paint1[i][j];
                } else {
                    paint[i][j] = merge_painting01(i, j);
                }
            }
        }
    }
    std::vector <std::size_t> ans;
    for (std::size_t ind = 0; ind < s.size(); ind++) {
        if (s[ind] != paint[s.size()][line_rules.d.size()][ind]) {
            ans.push_back(ind);
            s[ind] = paint[s.size()][line_rules.d.size()][ind];
        }
    }
    return ans;
}

void nonogram::change_pixel_state(int x, int y, CELL_TYPE type) {
    if (!(0 <= x && x < height) || !(0 <= y && y <= width) || rows_grid[x].s[y] == type) {
        return;
    }
    rows_grid[x].s[y] = type;
    columns_grid[y].s[x] = type;
    unpainted_pixels[x].erase(y);
}

std::vector <std::vector <CELL_TYPE>> nonogram::get_grid() {
    std::vector <std::vector <CELL_TYPE>> answer(height, std::vector <CELL_TYPE> (width));
    for (std::size_t x = 0; x < height; x++) {
        for (std::size_t y = 0; y < width; y++) {
            answer[x][y] = rows_grid[x].s[y];
        }
    }
    return answer;
}

std::vector <std::pair<int, int>> nonogram::propagate() {
    std::vector <std::pair<int, int>> updated_pixels;
    std::queue <std::pair<line, int>> queue_of_lines;
    std::set <std::pair <int, int>> set_of_lines;
    for (std::size_t ind = 0; ind < rows_grid.size(); ind++) {
        queue_of_lines.emplace(rows_grid[ind], ind);
        set_of_lines.insert({0, ind});
    }
    for (std::size_t ind = 0; ind < columns_grid.size(); ind++) {
        queue_of_lines.emplace(columns_grid[ind], ind);
        set_of_lines.insert({1, ind});
    }
    while (!queue_of_lines.empty()) {
        line next = queue_of_lines.front().first;
        int line_number = queue_of_lines.front().second;
        set_of_lines.erase({next.line_rules.type, line_number});
        queue_of_lines.pop();
        if (!next.is_fixable()) {
            state = CONFLICT;
            return updated_pixels;
        }
        std::vector <std::size_t> new_pixel_indexes = next.solve_line();
        if (!new_pixel_indexes.empty()) {
            for (const auto& ind: new_pixel_indexes) {
                if (next.line_rules.type == 0) {
                    if (set_of_lines.count({1, ind}) == 0) {
                        queue_of_lines.emplace(columns_grid[ind], ind);
                        set_of_lines.insert({1, ind});
                    }
                    updated_pixels.emplace_back(line_number, ind);
                } else {
                    if (set_of_lines.count({0, ind}) == 0) {
                        queue_of_lines.emplace(rows_grid[ind], ind);
                        set_of_lines.insert({0, ind});
                    }
                    updated_pixels.emplace_back(ind, line_number);
                }
            }
        }
    }
    for (const auto& row: unpainted_pixels) {
        if (!row.empty()) {
            state = INCOMPLETE;
            return updated_pixels;
        }
    }
    state = SOLVED;
    return updated_pixels;
}

//todo: smarter choose of new pixel
std::pair <int, int> nonogram::choose_next_pixel() {
    for (std::size_t x = 0; x < height; x++) {
        if (!unpainted_pixels[x].empty()) {
            return {x, *unpainted_pixels[x].begin()};
        }
    }
}

std::ostream &operator<<(std::ostream &fout, nonogram &other) {
    for (std::size_t x = 0; x < other.height; x++) {
        fout << other.rows_grid[x];
    }
    fout << std::endl;
    return fout;
}

void nonogram::backtracking() {
    fp();
    if (state == CONFLICT) {
        return;
    }
    if (state == SOLVED) {
        solutions.insert(get_grid());
    }
    auto [x, y] = choose_next_pixel();
    *this = pixel_grids[x][y].first;
    backtracking();
    *this = pixel_grids[x][y].second;
    backtracking();
}


std::string nonogram::solve() {
    backtracking();
    if (state == CONFLICT || solutions.empty()) {
        return "Nonogram has no solution";
    }
    if (solutions.size() > 1) {
        return "Nonogram has more than one solution";
    }
    return "Nonogram is correct";
}

void nonogram::probe(int x, int y) {
    std::vector <std::pair<int, int>> updated_pixels;
    std::vector <std::pair <int, int>> scratched_updated_pixels = pixel_grids[x][y].first.propagate();
    std::vector <std::pair <int, int>> filled_updated_pixels = pixel_grids[x][y].second.propagate();
    if (pixel_grids[x][y].first.state == CONFLICT &&
        pixel_grids[x][y].second.state == CONFLICT) {
        state = CONFLICT;
        return;
    }
    if (pixel_grids[x][y].first.state == CONFLICT) {
        updated_pixels = filled_updated_pixels;
    } else if (pixel_grids[x][y].second.state == CONFLICT) {
        updated_pixels = scratched_updated_pixels;
    } else {
        std::sort(scratched_updated_pixels.begin(), scratched_updated_pixels.end());
        std::sort(filled_updated_pixels.begin(), filled_updated_pixels.end());
        std::set_intersection(scratched_updated_pixels.begin(), scratched_updated_pixels.end(),
                              filled_updated_pixels.begin(), filled_updated_pixels.end(),
                              back_inserter(updated_pixels));
    }
    if (!updated_pixels.empty()) {
        update_pixel_grids(updated_pixels);
        state = PAINTED;
    } else {
        state = INCOMPLETE;
    }
}

void nonogram::fp() {
    std::vector <std::pair<int, int>> updated_pixels;
    do {
        std::vector <std::pair <int, int>> updated_pixels = propagate();
        if (state == CONFLICT || state == SOLVED) { return; }
        update_pixel_grids(updated_pixels);
        for (std::size_t x = 0; x < height; x++) {
            for (const auto &y: unpainted_pixels[x]) {
                probe(x, y);
                if (state == CONFLICT || state == SOLVED) { return; }
                if (state == PAINTED) { break; }
            }
        }
    } while (!updated_pixels.empty());
}

void nonogram::update_pixel_grids(std::vector <std::pair<int, int>>& updated_pixels) {
    for (const auto& [x_to_update, y_to_update]: updated_pixels) {
        for (std::size_t x = 0; x < height; x++) {
            for (const auto &y: unpainted_pixels[x]) {
                pixel_grids[x][y].first.change_pixel_state(x_to_update, y_to_update, rows_grid[x_to_update].s[y_to_update]);
            }
        }
    }
}