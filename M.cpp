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

void dfs(const Graph& graph, uint64_t current_vertex, std::vector<bool>& used, std::vector<uint64_t>& comp) {
    used[current_vertex] = true;
    comp.push_back(current_vertex);

    std::vector<uint64_t> neighbors = graph.getNeighbors(current_vertex);
    for (auto& neighbor: neighbors) {
        if (!used[neighbor]) {
            dfs(graph, neighbor, used, comp);
        }
    }
}

void findComponents(const Graph& graph) {
    std::vector<bool> used(graph.getVertexDegree(), false);
    std::vector<uint64_t> comp;
    std::vector<std::vector<uint64_t>> answer;
    for (auto i = 0; i < graph.getVertexDegree(); i++) {
        if (!used[i]) {
            comp.clear();
            dfs(graph, i, used, comp);
            answer.push_back(comp);
        }
    }

    std::cout << answer.size() << "\n";
    for (auto& chats: answer) {
        std::cout << chats.size() << "\n";
        for (auto& student: chats) {
            std::cout << student + 1 << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    uint64_t students = 0, pairs = 0;
    std::cin >> students >> pairs;

    GraphList graph(students, NOT_ORIENTED);

    for (uint64_t i = 0; i < pairs; i++) {
        uint64_t a = 0, b = 0;
        std::cin >> a >> b;
        graph.addEdge(a - 1, b - 1);
    }

    findComponents(graph);

    return 0;
}