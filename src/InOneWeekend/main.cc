#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main() {
    hittable_list world;

    // ------------------------
    // Chão
    // ------------------------
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    // ------------------------
    // Balões no ar
    // ------------------------
    for (int i = 0; i < 100; ++i) {
        auto center = point3(
            0 + 1.5 * random_double(-1, 1),
            5 + 1.5 * random_double(0, 1),
            0 + 1.5 * random_double(-1, 1)
        );

        auto color_balloon = color::random();
        auto balloon_material = make_shared<lambertian>(color_balloon);
        world.add(make_shared<sphere>(center, 0.3, balloon_material));
    }

    // ------------------------
    // Casa de bolinhas
    // ------------------------

    auto wall_color = make_shared<lambertian>(color(0.72, 0.45, 0.2)); // cor da parede
    auto roof_color = make_shared<lambertian>(color(0.5, 0.1, 0.1));   // cor do telhado

    const double r = 0.3; // raio das bolinhas
    const int largura = 5;
    const int altura = 6;

    // Paredes da frente e fundo
    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            world.add(make_shared<sphere>(point3(-1.2 + x*r*2, y*r*2 + r, -1.5), r, wall_color)); // frente
            world.add(make_shared<sphere>(point3(-1.2 + x*r*2, y*r*2 + r,  1.5), r, wall_color)); // fundo
        }
    }

    // Paredes laterais
    for (int y = 0; y < altura; ++y) {
        for (int z = 1; z < largura-1; ++z) {
            world.add(make_shared<sphere>(point3(-1.5, y*r*2 + r, -1.2 + z*r*2), r, wall_color)); // esquerda
            world.add(make_shared<sphere>(point3( 1.5, y*r*2 + r, -1.2 + z*r*2), r, wall_color)); // direita
        }
    }

    // Telhado: duas diagonais de esferas
    for (int i = 0; i < largura; ++i) {
        world.add(make_shared<sphere>(
            point3(-1.2 + i*r*2, altura*r*2 + r, -1.5 - i*r*0.6),
            r, roof_color));
        world.add(make_shared<sphere>(
            point3(-1.2 + i*r*2, altura*r*2 + r, 1.5 + i*r*0.6),
            r, roof_color));
    }

    // ------------------------
    // Câmera
    // ------------------------
    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 800;
    cam.samples_per_pixel = 50;
    cam.max_depth         = 20;

    cam.vfov     = 40;
    cam.lookfrom = point3(13, 4, 6);
    cam.lookat   = point3(0, 1, 0);
    cam.vup      = vec3(0, 1, 0);

    cam.defocus_angle = 0.05;
    cam.focus_dist    = 10.0;

    cam.render(world);
}