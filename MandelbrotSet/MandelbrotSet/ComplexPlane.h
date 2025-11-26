#ifndef COMPLEXPLANE_H
#define COMPLEXPLANE_H

#include <SFML/Graphics.hpp>
#include <complex>
#include <sstream>

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0f;
const float BASE_HEIGHT = 4.0f;
const float BASE_ZOOM = 0.5f;

enum class State { CALCULATING, DISPLAYING };

class ComplexPlane
{
public:
    ComplexPlane(int pixelWidth, int pixelHeight);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void zoomIn();
    void zoomOut();
    void setCenter(sf::Vector2i mousePixel);
    void setMouseLocation(sf::Vector2i mousePixel);
    void loadText(sf::Text& text);
    void updateRender();

private:
    sf::VertexArray m_vArray;
    State m_state;
    sf::Vector2f m_mouseLocation;
    sf::Vector2i m_pixel_size;
    sf::Vector2f m_plane_center;
    sf::Vector2f m_plane_size;
    int m_zoomCount;
    float m_aspectRatio;

    int countIterations(sf::Vector2f coord);
    void iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b);
    sf::Vector2f mapPixelToCoords(sf::Vector2i mousePixel);
};

#endif
