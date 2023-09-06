#include "nonogram.hpp"

std::ostream &operator<<(std::ostream &fout, line &out_line) {
    for (std::size_t ind = 2; ind < out_line.s.size(); ind++) {
        if (out_line.s[ind] == UNKNOWN) {
            fout << "_";
        } else if (out_line.s[ind] == SCRATCHED) {
            fout << " ";
        } else {
            fout << "O";
        }
    }
    fout << std::endl;
    return fout;
}

bool line::sigma_match(int start, int end)  {
    if (1 <= start && start <= s.size() - 1 &&
        1 <= end && end <= s.size() - 1) {
        if (s[start] == FILLED) {
            return false;
        }
        for (int ind = start + 1; ind <= end; ind++) {
            if (s[ind] == SCRATCHED) {
                return false;
            }
        }
    } else {
        return false;
    }
    return true;
}

bool line::prefix_size_limitations_satisfied(std::size_t i, std::size_t j) {
    return (i >= (line_rules.prefix_sum[j] + j));
}

bool line::is_fixable() {
    fix[0][0] = true;
    for (std::size_t i = 1; i < s.size(); i++) {
        for (std::size_t j = 0; j < line_rules.d.size(); j++) {
            if (prefix_size_limitations_satisfied(i, j)) {
                if (s[i] == UNKNOWN || s[i] == SCRATCHED) {
                    fix0[i][j] = fix[i - 1][j];
                }
                if (1 <= j &&
                    sigma_match(i - line_rules.d[j], i)) {
                    fix1[i][j] = fix[i - line_rules.d[j] - 1][j - 1];
                }
                fix[i][j] = fix0[i][j] || fix1[i][j];
            }
        }
    }
    return fix[s.size() - 1][line_rules.d.size() - 1];
}


std::vector<CELL_TYPE> line::merge_painting01(std::size_t i, std::size_t j) {
    std::vector<CELL_TYPE> ans(i + 1, UNKNOWN);
    for (std::size_t ind = 0; ind <= i; ind++) {
        if (paint0[i][j][ind] == paint1[i][j][ind]) {
            ans[ind] = paint0[i][j][ind];
        }
    }
    return ans;
}

