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

    [[nodiscard]] virtual std::list<uint64_t>& getNeighbors(const uint64_t& v) = 0;

    virtual void addEdge(const uint64_t& source, const uint64_t& dest) = 0;
};

class GraphList final : public Graph {
private:
    std::vector<std::list<uint64_t>> adjacency_list;
public:
    explicit GraphList(const uint64_t& vertex_number, bool oriented_status) {
        adjacency_list.resize(vertex_number);
        vertex_degree = vertex_number;
        oriented = oriented_status;
    }

    [[nodiscard]] std::list<uint64_t>& getNeighbors(const uint64_t& v) override {
        return adjacency_list[v];
    }

    void addEdge(const uint64_t& source, const uint64_t& dest) override {
        adjacency_list[source].push_back(dest);
        if (!oriented) {
            adjacency_list[dest].push_back(source);
        }
    }
};

void getPath(Graph& graph, uint64_t current_vertex, std::vector<uint64_t>& path) {
    auto to = 0;
    auto edge = graph.getNeighbors(current_vertex).begin();
    while (!graph.getNeighbors(current_vertex).empty()) {
        to = *edge;
        edge = graph.getNeighbors(current_vertex).erase(edge);
        getPath(graph, to, path);
    }
    path.push_back(current_vertex);
}

void dfs(Graph& graph, uint64_t current_vertex, std::vector<bool>& used) {
    used[current_vertex] = true;
    std::list<uint64_t> neighbors = graph.getNeighbors(current_vertex);

    for (auto& neighbor: neighbors) {
        if (!used[neighbor]) {
            dfs(graph, neighbor, used);
        }
    }
}

void printPath(Graph& graph, std::vector<bool>& empty_square) {
    std::vector<uint64_t> path;
    auto first_not_empty = 0;
    for (auto i =  0; i < graph.getVertexDegree(); i++) {
        if (!empty_square[i]) {
            first_not_empty = i;
            break;
        }
    }

    getPath(graph, first_not_empty, path);

    std::cout << path.size() << " ";
    for (auto i = 0; i < path.size(); i++) {
        std::cout << path[path.size() - i - 1] + 1 << " ";
    }
}

bool checkEulerGraph(Graph& graph, std::vector<bool>& empty_square) {
    std::vector<bool> visited(graph.getVertexDegree(), false);
    auto first_not_empty = 0;
    for (auto i = 0; i < graph.getVertexDegree(); i++) {
        if (!empty_square[i]) {
            first_not_empty = i;
            break;
        }
    }

    dfs(graph, first_not_empty, visited);

    for (auto i = 0; i < graph.getVertexDegree(); i++) {
        if (!visited[i] && !empty_square[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint64_t routes = 0, squares = 0;
    std::cin >> routes >> squares;

    if (routes == 0) {
        std::cout << 0;
        return 0;
    }

    GraphList graph(squares, ORIENTED);

    uint64_t current = 0, next = 0, route_len = 0;
    std::vector<bool> empty_square(squares, true);

    for (uint64_t i = 0; i < routes; i++) {
        std::cin >> route_len >> current;
        for (uint64_t j = 0; j < route_len; j++) {
            std::cin >> next;
            graph.addEdge(current - 1, next - 1);
            empty_square[current - 1] = false;
            empty_square[next - 1] = false;
            current = next;
        }
    }

    if (!checkEulerGraph(graph, empty_square)) {
        std::cout << 0;
        return 0;
    }

    printPath(graph, empty_square);
    return 0;
}