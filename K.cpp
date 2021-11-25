#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>

using VertexT = int32_t;

enum { ORIENTED = true, NOT_ORIENTED = false };
enum { WHITE = 1, BLACK = 2, NO_COLOR = 0, NO_PATH = -1 };
enum { CYCLE = true, NO_CYCLE = false };

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

struct Cycle {
    std::vector<VertexT> used;
    std::vector<VertexT> path;

    explicit Cycle(VertexT size) {
        used.resize(size, NO_COLOR);
        path.resize(size, NO_PATH);
    }
};

void PrintCycle(Cycle& cycle, VertexT cycle_start, VertexT cycle_end) {
    if (cycle_start != -1) {
        std::cout << "YES\n";
        std::vector<int> cycle_found;
        cycle_found.push_back(cycle_start);

        for (auto v = cycle_end; v != cycle_start; v = cycle.path[v]) {
            cycle_found.push_back(v);
        }
        cycle_found.push_back(cycle_start);
        std::reverse(cycle_found.begin(), cycle_found.end());
        for (size_t i = 0; i < cycle_found.size() - 1; i++) {
            std::cout << cycle_found[i] + 1 << " ";
        }
    } else {
        std::cout << "NO\n";
    }
}

bool TryToBuildCycle(const IGraph& graph, VertexT& start, Cycle& cycle, VertexT& cycle_start, VertexT& cycle_end) {
    cycle.used[start] = WHITE;
    cycle.path.push_back(start);

    std::vector<VertexT> neighbors = graph.GetNeighbors(start);
    for (auto& neighbor : neighbors) {
        if (cycle.used[neighbor] == NO_COLOR) {
            cycle.path[neighbor] = start;
            if (TryToBuildCycle(graph, neighbor, cycle, cycle_start, cycle_end)) {
                return CYCLE;
            }
        } else if (cycle.used[neighbor] == WHITE) {
            cycle_end = start;
            cycle_start = neighbor;
            return CYCLE;
        }
    }
    cycle.used[start] = BLACK;
    return NO_CYCLE;
}

void FindCycle(const IGraph& graph) {
    Cycle cycle(graph.GetVertexNum());
    VertexT cycle_start = -1;
    VertexT cycle_end = 0;

    for (VertexT i = 0; i < graph.GetVertexNum(); i++) {
        if (TryToBuildCycle(graph, i, cycle, cycle_start, cycle_end)) {
            break;
        }
    }

    PrintCycle(cycle, cycle_start, cycle_end);
}

int main() {
    VertexT vertex_num = 0;
    VertexT edge_num = 0;
    std::cin >> vertex_num >> edge_num;

    GraphList graph(vertex_num, ORIENTED);

    for (VertexT i = 0; i < edge_num; ++i) {
        VertexT vertex_from = 0;
        VertexT vertex_to = 0;
        std::cin >> vertex_from >> vertex_to;
        graph.AddEdge(vertex_from - 1, vertex_to - 1);
    }

    FindCycle(graph);
    return 0;
}