std::vector <int> line::solve() {
    if (!is_fixable() || !number_of_unknowns) {
        return {};
    }
    for (std::size_t i = 1; i < s.size(); i++) {
        for (std::size_t j = 0; j < line_rules.d.size(); j++) {
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
    std::vector <int> painted_pixels;
    for (std::size_t ind = 2; ind <= s.size() - 1; ind++) {
        if (s[ind] == UNKNOWN && paint[s.size() - 1][line_rules.d.size() - 1][ind] != UNKNOWN) {
            s[ind] = paint[s.size() - 1][line_rules.d.size() - 1][ind];

            painted_pixels.push_back(ind - 2);
            number_of_unknowns--;
        }
    }
    return painted_pixels;
}

void nonogram::change_pixel_state(int y, int x, CELL_TYPE type) {
    if (!(0 <= y && y < height) || !(0 <= x + 2 && x + 2 < width + 2)) {
        return;
    }
    unpainted_pixels[y].erase(x);
    rows_grid[y].s[x + 2] = type;
    columns_grid[x].s[y + 2] = type;
}


std::vector <std::vector <CELL_TYPE>> nonogram::get_grid() {
    std::vector <std::vector <CELL_TYPE>> answer(height, std::vector <CELL_TYPE> (width));
    for (std::size_t y = 0; y < height; y++) {
        for (std::size_t x = 0; x < width; x++) {
            answer[y][x] = rows_grid[y].s[x + 2];
        }
    }
    return answer;
}

std::ostream &operator<<(std::ostream &fout, nonogram &other) {
    for (int y = 0; y < other.height; y++) {
        fout << other.rows_grid[y];
    }
    fout << std::endl;
    return fout;
}

std::vector <std::pair<int, int>> nonogram::propagate() {
    std::vector <std::pair<int, int>> updated_pixels;
    std::queue <std::pair<int, int>> queue_of_lines;
    std::set <std::pair <int, int>> set_of_lines;
    for (std::size_t ind = 0; ind < rows_grid.size(); ind++) {
        queue_of_lines.emplace(0, ind);
        set_of_lines.insert({0, ind});
    }
    for (std::size_t ind = 0; ind < columns_grid.size(); ind++) {
        queue_of_lines.emplace(1, ind);
        set_of_lines.insert({1, ind});
    }
    while (!queue_of_lines.empty()) {
        int type = queue_of_lines.front().first;
        int line_number = queue_of_lines.front().second;
        line next = type == 0 ? rows_grid[line_number] : columns_grid[line_number];
        set_of_lines.erase({type, line_number});
        queue_of_lines.pop();
        if (!next.is_fixable()) {
            state = CONFLICT;
            return updated_pixels;
        }
        std::vector <int> new_pixel_indexes = next.solve();
        if (!new_pixel_indexes.empty()) {
            for (const auto& ind: new_pixel_indexes) {
                if (type == 0) {
                    change_pixel_state(line_number, ind, next.s[ind + 2]);
                    if (set_of_lines.count({1, ind}) == 0) {
                        queue_of_lines.emplace(1, ind);
                        set_of_lines.insert({1, ind});
                    }
                    updated_pixels.emplace_back(line_number, ind);
                } else {
                    change_pixel_state(ind, line_number, next.s[ind + 2]);
                    if (set_of_lines.count({0, ind}) == 0) {
                        queue_of_lines.emplace(0, ind);
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

std::vector<std::vector<int>> nonogram::get_xAxisClue() {
    std::vector<std::vector<int>> answer(width);
    int num = 0;
    for (const auto& column: columns_grid) {
        for (int i = 1; i < column.line_rules.d.size(); i++) {
            answer[num].push_back(column.line_rules.d[i]);
        }
        num++;
    }
    return answer;
}

std::string nonogram::convert_xAxis_into_str() {
    std::string answer;
    for (const auto& column: columns_grid) {
        answer += std::to_string(column.line_rules.d.size() - 1) + " ";
        for (int i = 1; i < column.line_rules.d.size(); i++) {
            answer += std::to_string(column.line_rules.d[i]) + " ";
        }
    }
    return answer;
}

std::string nonogram::convert_yAxis_into_str() {
    std::string answer;
    for (const auto& row: rows_grid) {
        answer += std::to_string(row.line_rules.d.size() - 1) + " ";
        for (int i = 1; i < row.line_rules.d.size(); i++) {
            answer += std::to_string(row.line_rules.d[i]) + " ";
        }
    }
    return answer;
}

std::vector<std::vector<int>> nonogram::get_yAxisClue() {
    std::vector<std::vector<int>> answer(height);
    int num = 0;
    for (const auto& row: rows_grid) {
        for (int i = 1; i < row.line_rules.d.size(); i++) {
            answer[num].push_back(row.line_rules.d[i]);
        }
        num++;
    }
    return answer;
}



std::string nonogram::convert_field_into_str() {
    std::string g = std::to_string(height) + " " + std::to_string(width) + " ";
    for (std::size_t y = 0; y < height; y++) {
        std::string row = "";
        for (std::size_t x = 0; x < width; x++) {
            row +=  rows_grid[y].s[x + 2] == UNKNOWN ? '0' : (rows_grid[y].s[x + 2] == FILLED ? '1' : '2');
        }
        g += row;
    }
    return g;
}


/*


void nonogram::update_pixel_grids(std::vector <std::pair<int, int>>& updated_pixels) {
    for (const auto& [x_to_update, y_to_update]: updated_pixels) {
        for (std::size_t x = 0; x < height; x++) {
            for (const auto &y: unpainted_pixels[x]) {
                pixel_grids[x][y].first.change_pixel_state(x_to_update, y_to_update, rows_grid[x_to_update].s[y_to_update]);
            }
        }
    }
}

void nonogram::fp() {
    std::vector <std::pair<int, int>> updated_pixels = propagate();
    do {
        if (state == CONFLICT || state == SOLVED) { return; }
        update_pixel_grids(updated_pixels);
        for (std::size_t x = 0; x < height; x++) {
            for (const auto &y: unpainted_pixels[x]) {
                probe(x, y);
                if (state == CONFLICT || state == SOLVED) { return; }
                if (state == PAINTED) { break; }
            }
        }
        updated_pixels = propagate();
    } while (!updated_pixels.empty());
}

void nonogram::probe(int y, int x) {
    std::vector <std::pair<int, int>> updated_pixels;
    std::vector <std::pair <int, int>> scratched_updated_pixels = pixel_grids[y][x].first.propagate();
    std::vector <std::pair <int, int>> filled_updated_pixels = pixel_grids[y][x].second.propagate();
    if (pixel_grids[y][x].first.state == CONFLICT &&
        pixel_grids[y][x].second.state == CONFLICT) {
        state = CONFLICT;
        return;
    }
    if (pixel_grids[y][x].first.state == CONFLICT) {
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


//todo: smarter choose of new pixel
std::pair <int, int> nonogram::choose_next_pixel() {
    for (std::size_t x = 0; x < height; x++) {
        if (!unpainted_pixels[x].empty()) {
            return {x, *unpainted_pixels[x].begin()};
        }
    }
    return {0, 0};
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


*/
