#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <queue>

const int64_t kMaxNum = 10000;
using VertexT = int32_t;

enum { ORIENTED = true, NOT_ORIENTED = false };

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
        predecessor.resize(size, kMaxNum);
        dist.resize(size, kMaxNum);
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
            if (path.dist[neighbor] == kMaxNum) {
                path.dist[neighbor] = path.dist[current_vertex] + 1;
                path.predecessor[neighbor] = current_vertex;
                queue.push(neighbor);
            }
        }
    }
}

std::vector<VertexT> RestorePath(const VertexT destination, Path& path) {
    std::vector<VertexT> shortest_path;
    if (path.dist[destination] == kMaxNum) {
        return shortest_path;
    }

    VertexT vertex = destination;
    while (vertex != kMaxNum) {
        shortest_path.push_back(vertex);
        vertex = path.predecessor[vertex];
    }
    std::reverse(shortest_path.begin(), shortest_path.end());

    return shortest_path;
}

void PrintShortestPath(const std::vector<VertexT>& path_list) {
    if (path_list.empty()) {
        std::cout << -1 << "\n";
    } else {
        std::cout << path_list.size() - 1 << "\n";
        for (auto& vertex : path_list) {
            std::cout << vertex << "\n";
        }
    }
}

std::vector<VertexT> ShortestPath(const IGraph& graph, const VertexT start, const VertexT finish) {
    Path path(graph.GetVertexNum());

    BuildShortestPath(graph, start, path);
    auto restored_path = RestorePath(finish, path);
    return restored_path;
}

struct ActionsWithNum {
    int IncrementFirstDigit(int num) {
        if (num / 1000 != 9) {
            return num + 1000;
        }
        return num;
    }

    int DecrementFirstDigit(int num) {
        if (num % 10 != 1) {
            return num - 1;
        }
        return num;
    }

    int ShiftLeft(int num) {
        return (num % 1000) * 10 + num / 1000;
    }

    int ShiftRight(int num) {
        return (num % 10) * 1000 + num / 10;
    }

    bool FirstNotNine(int num) {
        return num / 1000 != 9;
    }

    bool LastNotOne(int num) {
        return num % 10 != 1;
    }
};

void SetNeighbors(IGraph& graph) {
    ActionsWithNum action{};
    for (int num = 1000; num < kMaxNum; num++) {
        std::string number_str = std::to_string(num);
        if (number_str.find_first_of('0') != std::string::npos) {
            continue;
        }
        VertexT next_num = 0;

        if (action.FirstNotNine(num)) {
            next_num = action.IncrementFirstDigit(num);
            graph.AddEdge(num, next_num);
        }

        if (action.LastNotOne(num)) {
            next_num = action.DecrementFirstDigit(num);
            graph.AddEdge(num, next_num);
        }

        next_num = action.ShiftLeft(num);
        graph.AddEdge(num, next_num);

        next_num = action.ShiftRight(num);
        graph.AddEdge(num, next_num);
    }
}

int main() {
    VertexT start = 0;
    VertexT finish = 0;
    std::cin >> start >> finish;

    GraphList graph(kMaxNum, ORIENTED);
    SetNeighbors(graph);

    auto shortest_path = ShortestPath(graph, start, finish);
    PrintShortestPath(shortest_path);

    return 0;
}