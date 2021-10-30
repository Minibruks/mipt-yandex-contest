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

    [[nodiscard]] virtual std::vector<uint64_t>& getNeighbors(const uint64_t& v) = 0;

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

    [[nodiscard]] std::vector<uint64_t>& getNeighbors(const uint64_t& v) override {
        return adjacency_list.at(v);
    }

    void addEdge(const uint64_t& source, const uint64_t& dest) override {
        adjacency_list[source].push_back(dest);
        if (!oriented) {
            adjacency_list[dest].push_back(source);
        }
    }
    /*
     * 1  2  3  4   w = 4; h = 3
     * 5  6 (1, 1)  7  8
     * 9 10 11 12
     */

    void setNeighbors(const int64_t& current_vertex, const uint64_t& current_height, const uint64_t& current_width, const uint64_t& height, const uint64_t& width) {
        uint64_t left_border = width * current_height;
        uint64_t right_border = width * (current_height + 1);

        if (current_vertex - 1 >= left_border && current_vertex - 1 < right_border) {
            addEdge(current_vertex, current_vertex - 1);
        }
        if (current_vertex + 1 >= left_border && current_vertex + 1 < right_border) {
            addEdge(current_vertex, current_vertex + 1);
        }

        if ((int64_t)(current_vertex - width) >= 0) {
            addEdge(current_vertex, current_vertex - width);
        }
        if (current_vertex + width < height * width) {
            addEdge(current_vertex, current_vertex + width);
        }
    }
};

std::vector<uint64_t> bfs(Graph& graph, std::vector<uint64_t>& restaurants) {
    std::vector<uint64_t> predecessor(graph.getVertexDegree(), INT32_MAX);
    std::vector<uint64_t> dist(graph.getVertexDegree(), INT32_MAX);
    std::list<uint64_t> queue;

    for (auto &vertex: restaurants) {
        queue.push_back(vertex);
        dist[vertex] = 0;
    }

    while (queue.empty() == 0) {
        uint64_t current_vertex = queue.front();
        queue.pop_front();
        std::vector<uint64_t> neighbors = graph.getNeighbors(current_vertex);

        for (auto &neighbor: neighbors) {
            if (dist[neighbor] == INT32_MAX) {
                dist[neighbor] = dist[current_vertex] + 1;
                predecessor[neighbor] = current_vertex;
                queue.push_back(neighbor);
            }
        }
    }

    return dist;
}

void printDist(const std::vector<uint64_t>& dist, const uint64_t& width) {
    for (uint64_t i = 1; i < dist.size(); i++) {
        std::cout << dist[i - 1] << " ";
        if (i % width == 0) {
            std::cout << "\n";
        }
    }
}


int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long height, width;
    std::cin >> height >> width;
    std::vector<uint64_t> subways;

    GraphList graph(height * width + 1, NOT_ORIENTED);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int cross = 0;
            std::cin >> cross;
            if (cross == 1) {
                subways.push_back(i * width + j);
            }
            graph.setNeighbors(i * width + j, i, j, height, width);
        }
    }

    printDist(bfs(graph, subways), width);
}