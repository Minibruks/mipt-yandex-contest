#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <list>
#include <set>

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

void dfs(const Graph& graph, uint64_t current_vertex, std::vector<bool>& used, std::vector<uint64_t>& time_up, std::vector<uint64_t>& time_in, std::set<uint64_t>& art_points, int& time, uint64_t root = -1) {
    used[current_vertex] = true;
    time_in[current_vertex] = time;
    time_up[current_vertex] = time;
    time++;
    int children = 0;

    std::vector<uint64_t> neighbors = graph.getNeighbors(current_vertex);
    for (auto& neighbor: neighbors) {
        if (neighbor == root) {
            continue;
        }
        if (used[neighbor]) {
            time_up[current_vertex] = std::min(time_up[current_vertex], time_in[neighbor]);
        } else {
            dfs(graph, neighbor, used, time_up, time_in, art_points, time, current_vertex);
            time_up[current_vertex] = std::min(time_up[current_vertex], time_up[neighbor]);

            if (time_up[neighbor] >= time_in[current_vertex] && root != -1) {
                art_points.insert(current_vertex);
            }
            children++;
        }
    }

    if (root == -1 && children > 1) {
        art_points.insert(current_vertex);
    }
}

void findArtPoints(const Graph& graph, uint64_t vertex_degree) {
    std::vector<bool> used(graph.getVertexDegree(), false);
    std::vector<uint64_t> time_up(graph.getVertexDegree() + 1);
    std::vector<uint64_t> time_in(graph.getVertexDegree() + 1);
    std::set<uint64_t> art_points;

    int time = 1;

    for (auto i = 0; i < graph.getVertexDegree(); i++) {
        if (!used[i]) {
            dfs(graph, i, used, time_up, time_in, art_points, time);
        }
    }

    std::vector<uint64_t> answer;
    for (auto& point: art_points) {
        if (point >= vertex_degree) {
            answer.push_back(point - vertex_degree + 1);
        }
    }
    std::cout << answer.size() << "\n";
    for (auto& point: answer) {
        std::cout << point << "\n";
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint64_t vertex = 0, edges = 0;
    std::cin >> vertex >> edges;

    GraphList graph(vertex + edges, NOT_ORIENTED);

    uint64_t magnetic_pillow = vertex;
    for (uint64_t i = 0; i < edges; i++) {
        uint64_t a = 0, b = 0, c = 0;
        std::cin >> a >> b >> c;
        graph.addEdge(a - 1, magnetic_pillow);
        graph.addEdge(b - 1, magnetic_pillow);
        graph.addEdge(c - 1, magnetic_pillow);
        magnetic_pillow++;
    }

    findArtPoints(graph, vertex);

    return 0;
}