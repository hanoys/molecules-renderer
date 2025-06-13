#pragma once

#include <vector>
#include <unordered_map>
#include <queue>

#include "Math3D.h"
#include "mesh.h"
#include "resources.h"

enum class BoundType {Single, Double, Triple};

struct AtomDescription {
    m3::vec3 rgb = 255;
    float scale_factor = 1;
    int valence = 1;
};

class MoleculaGenerator {
public:
    MoleculaGenerator() = default;

    void init();
    bool parse_string(std::string formula);

    std::vector<Mesh> meshes;
private:
    bool check_formula(std::string formula);
    void place_atom(char atom_type, m3::vec3 center);
    void place_cylinder(m3::vec3 center, m3::vec3 direction, float len, BoundType type);
    void reflect_directions();
    void rotate_directions(m3::vec3 axis, float angle);
    void fill_queue(std::queue<m3::vec3> &q, size_t next_dir);
    void clear_queue(std::queue<m3::vec3> &q);
    void color_mesh(Mesh &m, m3::vec3 rgb);

    std::unordered_map<char, AtomDescription> atoms_descriptions;


    m3::vec3 dir_a = m3::normalized({-0.5f, -std::sqrt(3.f)/6.f, -0.5f});
    m3::vec3 dir_b = m3::normalized({0.5f, -std::sqrt(3.f)/6.f, -0.5f});
    m3::vec3 dir_c = m3::normalized({0, std::sqrt(3.f)/2.f-std::sqrt(3.f)/6.f, -0.5f});
    m3::vec3 m_dir = m3::normalized({0, 0, std::sqrt(6.f)/3.f-0.5f});

    float hydrogen_len_coef = 1.5f;
    float main_atom_len_coef = 2.f;
    float cylinder_thickness = 0.2f;

    std::vector<m3::vec3> order;
};