#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <queue>

const int64_t kUndefinedDist = 1000 * 1000 * 1000 * 1LL;
using VertexT = int32_t;
using SquareT = int32_t;

enum { ORIENTED = true, NOT_ORIENTED = false };

struct Square {
    VertexT x;
    VertexT y;
};

class IGraph {
protected:
    VertexT vertex_num_ = 0;
    bool oriented_;

public:
    [[nodiscard]] VertexT GetVertexNum() const {
        return vertex_num_;
    }

    [[nodiscard]] virtual std::vector<VertexT> GetNeighbors(const VertexT& current_vertex) const = 0;

    virtual void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to) = 0;
};

class GraphList final : public IGraph {
private:
    std::vector<std::vector<VertexT>> adjacency_list_;

public:
    explicit GraphList(const VertexT& vertex_number, bool oriented_status) {
        vertex_num_ = vertex_number;
        adjacency_list_.resize(vertex_number);
        for (VertexT i = 0; i < vertex_number; i++) {
            adjacency_list_[i] = std::vector<VertexT>();
        }
        oriented_ = oriented_status;
    }

    [[nodiscard]] std::vector<VertexT> GetNeighbors(const VertexT& v) const override {
        return adjacency_list_.at(v);
    }

    void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to) override {
        adjacency_list_[vertex_from].push_back(vertex_to);
        if (!oriented_) {
            adjacency_list_[vertex_to].push_back(vertex_from);
        }
    }
};

class GraphMatrix final : public IGraph {
private:
    std::vector<std::vector<VertexT>> matrix_;

public:
    explicit GraphMatrix(const VertexT& quantity) {
        for (VertexT i = 0; i < quantity; ++i) {
            std::vector<VertexT> tmp(quantity, 0);
            matrix_.push_back(tmp);
        }
        vertex_num_ = quantity;
    }

    [[nodiscard]] std::vector<VertexT> GetNeighbors(const VertexT& current_vertex) const override {
        std::vector<VertexT> answer;
        for (VertexT i = 0; i < GetVertexNum(); ++i) {
            if (matrix_[current_vertex][i] != 0) {
                answer.push_back(i);
            }
        }
        return answer;
    }

    void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to) override {
        if (vertex_from == vertex_to) {
            return;
        }
        matrix_[vertex_from][vertex_to] = 1;
        if (!oriented_) {
            matrix_[vertex_to][vertex_from] = 1;
        }
    }
};

struct Path {
    std::vector<VertexT> dist;
    std::vector<VertexT> predecessor;

    explicit Path(VertexT size) {
        predecessor.resize(size, kUndefinedDist);
        dist.resize(size, kUndefinedDist);
    }
};

void BuildShortestPath(const IGraph& graph, const VertexT& source, Path& path) {
    std::queue<VertexT> queue;

    path.dist[source] = 0;
    queue.push(source);

    while (!queue.empty()) {
        VertexT current_vertex = queue.front();
        queue.pop();
        std::vector<VertexT> neighbors = graph.GetNeighbors(current_vertex);

        for (auto& neighbor : neighbors) {
            if (path.dist[neighbor] == kUndefinedDist) {
                path.dist[neighbor] = path.dist[current_vertex] + 1;
                path.predecessor[neighbor] = current_vertex;
                queue.push(neighbor);
            }
        }
    }
}

std::vector<VertexT> RestorePath(const VertexT destination, Path& path) {
    std::vector<VertexT> shortest_path;
    if (path.dist[destination] == kUndefinedDist) {
        return shortest_path;
    }

    VertexT vertex = destination;
    while (vertex != kUndefinedDist) {
        shortest_path.push_back(vertex);
        vertex = path.predecessor[vertex];
    }
    std::reverse(shortest_path.begin(), shortest_path.end());

    return shortest_path;
}

void PrintShortestPath(const std::vector<VertexT>& path_list, VertexT size_of_field) {
    if (path_list.empty()) {
        std::cout << -1 << "\n";
    } else {
        std::cout << path_list.size() - 1 << "\n";
        for (auto& vertex : path_list) {
            auto vertex_x = vertex / size_of_field;
            auto vertex_y = vertex % size_of_field;
            std::cout << vertex_x + 1 << " " << vertex_y + 1 << "\n";
        }
    }
}

std::vector<VertexT> ShortestPath(const IGraph& graph, const VertexT start, const VertexT finish) {
    Path path(graph.GetVertexNum());

    BuildShortestPath(graph, start, path);
    auto restored_path = RestorePath(finish, path);
    return restored_path;
}

class Grid {
    VertexT size_of_field_;
    bool inside_field_;

public:
    explicit Grid(VertexT size) {
        size_of_field_ = size;
        inside_field_ = false;
    }

    VertexT ConvertXYPositionToLinear(VertexT pos_x, VertexT pos_y) const {
        return pos_x * size_of_field_ + pos_y;
    }

    bool CheckIfInsideField(VertexT pos_x, VertexT pos_y) const {
        if (pos_x < 0 || pos_x >= size_of_field_ || pos_y < 0 || pos_y >= size_of_field_) {
            return false;
        }
        return true;
    }

    VertexT GetSize() {
        return size_of_field_;
    }
};

struct Move {
    std::vector<Square> possible_moves = {{-2, -1}, {-2, 1}, {-1, 2}, {1, 2}};
};

void SetNeighbors(IGraph& graph, Grid& grid) {
    Move move;

    for (VertexT pos_x = 0; pos_x < grid.GetSize(); pos_x++) {
        for (VertexT pos_y = 0; pos_y < grid.GetSize(); pos_y++) {
            for (size_t step = 0; step < move.possible_moves.size(); step++) {
                Square potential_move = move.possible_moves[step];
                if (grid.CheckIfInsideField(pos_x + potential_move.x, pos_y + potential_move.y)) {
                    VertexT vertex_from = grid.ConvertXYPositionToLinear(pos_x, pos_y);
                    VertexT vertex_to =
                            grid.ConvertXYPositionToLinear(pos_x + potential_move.x, pos_y + potential_move.y);

                    graph.AddEdge(vertex_from, vertex_to);
                }
            }
        }
    }
}

int main() {
    VertexT size_of_field = 0;
    std::cin >> size_of_field;

    Square start{};
    std::cin >> start.x >> start.y;

    Square finish{};
    std::cin >> finish.x >> finish.y;

    GraphList graph(size_of_field * size_of_field, NOT_ORIENTED);

    Grid grid(size_of_field);
    SetNeighbors(graph, grid);

    VertexT linear_start = grid.ConvertXYPositionToLinear(start.x - 1, start.y - 1);
    VertexT linear_finish = grid.ConvertXYPositionToLinear(finish.x - 1, finish.y - 1);
    auto shortest_path = ShortestPath(graph, linear_start, linear_finish);
    PrintShortestPath(shortest_path, size_of_field);

    return 0;
}