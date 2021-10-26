#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <list>

class Graph {
protected:
    uint64_t vertex_degree = 0;

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
    explicit GraphList(const uint64_t& vertex_number) {
        vertex_degree = vertex_number;
        for (uint64_t i = 0; i < vertex_number; i++) {
            adjacency_list[i] = std::vector<uint64_t>();
        }
    }

    [[nodiscard]] std::vector<uint64_t> getNeighbors(const uint64_t& v) const override {
        return adjacency_list.at(v);
    }

    void addEdge(const uint64_t& source, const uint64_t& dest) override {
        adjacency_list[source].push_back(dest);
        adjacency_list[dest].push_back(source);
    }
};

inline int invertColor(int color) {
    return color == 1 ? 2 : 1;
}

void dfs(const Graph& graph, const uint64_t& current_vertex, const int current_color, std::vector<uint64_t>& colors, bool& flag) {
    colors[current_vertex] = current_color;
    std::vector<uint64_t> neighbors = graph.getNeighbors(current_vertex);

    for (auto& neighbor: neighbors) {
        if (colors[neighbor] == 0) {
            dfs(graph, neighbor, invertColor(current_color), colors, flag);
        } else if (colors[neighbor] == current_color) {
            // return false;
            flag = false;
        }
    }

    // return true;
}

bool checkIfBipartite(const Graph& graph, const uint64_t& vertex_number) {
    std::vector<uint64_t> colors(graph.getVertexDegree(), 0);
    bool flag = true;

    for (int i = 0; i < vertex_number; i++) {
        if (colors[i] == 0) {
            dfs(graph, i, 1, colors, flag);
            if (!flag) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    uint64_t vertex = 0, edge = 0;
    std::cin >> vertex >> edge;

    if (vertex == 1) {
        std::cout << "NO\n";
        return 0;
    }

    GraphList graph(vertex);

    for (uint64_t i = 0; i < edge; ++i) {
        uint64_t a = 0, b = 0;
        std::cin >> a >> b;
        graph.addEdge(a - 1, b - 1);
    }

    bool answer = checkIfBipartite(graph, vertex);
    // printShortestDistance(answer);
    if (answer) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }

    return 0;
}