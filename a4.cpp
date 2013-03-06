#include "a4.hpp"
#include "image.hpp"
#include "background.hpp"
#include "collider.hpp"
#include <ctime>
#include <time.h>

extern bool debug;

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

    Vector3D m_view      = view;
    Vector3D m_up        = up;
    Vector3D m_side      = view.cross(up);

    double m_width = 2 * tan(M_PI * fov / (2 * 180));

    m_side.normalize();
    m_view.normalize();
    m_up.normalize();

    Background bg(width, height);
    RayCaster rayCaster(eye, bg, root, lights, ambient);

    Image img(width, height, 3);

    double focalLength = (double)width / (2 * tan(M_PI * fov / (2 * 180)));

    double offsetX = (double)width / 2;
    double offsetY = (double)height / 2;

    int hitCount = 0;

    int percentage = 0;
    clock_t t = clock();
    for (int x = 0; x < width; x++) {
      for (int y = 0; y < height; y++) {
            Vector3D dir = ( x / ((double)width) * 2 - 1 ) *
                    tan( fov * M_PI / 360.0 ) *
                    ( (double)width / (double)height ) *
                    m_side + ( y / (double)height * 2 - 1 ) *
                    tan( fov * M_PI / 360.0 ) *
                    -m_up + m_view;
            dir.normalize();

            Point3D rayPoint(x - offsetX, y - offsetY, focalLength);
            // cast_result cr = rayCaster.cast2(eye, rayPoint - eye);
            cast_result cr = rayCaster.cast2(eye, dir);

            // if (debug) cerr << x << ", " << y << endl;

            Colour c = (cr.hit) ? cr.finalColour : bg.getPixelColour(x, y);

            if (cr.hit) {
                hitCount++;
            }

            img(x, y, 0) = c.R();
            img(x, y, 1) = c.G();
            img(x, y, 2) = c.B();

            int newPercentage = (double)(x * height + y) * 100 / (width * height);
            if (newPercentage > percentage && (newPercentage % 10 == 0)) {
              percentage = newPercentage;
              cerr << percentage << "\% done" << endl;
            }
        }
    }

    t = clock() - t;
    cerr << "Took " << ((float)t)/CLOCKS_PER_SEC << " seconds to complete" << endl;
    cerr << "HIT COUNT: " << hitCount << endl;

    img.savePng(filename);
}
