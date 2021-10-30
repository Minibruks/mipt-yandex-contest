#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <queue>
#include <deque>

enum {
    ORIENTED = true,
    NOT_ORIENTED = false
};

struct Edge {
    uint64_t value;
    int weight;
};

class Graph {
protected:
    uint64_t vertex_degree = 0;
    bool oriented;

public:
    [[nodiscard]] uint64_t getVertexDegree() const {
        return vertex_degree;
    }

    [[nodiscard]] virtual std::vector<Edge> getNeighbors(const uint64_t& v) const = 0;

    virtual void addEdge(const uint64_t& source, const uint64_t& dest) = 0;
};

class GraphList final : public Graph {
private:
    std::vector<std::vector<Edge>> adjacency_list;
public:
    explicit GraphList(const uint64_t& vertex_number, bool oriented_status) {
        vertex_degree = vertex_number;
        adjacency_list.resize(vertex_number);
        for (uint64_t i = 0; i < vertex_number; i++) {
            adjacency_list[i] = std::vector<Edge>();
        }
        oriented = oriented_status;
    }

    [[nodiscard]] std::vector<Edge> getNeighbors(const uint64_t& v) const override {
        return adjacency_list.at(v);
    }

    void addEdge(const uint64_t& source, const uint64_t& dest) override {
        adjacency_list[source].push_back({dest, 0});
        if (!oriented) {
            adjacency_list[dest].push_back({source, 1});
        }
    }
};

int64_t bfs(const Graph& graph, const uint64_t& source, const uint64_t& dest) {
    std::vector<uint64_t> dist(graph.getVertexDegree(), INT32_MAX);
    dist[source] = 0;
    std::deque<uint64_t> deq;
    deq.push_back(source);

    while (!deq.empty()) {
        uint64_t current_vertex = deq.front();
        deq.pop_front();
        std::vector<Edge> neighbors = graph.getNeighbors(current_vertex);
        int i = 0;

        for (auto &neighbor: neighbors) {
            if ((dist[neighbor.value] > dist[current_vertex] + /*weights[neighbor][i]*/neighbor.weight) || dist[neighbor.value] == INT32_MAX) {
                dist[neighbor.value] = dist[current_vertex] + neighbor.weight;
                if (neighbor.weight == 0) {
                    deq.push_front(neighbor.value);
                } else {
                    deq.push_back(neighbor.value);
                }
            }
            i++;
        }
    }
    if (dist[dest] > graph.getVertexDegree()) {
        return -1;
    }
    return dist[dest];
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int vertex = 0, edges = 0;
    std::cin >> vertex >> edges;

    GraphList graph(vertex, NOT_ORIENTED);
    for (int i = 0; i < edges; i++) {
        int x = 0, y = 0;
        std::cin >> x >> y;
        graph.addEdge(x - 1, y - 1);
    }

    int questions = 0;
    std::cin >> questions;
    for (int i = 0; i < questions; i++) {
        int u, v;
        std::cin >> u >> v;
        std::cout << bfs(graph, u - 1, v - 1) << "\n";
    }
}