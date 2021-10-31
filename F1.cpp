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
    int64_t vertex_degree = 0;
    bool oriented;

public:
    [[nodiscard]] int64_t getVertexDegree() const {
        return vertex_degree;
    }

    [[nodiscard]] virtual std::vector<int64_t> getNeighbors(const int64_t& v) const = 0;

    virtual void addEdge(const int64_t& source, const int64_t& dest) = 0;
};

class GraphList final : public Graph {
private:
    std::unordered_map<int64_t, std::vector<int64_t>> adjacency_list;
public:
    explicit GraphList(const int64_t& vertex_number, bool oriented_status) {
        vertex_degree = vertex_number;
        for (int64_t i = 0; i < vertex_number; i++) {
            adjacency_list[i] = std::vector<int64_t>();
        }
        oriented = oriented_status;
    }

    [[nodiscard]] std::vector<int64_t> getNeighbors(const int64_t& v) const override {
        return adjacency_list.at(v);
    }

    void addEdge(const int64_t& source, const int64_t& dest) override {
        if (source != dest) {
            adjacency_list[source].push_back(dest);
            if (!oriented) {
                adjacency_list[dest].push_back(source);
            }
        }
    }
};

bool inWalls(const int64_t& vertex, std::vector<int64_t>& walls) {
    for (auto &wall_pos: walls) {
        if (wall_pos == vertex) {
            return true;
        }
    }
    return false;
}

void setNeighbors(Graph& G, const int64_t& v, const int64_t& width, std::vector<int64_t>& walls) {
    if (inWalls(v, walls)) {
        return;
    }
    int64_t down = v;
    while (true) {
        if (down + width >= G.getVertexDegree() || inWalls(down + width, walls)) {
            break;
        }
        down += width;
    }
    G.addEdge(v, down - (((down - v) / width) / 2) * width);

    int64_t up = v;
    while (true) {
        if (up - (int64_t)width < 0 || inWalls(up - width, walls)) {
            break;
        }
        up -= width;
    }
    G.addEdge(v, up + (((v - up) / width) / 2) * width);

    int64_t border = v / width;
    border *= width;

    int64_t left = v;
    while (true) {
        if (left - 1 < border || inWalls(left - 1, walls)) {
            break;
        }
        left--;
    }
    G.addEdge(v, left + (v - left) / 2);

    int64_t right = v;
    while (true) {
        if (right + 1 >= border + width || inWalls(right + 1, walls)) {
            break;
        }
        right++;
    }
    G.addEdge(v, right - (right - v) / 2);
}

void shortestDistance(const Graph& graph, const int64_t& source, const int64_t& dest) {
    std::vector<int64_t> predecessor(graph.getVertexDegree(), INT32_MAX);
    std::vector<int64_t> dist(graph.getVertexDegree(), INT32_MAX);
    std::list<int64_t> queue;

    dist[source] = 0;
    queue.push_back(source);

    while (queue.empty() == 0) {
        int64_t current_vertex = queue.front();
        queue.pop_front();
        std::vector<int64_t> neighbors = graph.getNeighbors(current_vertex);

        for (auto& neighbor: neighbors) {
            if (dist[neighbor] == INT32_MAX) {
                dist[neighbor] = dist[current_vertex] + 1;
                predecessor[neighbor] = current_vertex;
                queue.push_back(neighbor);
            }
        }
    }

    std::vector<int64_t> shortest_distance;
    if (dist[dest] == INT32_MAX) {
        std::cout << -1 << "\n";
        return;
    }

    int64_t vertex = dest;
    while (vertex != INT32_MAX) {
        shortest_distance.push_back(vertex);
        vertex = predecessor[vertex];
    }
    std::cout << shortest_distance.size() - 1 << "\n";
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int64_t height = 0, width = 0;
    std::cin >> height >> width;
    std::vector<int64_t> walls;

    GraphList graph(height * width, ORIENTED);

    int64_t start = 0, finish = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            char cross = 0;
            std::cin >> cross;
            if (cross == '#') {
                walls.push_back(i * width + j);
            } else if (cross == 'S') {
                start = i * width + j;
            } else if (cross == 'T') {
                finish = i * width + j;
            }
        }
    }

    for (int i = 0; i < height * width; i++) {
        setNeighbors(graph, i, width, walls);
    }

    shortestDistance(graph, start, finish);

    return 0;
}