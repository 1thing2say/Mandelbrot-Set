#include "ComplexPlane.h"

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
    m_pixel_size = { pixelWidth, pixelHeight };
    m_aspectRatio = static_cast<float>(pixelHeight) / pixelWidth;
    m_plane_center = { 0, 0 };
    m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
    m_zoomCount = 0;
    m_state = State::CALCULATING;
    m_vArray.setPrimitiveType(Points);
    m_vArray.resize(pixelWidth * pixelHeight);
}
void ComplexPlane::draw(RenderTarget& target, RenderStates states) const 
{
    target.draw(m_vArray);
}
void ComplexPlane::zoomIn()
{
    // Try to switch float to double and see if that increases accuracy
    m_zoomCount++;
    float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = { x, y };
    m_state = State::CALCULATING;
}
void ComplexPlane::zoomOut()
{
    m_zoomCount--;
    float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = { x, y };
    m_state = State::CALCULATING;
}
void ComplexPlane::setCenter(Vector2i mousePixel)
{
    m_plane_center = mapPixelToCoords(mousePixel);
    m_state = State::CALCULATING;
}
void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
    m_mouseLocation = ComplexPlane::mapPixelToCoords(mousePixel);
}
void ComplexPlane::loadText(Text& text)
{
    stringstream ss;
    ss << "Mandelbrot Set\n";
    ss << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")\n";
    ss << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n";
    ss << "Left-click to Zoom in\nRight-click to Zoom out";
    text.setString(ss.str());
}
void ComplexPlane::updateRender()
{
    if (m_state == State::CALCULATING)
    {
        for (int i = 0; i < m_pixel_size.x; i++)
        {
            for (int j = 0; j < m_pixel_size.y; j++)
            {
                m_vArray[j + i * m_plane_size.x].position = {(float)j, (float)i};
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
//priv
int ComplexPlane::countIterations(Vector2f coord)
{
    complex<double> c(coord.x, coord.y);
    // Begin with z = 0
    complex<double> z(0, 0);
    int i = 0;
    //try this and it should be faster:
    //while((z.real() * z.real() + z.imag() + z.imag()) < 4.0 && i < 64)
    while(abs(z) < 2.0 && i < 64)
    {
        z = z*z + c;
        //cout << "z_" << i << "= " << z << endl;
        //cout << "|z| = " << abs(c) << endl;
        i++;
    }
    return i;
    /*
    if(i == 64) cout << c << " converges!" << endl;
    else cout << c << " escapes after " << i << " iterations" << endl;
    */
}
void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
    if (count == MAX_ITER)
    {
        r = 0;
        g = 0;
        b = 0;
    }
    else 
    {
        r = static_cast<Uint8>((count * 255) / MAX_ITER);
        g = static_cast<Uint8>((count * 255) / MAX_ITER);
        b = static_cast<Uint8>((count * 64) / MAX_ITER);
    }
}
Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
    float pixelX = mousePixel.x;
    float pixelY = mousePixel.y;
    float coordX = ((pixelX / m_pixel_size.x) * m_plane_size.x) + (m_plane_center.x - m_plane_size.x / 2.0);
    float coordY = ((pixelY / m_pixel_size.y) * m_plane_size.y) + (m_plane_center.y - m_plane_size.y / 2.0);
    return Vector2f(coordX, coordY);
}

