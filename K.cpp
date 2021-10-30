#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <list>

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

bool findCycle(const Graph& graph, const uint64_t& start, std::vector<uint64_t>& used, std::vector<uint64_t>& path, int& cycle_start, int& cycle_end) {
    used[start] = 1;
    path.push_back(start);

    std::vector<uint64_t> neighbors = graph.getNeighbors(start);
    for (auto& neighbor: neighbors) {
        if (used[neighbor] == 0) {
            path[neighbor] = start;
            if (findCycle(graph, neighbor, used, path, cycle_start, cycle_end)) {
                return true;
            }
        } else if (used[neighbor] == 1) {
            cycle_end = start;
            cycle_start = neighbor;
            return true;
        }
    }
    used[start] = 2;
    return false;
}

void printCycle(const Graph& graph) {
    std::vector<uint64_t> used(graph.getVertexDegree(), 0);
    std::vector<uint64_t> path(graph.getVertexDegree(), -1);
    int cycle_status = 0;
    int cycle_start = -1, cycle_end = 0;

    for (int i = 0; i < graph.getVertexDegree(); i++) {
        if (findCycle(graph, i, used, path, cycle_start, cycle_end)) {
            break;
        }
    }

    if (cycle_start != -1) {
        std::cout << "YES\n";
        std::vector<int> cycle;
        cycle.push_back(cycle_start);

        for (auto v = cycle_end; v != cycle_start; v = path[v]) {
            cycle.push_back(v);
        }
        cycle.push_back(cycle_start);
        std::reverse(cycle.begin(), cycle.end());
        for (size_t i = 0; i < cycle.size() - 1; i++) {
            std::cout << cycle[i] + 1 << " ";
        }
    } else {
        std::cout << "NO\n";
    }
}

int main() {
    uint64_t vertex = 0, edge = 0;
    std::cin >> vertex >> edge;

    GraphList graph(vertex, ORIENTED);

    for (uint64_t i = 0; i < edge; i++) {
        uint64_t a = 0, b = 0;
        std::cin >> a >> b;
        graph.addEdge(a - 1, b - 1);
    }

    printCycle(graph);
    return 0;
}