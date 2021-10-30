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
    int vertex_degree = 0;
    bool oriented;

public:
    [[nodiscard]] int getVertexDegree() const {
        return vertex_degree;
    }

    [[nodiscard]] virtual std::vector<int> getNeighbors(const int& v) const = 0;

    virtual void addEdge(const int& source, const int& dest) = 0;
};

class GraphList final : public Graph {
private:
    std::unordered_map<int, std::vector<int>> adjacency_list;
public:
    explicit GraphList(const int& vertex_number, bool oriented_status) {
        vertex_degree = vertex_number;
        for (int i = 0; i < vertex_number; i++) {
            adjacency_list[i] = std::vector<int>();
        }
        oriented = oriented_status;
    }

    [[nodiscard]] std::vector<int> getNeighbors(const int& v) const override {
        return adjacency_list.at(v);
    }

    void addEdge(const int& source, const int& dest) override {
        adjacency_list[source].push_back(dest);
        if (!oriented) {
            adjacency_list[dest].push_back(source);
        }
    }
};

void dfs(const Graph& graph, const int& current_vertex, std::vector<int>& used) {
    used[current_vertex] = 1;
    std::vector<int> neighbors = graph.getNeighbors(current_vertex);

    for (auto& neighbor: neighbors) {
        if (!used[neighbor]) {
            dfs(graph, neighbor, used);
        }
    }
}

int BrokenMin(const Graph& graph, const uint64_t& vertex_number) {
    int answer = 0;
    std::vector<int> used(graph.getVertexDegree() * 100, 0);
    for (int i = 0; i < vertex_number; i++) {
        if (!used[i]) {
            dfs(graph, i, used);
            answer++;
        }
    }
    return answer;
}

int main() {
    int vertex = 0;
    std::cin >> vertex;

    GraphList graph(vertex, NOT_ORIENTED);

    for (int i = 0; i < vertex; i++) {
        int a = 0;
        std::cin >> a;
        graph.addEdge(a - 1, i);
    }

    std::cout << BrokenMin(graph, vertex) << "\n";
    return 0;
}