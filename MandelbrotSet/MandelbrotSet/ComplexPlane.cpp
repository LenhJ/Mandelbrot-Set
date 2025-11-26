#include "ComplexPlane.h"
#include <cmath>
#include <complex>

using namespace std;
using namespace sf;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) :
    m_pixel_size(pixelWidth, pixelHeight),
    m_aspectRatio(static_cast<float>(pixelHeight) / pixelWidth),
    m_plane_center(0, 0),
    m_plane_size(BASE_WIDTH, BASE_HEIGHT* m_aspectRatio),
    m_zoomCount(0),
    m_state(State::CALCULATING)
{
    m_vArray.setPrimitiveType(Points);
    m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(m_vArray);
}

void ComplexPlane::updateRender()
{
    if (m_state == State::CALCULATING)
    {
        for (int i = 0; i < m_pixel_size.y; i++)
        {
            for (int j = 0; j < m_pixel_size.x; j++)
            {
                m_vArray[j + i * m_pixel_size.x].position = {
                    static_cast<float>(j),
                    static_cast<float>(i)
                };

                Vector2f coord = mapPixelToCoords(Vector2i(j, i));
                int iterations = countIterations(coord);

                Uint8 r, g, b;
                iterationsToRGB(iterations, r, g, b);

                m_vArray[j + i * m_pixel_size.x].color = { r, g, b };
            }
        }
        m_state = State::DISPLAYING;
    }
}

void ComplexPlane::zoomIn()
{
    m_zoomCount++;
    float x_size = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y_size = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = { x_size, y_size };
    m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut()
{
    m_zoomCount--;
    float x_size = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y_size = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = { x_size, y_size };
    m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
    m_plane_center = mapPixelToCoords(mousePixel);
    m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
    m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text)
{
    stringstream ss;
    ss << "Mandelbrot Set\n";
    ss << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")\n";
    ss << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n";
    ss << "Left-click to Zoom in\n";
    ss << "Right-click to Zoom out";

    text.setString(ss.str());
}

int ComplexPlane::countIterations(Vector2f coord)
{
    complex<double> c(coord.x, coord.y);
    complex<double> z(0, 0);
    int count = 0;

    while (abs(z) < 2.0 && count < MAX_ITER)
    {
        z = z * z + c;
        count++;
    }

    return count;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
    if (count == MAX_ITER)
    {
        // Black for points inside the Mandelbrot Set
        r = g = b = 0;
    }
    else
    {
        // Smooth blue-ish gradient
        float t = static_cast<float>(count) / MAX_ITER;

        r = static_cast<Uint8>(9 * (1 - t) * t * t * t * 255);
        g = static_cast<Uint8>(15 * (1 - t) * (1 - t) * t * t * 255);
        b = static_cast<Uint8>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
    }
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
    float x = ((mousePixel.x - 0.0f) / m_pixel_size.x) * m_plane_size.x +
        (m_plane_center.x - m_plane_size.x / 2.0f);

    float y = ((mousePixel.y - m_pixel_size.y) / (0.0f - m_pixel_size.y)) * m_plane_size.y +
        (m_plane_center.y - m_plane_size.y / 2.0f);

    return Vector2f(x, y);
}