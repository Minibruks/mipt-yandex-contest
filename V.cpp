#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <map>
#include <set>

struct Edge {
    uint64_t from;
    uint64_t to;
};

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

    [[nodiscard]] virtual std::multiset<uint64_t> getNeighbors(const uint64_t& v) const = 0;

    virtual void addEdge(const uint64_t& source, const uint64_t& dest) = 0;

    [[nodiscard]] virtual bool checkMulti(const uint64_t from, const uint64_t to) const = 0;
};

class GraphList final : public Graph {
private:
    std::unordered_map<uint64_t, std::multiset<uint64_t>> adjacency_list;
public:
    explicit GraphList(const uint64_t& vertex_number, bool oriented_status) {
        vertex_degree = vertex_number;
        for (uint64_t i = 0; i < vertex_number; i++) {
            adjacency_list[i] = std::multiset<uint64_t>();
        }
        oriented = oriented_status;
    }

    [[nodiscard]] std::multiset<uint64_t> getNeighbors(const uint64_t& v) const override {
        return adjacency_list.at(v);
    }

    void addEdge(const uint64_t& source, const uint64_t& dest) override {
        adjacency_list[source].insert(dest);
        if (!oriented) {
            adjacency_list[dest].insert(source);
        }
    }

    [[nodiscard]] bool checkMulti(const uint64_t from, const uint64_t to) const override {
        if (adjacency_list.at(from).count(to) == 1) {
            return true;
        }
        return false;
    }
};

std::pair<uint64_t, uint64_t> returnEdge(uint64_t from, uint64_t to) {
    if (from > to) {
        std::swap(from, to);
    }
    return std::make_pair(from, to);
}

void dfs(const Graph& graph, uint64_t current_vertex, std::vector<bool>& used, std::vector<uint64_t>& time_up, std::vector<uint64_t>& time_in, std::set<uint64_t>& bridges, std::map<std::pair<uint64_t, uint64_t>, uint64_t>& edge_list, int& time, uint64_t root = -1) {
    used[current_vertex] = true;
    time_in[current_vertex] = time;
    time_up[current_vertex] = time;
    time++;

    std::multiset<uint64_t> neighbors = graph.getNeighbors(current_vertex);
    for (auto& neighbor: neighbors) {
        if (neighbor == root) {
            continue;
        }
        if (used[neighbor]) {
            time_up[current_vertex] = std::min(time_up[current_vertex], time_in[neighbor]);
        } else {
            dfs(graph, neighbor, used, time_up, time_in, bridges, edge_list, time, current_vertex);
            time_up[current_vertex] = std::min(time_up[current_vertex], time_up[neighbor]);
            if (time_up[neighbor] > time_in[current_vertex] && graph.checkMulti(current_vertex, neighbor)) {
                bridges.insert(edge_list[returnEdge(current_vertex, neighbor)]);
            }
        }
    }

}

void findArtPoints(const Graph& graph, std::map<std::pair<uint64_t, uint64_t>, uint64_t>& edge_list) {
    std::vector<bool> used(graph.getVertexDegree(), false);
    std::vector<uint64_t> time_up(graph.getVertexDegree() + 1);
    std::vector<uint64_t> time_in(graph.getVertexDegree() + 1);
    std::set<uint64_t> bridges;

    int time = 1;

    for (auto i = 0; i < graph.getVertexDegree(); i++) {
        if (!used[i]) {
            dfs(graph, i, used, time_up, time_in, bridges, edge_list, time);
        }
    }

    std::cout << bridges.size() << "\n";
    for (auto& point: bridges) {
        std::cout << point + 1 << "\n";
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint64_t vertex = 0, edges = 0;
    std::cin >> vertex >> edges;

    GraphList graph(vertex, NOT_ORIENTED);
    std::map<std::pair<uint64_t, uint64_t>, uint64_t> edge_list;

    for (uint64_t i = 0; i < edges; i++) {
        uint64_t a = 0, b = 0;
        std::cin >> a >> b;
        graph.addEdge(a - 1, b - 1);
        edge_list[returnEdge(a - 1, b - 1)] = i;
    }

    findArtPoints(graph, edge_list);

    return 0;
}