#include "a4.hpp"
#include "image.hpp"
#include "background.hpp"
#include "collider.hpp"

void a4_render(// What to render
               SceneNode* root,
               // Where to output the image
               const std::string& filename,
               // Image size
               int width, int height,
               // Viewing parameters
               const Point3D& eye, const Vector3D& view,
               const Vector3D& up, double fov,
               // Lighting parameters
               const Colour& ambient,
               const list<Light*>& lights
               )
{
    Background bg(width, height);
    Collider collider(root);

    Image img(width, height, 3);

    cerr << root->getName() << endl;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < height; x++) {
            Ray r = Ray(Point3D(x, y, 0), view, bg, root, collider);
            Colour c = r.cast();

            img(x, y, 0) = c.R();
            img(x, y, 1) = c.G();
            img(x, y, 2) = c.B();
        }
    }

    img.savePng(filename);
}
