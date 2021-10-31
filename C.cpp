#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <queue>

#define BIG_NUM 1000000000

struct field {
    int x;
    int y;
};

class Graph {
protected:
    uint64_t vertex_degree = 0;

public:
    [[nodiscard]] uint64_t getVertexDegree() const {
        return vertex_degree;
    }

    [[nodiscard]] virtual std::vector<uint64_t> getNeighbors(const uint64_t& v) const = 0;

    virtual void addEdge(const uint64_t& source, const uint64_t& dest) = 0;

    virtual void setFieldValue(const uint64_t& value, const int& x, const int& y) = 0;

    [[nodiscard]] virtual uint64_t getValue(const int& x, const int& y) const = 0;
};

class GraphMatrix final : public Graph {
private:
    std::vector<std::vector<uint64_t>> matrix;

public:
    explicit GraphMatrix(const uint64_t& vertex_number) {
        for (uint64_t i = 0; i < vertex_number + 1; i++) {
            std::vector<uint64_t> tmp(vertex_number + 1, BIG_NUM);
            matrix.push_back(tmp);
        }
        vertex_degree = vertex_number;
    }

    [[nodiscard]] std::vector<uint64_t> getNeighbors(const uint64_t& v) const override {
        std::vector<uint64_t> answer;
        for (uint64_t i = 0; i < getVertexDegree(); i++) {
            if (matrix[v][i] != 0) {
                answer.push_back(i);
            }
        }
        return answer;
    }

    void addEdge(const uint64_t& source, const uint64_t& dest) override {
        if (source == dest) {
            return;
        }
        matrix[source][dest] = 1;
        matrix[dest][source] = 1;
    }

    void setFieldValue(const uint64_t& value, const int& x, const int& y) override {
        matrix[x][y] = value;
    }

    [[nodiscard]] uint64_t getValue(const int& x, const int& y) const override {
        return matrix[x][y];
    }
};

std::vector<field> shortestPath(Graph& graph, int start_x, int start_y, int finish_x, int finish_y) {
    std::queue<field> queue;
    std::vector<std::vector<field>> parent(graph.getVertexDegree() + 1, std::vector<field>(graph.getVertexDegree() + 1, {INT32_MAX, INT32_MAX}));


    int possible_moves_x[8] = {-2, -2, -1, -1, 1, 1, 2, 2};
    int possible_moves_y[8] = {-1, 1, -2, 2, -2, 2, -1, 1};

    graph.setFieldValue(0, start_x, start_y);
    std::vector<field> answer;

    queue.push(field{start_x, start_y});
    while (!queue.empty()) {
        int current_x = queue.front().x;
        int current_y = queue.front().y;
        queue.pop();
        for (int step = 0; step < 8; step++) {
            int next_x = current_x + possible_moves_x[step];
            int next_y = current_y + possible_moves_y[step];
            if (next_x > 0 && next_x <= graph.getVertexDegree() && next_y > 0 && next_y <= graph.getVertexDegree() && graph.getValue(next_x, next_y) > graph.getValue(current_x, current_y) + 1) {
                answer.push_back(field{next_x, next_y});
                graph.setFieldValue(graph.getValue(current_x, current_y) + 1, next_x, next_y);
                parent[next_x][next_y] = {current_x, current_y};
                queue.push(field{next_x, next_y});
            }
        }
    }

    std::cout << graph.getValue(finish_x, finish_y) << "\n";
    field res {finish_x, finish_y};
    while (!(res.x == INT32_MAX && res.y == INT32_MAX)) {
        answer.push_back(res);
        res = parent[res.x][res.y];
    }
    return answer;
}

int main() {
    int vertex, start_x, start_y, finish_x, finish_y;

    std::cin >> vertex >> start_x >> start_y >> finish_x >> finish_y;

    GraphMatrix graph(vertex);

    std::vector<field> answer = shortestPath(graph, start_x, start_y, finish_x, finish_y);

    for (auto c = 0; c < graph.getValue(finish_x, finish_y) + 1; c++) {
        std::cout << answer[answer.size() - c - 1].x << " " << answer[answer.size() - c - 1].y << "\n";
    }

    return 0;
}