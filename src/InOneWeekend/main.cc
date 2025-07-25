//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"


int main() {
    hittable_list world;

    // Chão
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    // Materiais
    auto green_leaf = make_shared<lambertian>(color(0.1, 0.5, 0.1));
    auto shiny_red = make_shared<metal>(color(0.8, 0.1, 0.1), 0.0);
    auto shiny_silver = make_shared<metal>(color(0.8, 0.8, 0.8), 0.0);
    auto trunk = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    auto gold = make_shared<metal>(color(1.0, 0.85, 0.3), 0.0);

    // Tronco
    world.add(make_shared<sphere>(point3(0, 0.5, 0), 0.5, trunk));
    world.add(make_shared<sphere>(point3(0, 1.2, 0), 0.4, trunk));

    double base_y = 1.7;

    // Parâmetros da árvore
    int layers = 6;                   // número de camadas
    double r = 0.3;                   // raio da esfera
    double spacing = 2.0 * r * 0.90;  // espaçamento entre esferas (ligeiramente maior que o diâmetro)

    for (int y = 0; y < layers; y++) {
        int tipo = 0;
        int spheres_per_row = layers - y;
        double y_pos = base_y + r + y * spacing;

        for (int i = 0; i < spheres_per_row; i++) {
            for (int j = 0; j < spheres_per_row; j++) {
                double x = (i - (spheres_per_row - 1) / 2.0) * spacing;
                double z = (j - (spheres_per_row - 1) / 2.0) * spacing;
                point3 center(x, y_pos, z);
                if (tipo % 3 == 0){
                    world.add(make_shared<sphere>(center, r, shiny_red));
                } else if (tipo % 5 == 0){
                    world.add(make_shared<sphere>(center, r, shiny_silver));
                } else {
                    world.add(make_shared<sphere>(center, r, green_leaf));
                }
                tipo++;
            }
        }
    }

    // Estrela no topo
    double top_y = base_y + layers * 2 * r;
    world.add(make_shared<sphere>(point3(0, top_y, 0), 0.3, gold));

    // Câmera
    camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 150;
    cam.max_depth         = 50;

    cam.vfov     = 30;
    cam.lookfrom = point3(0, 4, 12);
    cam.lookat   = point3(0, 3, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.0;
    cam.focus_dist    = 10.0;

    cam.render(world);
}
