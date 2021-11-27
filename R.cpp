#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <queue>
#include <set>

using VertexT = int32_t;
using SizeT = int32_t;

enum { ORIENTED = true, NOT_ORIENTED = false };

class IGraph {
protected:
    VertexT vertex_num_ = 0;
    bool oriented_;

public:
    [[nodiscard]] VertexT GetVertexNum() const {
        return vertex_num_;
    }

    [[nodiscard]] virtual std::vector<VertexT> GetNeighbors(const VertexT& current_vertex) const = 0;

    virtual void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to) = 0;
};

class GraphList final : public IGraph {
private:
    std::vector<std::vector<VertexT>> adjacency_list_;

public:
    explicit GraphList(const VertexT& vertex_number, bool oriented_status) {
        vertex_num_ = vertex_number;
        adjacency_list_.resize(vertex_number);
        for (VertexT i = 0; i < vertex_number; i++) {
            adjacency_list_[i] = std::vector<VertexT>();
        }
        oriented_ = oriented_status;
    }

    [[nodiscard]] std::vector<VertexT> GetNeighbors(const VertexT& v) const override {
        return adjacency_list_.at(v);
    }

    void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to) override {
        adjacency_list_[vertex_from].push_back(vertex_to);
        if (!oriented_) {
            adjacency_list_[vertex_to].push_back(vertex_from);
        }
    }
};

class GraphMatrix final : public IGraph {
private:
    std::vector<std::vector<VertexT>> matrix_;

public:
    explicit GraphMatrix(const VertexT& quantity) {
        for (VertexT i = 0; i < quantity; ++i) {
            std::vector<VertexT> tmp(quantity, 0);
            matrix_.push_back(tmp);
        }
        vertex_num_ = quantity;
    }

    [[nodiscard]] std::vector<VertexT> GetNeighbors(const VertexT& current_vertex) const override {
        std::vector<VertexT> answer;
        for (VertexT i = 0; i < GetVertexNum(); ++i) {
            if (matrix_[current_vertex][i] != 0) {
                answer.push_back(i);
            }
        }
        return answer;
    }

    void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to) override {
        if (vertex_from == vertex_to) {
            return;
        }
        matrix_[vertex_from][vertex_to] = 1;
        if (!oriented_) {
            matrix_[vertex_to][vertex_from] = 1;
        }
    }
};

struct ArtPointsStruct {
    std::vector<bool> used;
    std::vector<VertexT> time_up;
    std::vector<VertexT> time_in;
    std::set<VertexT> art_points;
    int time;

    explicit ArtPointsStruct(SizeT size) {
        used.resize(size, false);
        time_up.resize(size + 1);
        time_in.resize(size + 1);
        time = 1;
    }
};

void DFSforArtPoints(const IGraph& graph, VertexT current_vertex, ArtPointsStruct& art_point, VertexT root = -1) {
    art_point.used[current_vertex] = true;
    art_point.time_in[current_vertex] = art_point.time;
    art_point.time_up[current_vertex] = art_point.time;
    art_point.time++;
    int children = 0;

    std::vector<VertexT> neighbors = graph.GetNeighbors(current_vertex);
    for (auto& neighbor: neighbors) {
        if (neighbor == root) {
            continue;
        }
        if (art_point.used[neighbor]) {
            art_point.time_up[current_vertex] = std::min(art_point.time_up[current_vertex], art_point.time_in[neighbor]);
        } else {
            DFSforArtPoints(graph, neighbor, art_point, current_vertex);
            art_point.time_up[current_vertex] = std::min(art_point.time_up[current_vertex], art_point.time_up[neighbor]);

            if (art_point.time_up[neighbor] >= art_point.time_in[current_vertex] && root != -1) {
                art_point.art_points.insert(current_vertex);
            }
            children++;
        }
    }

    if (root == -1 && children > 1) {
        art_point.art_points.insert(current_vertex);
    }
}

void PrintArtPoints(std::set<VertexT>& art_points) {
    std::cout << art_points.size() << "\n";
    for (auto& point: art_points) {
        std::cout << point + 1 << "\n";
    }
}

std::set<VertexT> FindArtPoints(const IGraph& graph) {
    ArtPointsStruct art_point(graph.GetVertexNum());

    for (VertexT i = 0; i < graph.GetVertexNum(); i++) {
        if (!art_point.used[i]) {
            DFSforArtPoints(graph, i, art_point);
        }
    }

    return art_point.art_points;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    VertexT vertex_num = 0;
    VertexT edge_num = 0;
    std::cin >> vertex_num >> edge_num;

    GraphList graph(vertex_num, NOT_ORIENTED);

    for (VertexT i = 0; i < edge_num; ++i) {
        VertexT vertex_from = 0;
        VertexT vertex_to = 0;
        std::cin >> vertex_from >> vertex_to;
        graph.AddEdge(vertex_from - 1, vertex_to - 1);
    }

    std::set<VertexT> answer = FindArtPoints(graph);
    PrintArtPoints(answer);

    return 0;
}
