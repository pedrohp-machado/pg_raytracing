//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//
// FORK para o projeto de processamento gráfico.
//==============================================================================================

#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"


int main() {
    hittable_list world;

    // alterando a cor do chão dentro de color()
    auto ground_material = make_shared<lambertian>(color(0.2, 0.3, 0.1));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    // esfera de vidro
    auto material_glass = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material_glass));

    //esfera fosca
    auto material_lambertian = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material_lambertian));

    //esfera metálica
    auto material_metal = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material_metal));

    // menos esferas aleatórias
    for (int a = -6; a < 6; a++) {
        for (int b = -6; b < 6; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
	    // esferas aleatórias não devem surgir no lugar das grandes
            if ((center - point3(4, 0.2, 0)).length() > 0.9 && (center - point3(-4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 100; //menos ruído (10 -> 100)
    cam.max_depth         = 50; //melhor resultado no vidro

    cam.vfov     = 25;
    cam.lookfrom = point3(0, 2, 13); // mais centralizado
    cam.lookat   = point3(0, 1, 0); 
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.5;
    cam.focus_dist    = 13.0; // 10->13

    cam.render(world);
}
