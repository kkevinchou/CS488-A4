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
    cerr << "RUNNING..........................." << endl;

    Background bg(width, height);
    RayCaster rayCaster(eye, bg, root);

    Image img(width, height, 3);

    double focalLength = (double)width / (2 * tan(fov / 2));

    double offsetX = (double)width / 2;
    double offsetY = (double)height / 2;

    int hitCount = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < height; x++) {
    // for (int y = 0; y < 1; y++) {
    //     for (int x = 0; x < 1; x++) {
            Point3D rayPoint(x - offsetX, y - offsetY, focalLength);
            cast_result cr = rayCaster.cast(eye, rayPoint - eye);

            Colour c = (cr.hit) ? cr.colour : bg.getPixelColour(x, y);

            if (cr.hit) {
              hitCount++;
            }

            img(x, y, 0) = c.R();
            img(x, y, 1) = c.G();
            img(x, y, 2) = c.B();
        }
    }

    cerr << "HIT COUNT: " << hitCount << endl;

    img.savePng(filename);
}
