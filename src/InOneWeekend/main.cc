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

    auto material_chao = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, material_chao));

    // -----------------------------------
    // Configuração dos Cenário
    // -----------------------------------


    auto parede_fundo = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, 0, 45), 25, parede_fundo));  

    auto cor_decorativa = make_shared<metal>(color(0.3, 0.3, 0.7), 0.1);

    // Adiciona colunas decorativas com esferas empilhadas
    for (int i = -2; i <= 2; ++i) {
        double x = i * 2.5;
        for (int j = 0; j < 3; ++j) {
            double y = 0.7 + j * 1.4;
            world.add(make_shared<sphere>(
                point3(x, y, 20), 0.7, cor_decorativa
            ));
        }
    }

    // Material de vidro 
    auto vidro = make_shared<dielectric>(1.5);

    // Esfera de vidro no topo da parede de fundo
    world.add(make_shared<sphere>(point3(0, 7, 20), 1.0, vidro));

    // Metal cromado para colunas laterais
    auto metal_reflexivo = make_shared<metal>(color(0.8, 0.85, 0.88), 0.0);

    // Coluna metálica lateral esquerda
    for (int i = 0; i < 5; ++i) {
        world.add(make_shared<sphere>(
            point3(-8.0, 0.7 + i * 1.4, 15), 0.7, metal_reflexivo
        ));
    }

    // Coluna metálica lateral direita
    for (int i = 0; i < 5; ++i) {
        world.add(make_shared<sphere>(
            point3(8.0, 0.7 + i * 1.4, 15), 0.7, metal_reflexivo
        ));
}


    // -----------------------------------
    // Configuração dos Personagens
    // -----------------------------------

    //Gru
    auto material_escuro = make_shared<metal>(color(0.2, 0.2, 0.2), 0.2);
    world.add(make_shared<sphere>(point3(0, 0.5, 0), 0.60, material_escuro));

    // Agnes
    auto material_jeans = make_shared<lambertian>(color(0.3, 0.45, 0.65)); 
    world.add(make_shared<sphere>(point3(1.0, 0.15, 0), 0.15, material_jeans));


    // Edith
    auto material_rosa = make_shared<lambertian>(color(0.9, 0.4, 0.6)); 
    world.add(make_shared<sphere>(point3(1.75, 0.30, 0), 0.30, material_rosa));

    // Margo
    auto material_verde = make_shared<lambertian>(color(0.1, 0.25, 0.1));
    world.add(make_shared<sphere>(point3(3, 0.40, 0), 0.40, material_verde));

    // ----------------------------------
    //      Configuração da Plateia
    // ----------------------------------

    int    numero_de_fileiras = 10;      // Quantas fileiras tem na plateia
    int    esferas_por_fileira = 15;     // Quantos "minions" na primeira fileira
    double raio_esfera = 0.35;         // O tamanho de cada "minions" na plateia
    double raio_inicial = 5.0;         // A distância da primeira fileira até o palco
    double espacamento_fileiras = 1.2; // O espaço entre uma fileira e outra

    for (int i = 0; i < numero_de_fileiras; ++i) {

        double raio_atual = raio_inicial + i * espacamento_fileiras;
        // Aumenta o número de esferas nas fileiras de trás para preencher o arco
        int esferas_na_fileira_atual = esferas_por_fileira + i * 2;

        for (int j = 0; j < esferas_na_fileira_atual; ++j) {

            double proporcao_angulo = (double)j / (esferas_na_fileira_atual - 1);
            double angulo = -1.3 + proporcao_angulo * 2.6;

            // Converte as coordenadas do arco (raio, ângulo) para a posição (x, z)
            double x = raio_atual * sin(angulo);
            double z = raio_atual * cos(angulo);

            // Aleatoriedade para parecer mais natural
            x += random_double(-0.2, 0.2);
            z += random_double(-0.2, 0.2);
            
            point3 centro(x, raio_esfera, z);

            auto material_esfera = make_shared<lambertian>(color(0.9, 0.9, 0.2));

            world.add(make_shared<sphere>(centro, raio_esfera, material_esfera));
            
        }
    }


    // ----------------------------
    // Configuração da câmera
    // ----------------------------

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 700;
    cam.samples_per_pixel = 150; 
    cam.max_depth         = 50;

    cam.vfov     = 60; 
    cam.lookfrom = point3(0, 4, -5);  
    cam.lookat   = point3(0, 0.5, 4);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.5;
    cam.focus_dist = 5.0;

    cam.render(world);
}