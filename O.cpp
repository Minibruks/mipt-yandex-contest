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

bool constructPaint(const Graph& graph, const uint64_t& start, std::vector<uint64_t>& used, int& cycle_start, int& cycle_end) {
    used[start] = 1;

    std::vector<uint64_t> neighbors = graph.getNeighbors(start);
    for (auto& neighbor: neighbors) {
        if (used[neighbor] == 0) {
            if (constructPaint(graph, neighbor, used, cycle_start, cycle_end)) {
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

void checkPaint(const Graph& graph) {
    std::vector<uint64_t> used(graph.getVertexDegree(), 0);
    int cycle_start = -1, cycle_end = 0;

    for (int i = 0; i < graph.getVertexDegree(); i++) {
        if (constructPaint(graph, i, used, cycle_start, cycle_end)) {
            break;
        }
    }

    if (cycle_start != -1) {
        std::cout << "NO\n";
    } else {
        std::cout << "YES\n";
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint64_t vertex = 0;
    std::cin >> vertex;
    GraphList graph(vertex, ORIENTED);

    for (int i = vertex - 1; i > 0; i--) {
        for (int j = vertex - i - 1; j < vertex - 1; j++) {
            char color;
            std::cin >> color;
            if (color == 'R') {
                graph.addEdge(vertex - i - 1, j + 1);
            } else {
                graph.addEdge(j + 1, vertex - i - 1);
            }
        }
    }

    checkPaint(graph);

    return 0;
}