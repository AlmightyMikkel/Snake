// screen.cpp

#include "Screen.h"

#define SFML_STATIC
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <unordered_map>

// Source:
// - https://github.com/dhepper/font8x8
//
// license: 
// - Public Domain
//

const float PI = 3.141592f;

#pragma region Vector2 

Vector2::Vector2()
{
    x = 0.0f;
    y = 0.0f;
}

Vector2::Vector2(const Vector2& rhs)
{
    x = rhs.x;
    y = rhs.y;
}

Vector2::Vector2(const float x, const float y)
{
    this->x = x;
    this->y = y;
}

Vector2 Vector2::operator-() const
{
    return Vector2(-x, -y);
}

Vector2 Vector2::operator+(const Vector2& rhs) const
{
    return Vector2(x + rhs.x, y + rhs.y);
}

Vector2 Vector2::operator-(const Vector2& rhs) const
{
    return Vector2(x - rhs.x, y - rhs.y);
}

Vector2 Vector2::operator*(const float rhs) const
{
    return Vector2(x * rhs, y * rhs);
}

Vector2 Vector2::operator/(const float rhs) const
{
    return Vector2(x / rhs, y / rhs);
}

Vector2& Vector2::operator=(const Vector2& rhs)
{
    x = rhs.x;
    y = rhs.y;
    return *this;
}

