#include "molecula_generator.h"

#include <numbers>
#include <queue>

void MoleculaGenerator::init() {
    atoms_descriptions['C'] = AtomDescription{.rgb = {142, 142, 142},
                                              .scale_factor = 0.6f,
                                              .valence = 4};
    atoms_descriptions['H'] = AtomDescription{.rgb = {255, 0, 0},
                                              .scale_factor = 0.5f,
                                              .valence = 1};
    atoms_descriptions['O'] = AtomDescription{.rgb = {255, 255, 0},
                                              .scale_factor = 0.6f,
                                              .valence = 2};
    atoms_descriptions['N'] = AtomDescription{.rgb{53, 100, 250},
                                              .scale_factor = 0.6f,
                                              .valence = 3};

    order.push_back(dir_a);
    order.push_back(dir_b);
    order.push_back(dir_c);
    order.push_back(m_dir);
}

bool MoleculaGenerator::check_formula(std::string formula) {
    if (formula.size() == 0) return false;
    std::transform(formula.begin(), formula.end(), formula.begin(), ::toupper);

    for (int i = 0; i < formula.size(); i++) {
        if (!atoms_descriptions.contains(formula[i]) && formula[i] != '=' && formula[i] != '#')
            return false;
    }

    return true;
}

bool MoleculaGenerator::parse_string(std::string formula) {
    if (!check_formula(formula)) return false;
    std::transform(formula.begin(), formula.end(), formula.begin(), ::toupper);

    m3::vec3 center = {0, 0, 0};
    m3::vec3 prev_center = {0, 0, 0};

    int cur_bonds_count;
    int bonds_count = 0;
    size_t next_dir = order.size() - 1;

    m3::vec3 prev_main_dir = m_dir;
    m3::vec3 main_dir = m_dir;

    BoundType b_type = BoundType::Single;

    std::shared_ptr<Resources> resources = Resources::instance();
    resources->init();

    std::queue<m3::vec3> q;

    fill_queue(q, next_dir);

    for (int i = 0; i < formula.size(); i++) {
        if (atoms_descriptions.contains(formula[i])) {
            b_type = BoundType::Single;
            cur_bonds_count = atoms_descriptions[formula[i]].valence - bonds_count;
            if (i + 1 < formula.size() && atoms_descriptions.contains(formula[i + 1])) {
                cur_bonds_count--;
            }
            if (i + 1 < formula.size() && formula[i + 1] == '=') {
                cur_bonds_count -= 2;
                b_type = BoundType::Double;
            }
            if (i + 1 < formula.size() && formula[i + 1] == '#') {
                cur_bonds_count -= 3;
                b_type = BoundType::Triple;
            }

            place_atom(formula[i], center);

            for (int j = 0; j < cur_bonds_count; j++) {
                m3::vec3 dir = q.front();
                q.pop();
                place_atom('H', center + hydrogen_len_coef * dir);
                place_cylinder(center, dir, m3::len(hydrogen_len_coef * dir), BoundType::Single);
            }

            if (i + 1 == formula.size()) break;

            place_cylinder(center, order[next_dir], m3::len(main_atom_len_coef * order[next_dir]), b_type);

            bonds_count = 1;
            prev_center = center;
            center = center + main_atom_len_coef * order[next_dir];

            next_dir = next_dir == order.size() - 1 ? 0 : (next_dir + 1) % (order.size() - 1);
            prev_main_dir = main_dir;
            main_dir = m3::normalized(prev_center - center);

            if (m3::len(m3::cross(prev_main_dir, main_dir)) < 0.001f) {
                reflect_directions();
            } else {
                m3::vec3 axis = m3::normalized(m3::cross(main_dir, prev_main_dir));
                float angle = std::acos(m3::dot(main_dir, prev_main_dir));
                rotate_directions(axis, angle);
            }

            clear_queue(q);
            fill_queue(q, next_dir);
        } else if (formula[i] == '=') {
            bonds_count = 2;
        } else if (formula[i] == '#') {
            bonds_count = 3;
        }
    }

    return true;
}

void MoleculaGenerator::reflect_directions() {
    for (size_t j = 0; j < order.size(); j++) {
        m3::vec3 up = {0, 1, 0};
        m3::vec3 axis = m3::normalized(m3::cross(order[j], up));
        float angle = -2 * std::acos(m3::dot(order[j], up));
        order[j] = m3::transform_vector(m3::axis_rotation(axis, angle), order[j]);
    }
}

void MoleculaGenerator::rotate_directions(m3::vec3 axis, float angle) {
    for (size_t j = 0; j < order.size(); j++) {
        order[j] = m3::transform_vector(m3::axis_rotation(axis, angle), order[j]);
    }
}

void MoleculaGenerator::fill_queue(std::queue<m3::vec3> &q, size_t next_dir) {
    for (size_t i = 0; i < order.size(); i++)
        if (i != order.size() - 1 && i != next_dir)
            q.push(order[i]);

    q.push(order[next_dir]);
    if (next_dir != order.size() - 1)
        q.push(order[order.size() - 1]);
}

void MoleculaGenerator::clear_queue(std::queue<m3::vec3> &q) {
    std::queue<m3::vec3> empty;
    std::swap(q, empty);
}

void MoleculaGenerator::place_atom(char atom_type, m3::vec3 center) {
    Mesh m = Resources::instance()->sphere;
    color_mesh(m, atoms_descriptions[atom_type].rgb);
    float scale_factor = atoms_descriptions[atom_type].scale_factor;
    m.scale(m3::scale({scale_factor, scale_factor, scale_factor}));
    m.translate(center);
    meshes.push_back(m);
}

void MoleculaGenerator::place_cylinder(m3::vec3 center, m3::vec3 direction, float len, BoundType type) {
    Mesh c;

    switch (type) {
        case BoundType::Single:
            c = Resources::instance()->cylinder;
            break;
        case BoundType::Double:
            c = Resources::instance()->double_cylinder;
            break;
        case BoundType::Triple:
            c = Resources::instance()->triple_cylinder;
            break;
        default:
            c = Resources::instance()->cylinder;
    }

    m3::vec3 up = {0, 1, 0};
    c.scale(m3::scale({cylinder_thickness, len / 2.f, cylinder_thickness}));

    c.translate(center);
    c.translate({0, len / 2.f, 0});

    m3::vec3 axis = m3::normalized(m3::cross(direction, up));
    float angle = std::acos(m3::dot(direction, up));

    c.rotate_around_point(m3::axis_rotation(axis, angle), center);
    meshes.push_back(c);
}

void MoleculaGenerator::color_mesh(Mesh &m, m3::vec3 rgb) {
    for (auto &face : m.faces)
        face.rgb = rgb;
}