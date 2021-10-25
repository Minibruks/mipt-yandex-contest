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

    std::vector<uint64_t> shortestDistance(const uint64_t& source, const uint64_t& dest) {
        std::vector<uint64_t> predecessor(vertex_degree, INT32_MAX);
        std::vector<uint64_t> dist(vertex_degree, INT32_MAX);
        std::list<uint64_t> queue;

        dist[source] = 0;
        queue.push_back(source);

        while (queue.empty() == 0) {
            uint64_t current_vertex = queue.front();
            queue.pop_front();
            std::vector<uint64_t> neighbors = getNeighbors(current_vertex);

            for (auto& neighbor: neighbors) {
                if (dist[neighbor] == INT32_MAX) {
                    dist[neighbor] = dist[current_vertex] + 1;
                    predecessor[neighbor] = current_vertex;
                    queue.push_back(neighbor);
                }
            }
        }

        std::vector<uint64_t> shortest_distance;
        if (dist[dest] == INT32_MAX) {
            return shortest_distance;
        }

        uint64_t vertex = dest;
        while (vertex != INT32_MAX) {
            shortest_distance.push_back(vertex);
            vertex = predecessor[vertex];
        }
        std::reverse(shortest_distance.begin(), shortest_distance.end());

        return shortest_distance;
    }
};

void printShortestDistance(const std::vector<uint64_t>& path_list) {
    if (path_list.empty()) {
        std::cout << - 1 << "\n";
    } else {
        std::cout << path_list.size() - 1 << "\n";
        for (auto &vertex: path_list) {
            std::cout << vertex + 1 << " ";
        }
    }
}

int main() {
    uint64_t vertex = 0, edge = 0;
    std::cin >> vertex >> edge;

    uint64_t start = 0, finish = 0;
    std::cin >> start >> finish;

    GraphList graph(vertex);

    for (uint64_t i = 0; i < edge; ++i) {
        uint64_t a = 0, b = 0;
        std::cin >> a >> b;
        graph.addEdge(a - 1, b - 1);
    }

    auto answer = graph.shortestDistance(start - 1, finish - 1);
    printShortestDistance(answer);

    return 0;
}