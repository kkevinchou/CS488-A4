#include "a4.hpp"
#include "image.hpp"
#include "background.hpp"
#include "collider.hpp"

bool debug = false;

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
    cerr << "RUNNING..........................." << endl;

    Background bg(width, height);
    RayCaster rayCaster(eye, bg, root, lights);

    Image img(width, height, 3);

    double focalLength = (double)width / (2 * tan(fov / 2));

    double offsetX = (double)width / 2;
    double offsetY = (double)height / 2;

    int hitCount = 0;

    for (int x = 0; x < width; x++) {
      for (int y = 0; y < height; y++) {
            if (x == 250 && height - y - 1 == 250) {
                debug = true;
            }
            Point3D rayPoint(x - offsetX, y - offsetY, focalLength);
            cast_result cr = rayCaster.cast(eye, rayPoint - eye);

            Colour c = (cr.hit) ? cr.colour : bg.getPixelColour(x, y);

            if (cr.hit) {
                hitCount++;
            }

            // cerr << x << ", " << y << endl;
            img(x, height - y - 1, 0) = c.R();
            img(x, height - y - 1, 1) = c.G();
            img(x, height - y - 1, 2) = c.B();
        }
    }

    cerr << "HIT COUNT: " << hitCount << endl;

    img.savePng(filename);
}