Vector2& Vector2::operator+=(const Vector2& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Vector2& Vector2::operator*=(const float rhs)
{
    x *= rhs;
    y *= rhs;
    return *this;
}

Vector2& Vector2::operator/=(const float rhs)
{
    x /= rhs;
    y /= rhs;
    return *this;
}

float Magnitude(const Vector2& rhs)
{
    return sqrt(rhs.x * rhs.x + rhs.y * rhs.y);
}


float Dot(const Vector2& lhs, const Vector2& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}


float Distance(const Vector2& lhs, const Vector2& rhs)
{
    Vector2 diff = rhs - lhs;
    return Magnitude(diff);
}


void Normalize(Vector2& rhs)
{
    float mag = Magnitude(rhs);
    if (mag > 0.0f)
    {
        rhs /= mag;
    }
}

Vector2 Normalized(const Vector2& rhs)
{
    Vector2 result(rhs);
    Normalize(result);
    return result;
}

#pragma endregion

static const unsigned char font8x8_basic[][8] =
{
   { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},   // U+0021 (!)
   { 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0022 (")
   { 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},   // U+0023 (#)
   { 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00},   // U+0024 ($)
   { 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00},   // U+0025 (%)
   { 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00},   // U+0026 (&)
   { 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0027 (')
   { 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},   // U+0028 (()
   { 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},   // U+0029 ())
   { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00},   // U+002A (*)
   { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00},   // U+002B (+)
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+002C (,)
   { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00},   // U+002D (-)
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+002E (.)
   { 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},   // U+002F (/)
   { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},   // U+0030 (0)
   { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},   // U+0031 (1)
   { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},   // U+0032 (2)
   { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},   // U+0033 (3)
   { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},   // U+0034 (4)
   { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},   // U+0035 (5)
   { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},   // U+0036 (6)
   { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},   // U+0037 (7)
   { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+0038 (8)
   { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00},   // U+0039 (9)
   { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+003A (:)
   { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+003B (//)
   { 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00},   // U+003C (<)
   { 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00},   // U+003D (=)
   { 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},   // U+003E (>)
   { 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00},   // U+003F (?)
   { 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00},   // U+0040 (@)
   { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00},   // U+0041 (A)
   { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},   // U+0042 (B)
   { 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},   // U+0043 (C)
   { 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},   // U+0044 (D)
   { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00},   // U+0045 (E)
   { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00},   // U+0046 (F)
   { 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00},   // U+0047 (G)
   { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00},   // U+0048 (H)
   { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0049 (I)
   { 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00},   // U+004A (J)
   { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00},   // U+004B (K)
   { 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00},   // U+004C (L)
   { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00},   // U+004D (M)
   { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00},   // U+004E (N)
   { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00},   // U+004F (O)
   { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},   // U+0050 (P)
   { 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00},   // U+0051 (Q)
   { 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00},   // U+0052 (R)
   { 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00},   // U+0053 (S)
   { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0054 (T)
   { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00},   // U+0055 (U)
   { 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0056 (V)
   { 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00},   // U+0057 (W)
   { 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00},   // U+0058 (X)
   { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},   // U+0059 (Y)
   { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00},   // U+005A (Z)
   { 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00},   // U+005B ([)
   { 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},   // U+005C (\)
   { 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00},   // U+005D (])
   { 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},   // U+005E (^)
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},   // U+005F (_)
   { 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0060 (`)
   { 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00},   // U+0061 (a)
   { 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00},   // U+0062 (b)
   { 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00},   // U+0063 (c)
   { 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00},   // U+0064 (d)
   { 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00},   // U+0065 (e)
   { 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00},   // U+0066 (f)
   { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0067 (g)
   { 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00},   // U+0068 (h)
   { 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0069 (i)
   { 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E},   // U+006A (j)
   { 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},   // U+006B (k)
   { 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+006C (l)
   { 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00},   // U+006D (m)
   { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00},   // U+006E (n)
   { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00},   // U+006F (o)
   { 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},   // U+0070 (p)
   { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},   // U+0071 (q)
   { 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00},   // U+0072 (r)
   { 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00},   // U+0073 (s)
   { 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00},   // U+0074 (t)
   { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},   // U+0075 (u)
   { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0076 (v)
   { 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00},   // U+0077 (w)
   { 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},   // U+0078 (x)
   { 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0079 (y)
   { 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},   // U+007A (z)
   { 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00},   // U+007B ({)
   { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},   // U+007C (|)
   { 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00},   // U+007D (})
   { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+007E (~)
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+007F
};


static Color white{ 0xff, 0xff, 0xff, 0xff };
static sf::Color clearColor{ 0x00, 0x00, 0x00 };
static sf::Texture *gFont;
static sf::RenderWindow *window;
unsigned int _windowWidth;
unsigned int _windowHeight;
static std::unordered_map<std::string, sf::Texture*> textures;
static std::vector<sf::Sprite> sprites;
static std::unordered_map<std::string, sf::SoundBuffer*> soundBuffers;
static std::vector<sf::Sound> sounds;


Screen::Screen(unsigned int windowWidth, unsigned int windowHeight, std::string windowTitle)
{
    ::_windowWidth = windowWidth;
    ::_windowHeight = windowHeight;
    window = new sf::RenderWindow({ windowWidth, windowHeight }, windowTitle);
    window->setFramerateLimit(60);

    const int glyph_atlas_columns = 16;
    const int glyph_atlas_rows = 8;
    const int glyph_count = (sizeof(font8x8_basic) / sizeof(font8x8_basic[0]));

    sf::Image image;
    image.create(128, 128, sf::Color(0, 0, 0, 0));
    for (int glyph = 0; glyph < glyph_count; glyph++) 
    {
          const int glyph_x = glyph % glyph_atlas_columns * 8;
          const int glyph_y = glyph / glyph_atlas_columns * 8;
          for (int y = 0; y < 8; y++) 
          {
                 for (int x = 0; x < 8; x++) 
                 {
                        if (font8x8_basic[glyph][y] & 1 << x) 
                        {
                           image.setPixel(glyph_x + x, glyph_y + y, sf::Color(0xff, 0xff, 0xff, 0xff));
                        }
                 }
          }
    }

    gFont = new sf::Texture;
    gFont->loadFromImage(image);
    gFont->setSmooth(false);
    gFont->setRepeated(false);
}

Screen::~Screen()
{
    delete gFont;
	delete window;
    {
        auto it = textures.begin();
        while (it != textures.end())
        {
            delete (*it).second;
            (*it).second = nullptr;
        }
        textures.clear();
    }
    sprites.clear();
    {
        auto it = soundBuffers.begin();
        while (it != soundBuffers.end())
        {
            delete (*it).second;
            (*it).second = nullptr;
        }
        soundBuffers.clear();
    }
    sounds.clear();
}

bool Screen::IsOpen() const
{
    sf::Event event;
    while (window->pollEvent(event)) 
    {
	    switch (event.type) 
        {
		    case sf::Event::EventType::Closed:
            {
                window->close();
            } break;
	    }
    }

	return window->isOpen();
}

int Screen::GetWindowWidth() const
{
    return ::_windowWidth;
}

int Screen::GetWindowHeight() const
{
    return ::_windowHeight;
}

int Screen::GetMouseX() const
{
   return sf::Mouse::getPosition(*window).x;
}

int Screen::GetMouseY() const
{
   return sf::Mouse::getPosition(*window).y;
}

bool Screen::IsKeyDown(const Key key) const
{
#pragma warning(push)
#pragma warning(disable: 26812)
    sf::Keyboard::Key code = sf::Keyboard::KeyCount;
#pragma warning(pop)

   code = static_cast<sf::Keyboard::Key>(key);
   if (code == sf::Keyboard::KeyCount) 
   {
      return false;
   }

   return sf::Keyboard::isKeyPressed(code);
}

bool Screen::IsButtonDown(const Button button) const
{
#pragma warning(push)
#pragma warning(disable: 26812)
   return sf::Mouse::isButtonPressed(button == Button::Left ? sf::Mouse::Button::Left : sf::Mouse::Button::Right);
#pragma warning(pop)
}

void Screen::SetClearColor(unsigned char r, unsigned char g, unsigned char b)
{
    clearColor.r = r;
    clearColor.g = g;
    clearColor.b = b;
}

void Screen::SetClearColor(Color color)
{
    clearColor.r = color.r;
    clearColor.g = color.g;
    clearColor.b = color.b;
}

void Screen::Clear()
{
	window->clear(clearColor);
}

Sound Screen::LoadSound(std::string filePath)
{
    sf::SoundBuffer* soundBuffer = nullptr;
    if (soundBuffers[filePath] == nullptr)
    {
        soundBuffer = new sf::SoundBuffer();
        if (!soundBuffer->loadFromFile(filePath))
        {
            return {-1};
        }
        soundBuffers[filePath] = soundBuffer;
    }
    else
    {
        soundBuffer = soundBuffers[filePath];
    }
    sounds.push_back(sf::Sound(*soundBuffer));
    Sound s{ static_cast<const int>(sounds.size() - 1) };
    s.volume = 1.0f;
    s.looping = false;
    s.pitch = 1.0f;
    return s;
}

void Screen::PlaySound(const Sound& sound)
{
    if (sounds[sound.ID].getStatus() == sf::SoundSource::Status::Playing)
        return;

    sounds[sound.ID].setVolume(sound.volume);
    sounds[sound.ID].setLoop(sound.looping);
    sounds[sound.ID].setPitch(sound.pitch);
    sounds[sound.ID].play();
}

void Screen::StopSound(const Sound& sound)
{
    if (sounds[sound.ID].getStatus() == sf::SoundSource::Status::Stopped)
        return;

    sounds[sound.ID].stop();
}

Sprite Screen::LoadSprite(std::string filePath)
{
    sf::Texture* texture = nullptr;
    if (textures[filePath] == nullptr)
    {
        texture = new sf::Texture();
        if (!texture->loadFromFile(filePath))
        {
            return {-1};
        }
        textures[filePath] = texture;
    }
    else
    {
        texture = textures[filePath];
    }
    sprites.push_back(sf::Sprite(*texture));
    Sprite s{ static_cast<const int>(sprites.size() - 1)};
    s.angle = 0;
    s.texture.width = texture->getSize().x;
    s.texture.height = texture->getSize().y;
    s.texture.src = { 0,0,(int)s.texture.width,(int)s.texture.height };
    s.color = white;
    s.position = {};
    s.origin = {};
    s.scale = 1.0f;
    return s;
}

void Screen::DrawSprite(const Sprite &sprite)
{
    sprites[sprite.ID].setPosition(sprite.position.x, sprite.position.y);
    sprites[sprite.ID].setRotation(sprite.angle);
    sprites[sprite.ID].setColor({ sprite.color.r, sprite.color.g, sprite.color.b, sprite.color.a });
    sprites[sprite.ID].setOrigin(sprite.origin.x, sprite.origin.y);
    sprites[sprite.ID].setScale(sprite.scale, sprite.scale);
    sprites[sprite.ID].setTextureRect({sprite.texture.src.x, sprite.texture.src.y, sprite.texture.src.w, sprite.texture.src.h});
    window->draw(sprites[sprite.ID]);
}


void Screen::DrawPixel(int x, int y)
{
	DrawPixel(x, y, white);
}
void Screen::DrawPixel(int x, int y, Color color)
{
    DrawPixel(x, y, color.r, color.g, color.b, color.a);
}

void Screen::DrawPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	sf::RectangleShape shape({ 1,1 });
	shape.setFillColor(sf::Color(r, g, b, a));
	sf::RenderStates rs;
	rs.transform.translate({ float(x), float(y) });
	window->draw(shape, rs);
}

void Screen::DrawRectangle(int x, int y, int width, int height)
{
	DrawRectangle(x, y, width, height, white);
}

void Screen::DrawRectangle(int x, int y, int width, int height, Color color)
{
    DrawRectangle(x, y, width, height, color.r, color.g, color.b, color.a);
}

void Screen::DrawRectangle(int x, int y, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	sf::RectangleShape shape({ float(width), float(height) });
	shape.setFillColor(sf::Color(r, g, b, a));
	sf::RenderStates rs;
	rs.transform.translate({ float(x), float(y) });
	window->draw(shape, rs);
}

void Screen::DrawCircle(int x, int y, int radius)
{
	DrawCircle(x, y, radius, white);
}

void Screen::DrawCircle(int x, int y, int radius, Color color)
{
    DrawCircle(x, y, radius, color.r, color.g, color.b, color.a);
}

void Screen::DrawCircle(int x, int y, int radius, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
#pragma warning(push)
#pragma warning(disable: 26451)
    sf::CircleShape shape(float(radius), 5 + radius);
#pragma warning(pop)

    shape.setFillColor(sf::Color(r, g, b, a));
    sf::RenderStates rs;
    rs.transform.translate({ float(x), float(y) });
    window->draw(shape, rs);
}

void Screen::DrawText(int ox, int oy, Color color, const std::string& text, int text_scale)
{
    DrawText(ox, oy, color.r, color.g, color.b, color.a, text, text_scale);
}

void Screen::DrawText(int ox, int oy, unsigned char r, unsigned char g, unsigned char b, unsigned char a, const std::string& text, int text_scale)
{
    const float uvst = 8.0f;
    const int character_width = 8 * text_scale;
    const int line_feed_height = 10 * text_scale;
    const int first_valid_character = (int)'!';
    const int last_valid_character = (int)'~';
    const int invalid_character = (int)'?' - first_valid_character;

    sf::Color color(r, g, b, a);
    std::vector<sf::Vertex> vertices;
    vertices.reserve(6 * text.length());

    int x = ox;
    int y = oy;
    for (size_t index = 0; index < text.length(); index++) 
    {
        int character = (int)text[index];
        if (character == (int)(' ')) 
        {
            x += character_width;
            continue;
        }
        else if (character == (int)('\n')) 
        {
            x  = ox;
            y += line_feed_height;
            continue;
        }

        int character_index = character - first_valid_character;
        if (character_index < 0 || character_index >= last_valid_character) 
        {
            character_index = invalid_character;
        }

        float u = (character_index % 16) * uvst;
        float v = (character_index / 16) * uvst;
        float s = u + uvst;
        float t = v + uvst;

        sf::Vertex quad[6] = 
        {
            { { float(x                  ), float(y                  ) }, color, { u, v } },
            { { float(x + character_width), float(y                  ) }, color, { s, v } },
            { { float(x + character_width), float(y + character_width) }, color, { s, t } },
            { { float(x + character_width), float(y + character_width) }, color, { s, t } },
            { { float(x                  ), float(y + character_width) }, color, { u, t } },
            { { float(x                  ), float(y                  ) }, color, { u, v } },
        };

        for (auto &vert : quad) 
        {
            vertices.push_back(vert);
        }
       
        x += character_width;
   }

   window->draw(vertices.data(), vertices.size(), sf::Triangles, sf::RenderStates(gFont));
}

void Screen::Display()
{
	window->display();
}

extern int main(int, char **);
extern "C" int __stdcall WinMain(struct HINSTANCE__ *, struct HINSTANCE__ *, char *, int)
{
	return main(__argc, __argv);
}
