#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>

enum {
    ORIENTED = true,
    NOT_ORIENTED = false
};

class Graph {
protected:
    uint64_t vertex_degree = 0;
    bool oriented;

public:
    [[nodiscard]] uint64_t getVertexDegree() const {
        return vertex_degree;
    }

    [[nodiscard]] virtual std::vector<uint64_t> getNeighbors(const uint64_t& v) const = 0;

    virtual void addEdge(const uint64_t& source, const uint64_t& dest) = 0;
};

class GraphList final : public Graph {
private:
    std::unordered_map<uint64_t, std::vector<uint64_t>> adjacency_list;
public:
    explicit GraphList(const uint64_t& vertex_number, bool oriented_status) {
        vertex_degree = vertex_number;
        for (uint64_t i = 0; i < vertex_number; i++) {
            adjacency_list[i] = std::vector<uint64_t>();
        }
        oriented = oriented_status;
    }

    [[nodiscard]] std::vector<uint64_t> getNeighbors(const uint64_t& v) const override {
        return adjacency_list.at(v);
    }

    void addEdge(const uint64_t& source, const uint64_t& dest) override {
        adjacency_list[source].push_back(dest);
        if (!oriented) {
            adjacency_list[dest].push_back(source);
        }
    }
};

void dfs(const Graph& graph, const uint64_t& current_vertex, std::vector<int>& used, std::vector<uint64_t>& top, int& cycle_status) {
    used[current_vertex] = 1;

    std::vector<uint64_t> neighbors = graph.getNeighbors(current_vertex);
    for (auto& neighbor: neighbors) {
        if (used[neighbor] == 1) {
            cycle_status = 1;
        }
        if (!used[neighbor]) {
            dfs(graph, neighbor, used, top, cycle_status);
        }
    }
    used[current_vertex] = 2;
    top.push_back(current_vertex);
}

void TopSort(const Graph& graph) {
    std::vector<int> used(graph.getVertexDegree() + 1, 0);
    std::vector<uint64_t> top;
    int cycle_status = 0;

    for (auto i = 0; i < graph.getVertexDegree(); i++) {
        if (!used[i]) {
            dfs(graph, i, used, top, cycle_status);
        }
    }

    if (cycle_status) {
        std::cout << -1 << "\n";
        return;
    }

    std::reverse(top.begin(), top.end());
    for (auto& vertex: top) {
        std::cout << vertex + 1 << " ";
    }
}

int main() {
    uint64_t vertex = 0, edge = 0;
    std::cin >> vertex >> edge;

    GraphList graph(vertex, ORIENTED);

    for (uint64_t i = 0; i < edge; ++i) {
        uint64_t a = 0, b = 0;
        std::cin >> a >> b;
        graph.addEdge(a - 1, b - 1);
    }

    TopSort(graph);
    return 0;
}