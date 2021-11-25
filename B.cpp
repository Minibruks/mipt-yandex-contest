#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <list>

using VertexT = int32_t;
using ColorT = int32_t;

enum { ORIENTED = true, NOT_ORIENTED = false };

enum { WHITE = 1, BLACK = 2, NO_COLOR = 0 };

enum { BIPIRITATE = true, NOT_BIPIRITATE = false };

inline int InvertColor(ColorT color) {
    return color == WHITE ? BLACK : WHITE;
}

class IGraph {
protected:
    VertexT vertex_num_ = 0;
    bool oriented_;

public:
    [[nodiscard]] VertexT GetVertexNum() const {
        return vertex_num_;
    }

    [[nodiscard]] virtual std::vector<VertexT> GetNeighbors(const VertexT& current_vertex) const = 0;

    virtual void AddEdge(VertexT& vertex_from, VertexT& vertex_to) = 0;
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

    void AddEdge(VertexT& vertex_from, VertexT& vertex_to) override {
        vertex_from--;
        vertex_to--;

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

    void AddEdge(VertexT& vertex_from, VertexT& vertex_to) override {
        vertex_from--;
        vertex_to--;

        if (vertex_from == vertex_to) {
            return;
        }
        matrix_[vertex_from][vertex_to] = 1;
        if (!oriented_) {
            matrix_[vertex_to][vertex_from] = 1;
        }
    }
};

void DFS(const IGraph& graph, const VertexT& current_vertex, const int current_color, std::vector<ColorT>& colors,
         bool& vertex_is_same_color) {
    colors[current_vertex] = current_color;
    std::vector<VertexT> neighbors = graph.GetNeighbors(current_vertex);

    for (auto& neighbor : neighbors) {
        if (colors[neighbor] == NO_COLOR) {
            DFS(graph, neighbor, InvertColor(current_color), colors, vertex_is_same_color);
        } else if (colors[neighbor] == current_color) {
            vertex_is_same_color = false;
        }
    }
}

bool CheckIfBipartite(const IGraph& graph) {
    std::vector<ColorT> colors(graph.GetVertexNum(), NO_COLOR);
    bool vertex_is_same_color = true;

    for (int i = 0; i < graph.GetVertexNum(); i++) {
        if (colors[i] == NO_COLOR) {
            DFS(graph, i, WHITE, colors, vertex_is_same_color);
            if (!vertex_is_same_color) {
                return NOT_BIPIRITATE;
            }
        }
    }
    return BIPIRITATE;
}

int main() {
    VertexT vertex_num = 0;
    VertexT edge_num = 0;
    std::cin >> vertex_num >> edge_num;

    GraphList graph(vertex_num, NOT_ORIENTED);

    for (VertexT i = 0; i < edge_num; ++i) {
        VertexT vertex_from = 0;
        VertexT vertex_to = 0;
        std::cin >> vertex_from >> vertex_to;
        graph.AddEdge(vertex_from, vertex_to);
    }

    bool answer = CheckIfBipartite(graph);
    if (answer) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }

    return 0;
}