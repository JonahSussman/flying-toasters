#include <vector>

#include <SFML/Graphics.hpp>

// Using mingw32, the windows headers are lowercase for some reason...
#include <windows.h> 

#include "sheet.hpp"

using namespace sf;
using namespace std;

int W, H;
Color clear_color;

// If for some reason you are using this program on Windows XP (or even
// earlier... *shudders*) then you cannot use DwmEnableBlurBehindWindow.
// Use this version of setTransparency instead of the other. This function has
// a large CPU performance penalty however. I wish I could figure out how to
// make it better...

// #include <wingdi.h>
// void setTransparency(HWND hWnd) {
//   // For GDI we key out the color. Thus, the background can't be semitransparent
//   clear_color = Color(255, 0, 255);
//   SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
//   SetLayeredWindowAttributes(hWnd, RGB(255, 0, 255), 255, LWA_COLORKEY);
// }

#include <dwmapi.h>
void setTransparency(HWND hWnd) {
  // Background can be any RGBA color. I've set it to darken as a default
  clear_color = Color(0, 0, 0, 31);

  DWM_BLURBEHIND bb = {0};
  bb.dwFlags = DWM_BB_ENABLE;
  bb.fEnable = TRUE;
  bb.hRgnBlur = NULL;
  DwmEnableBlurBehindWindow(hWnd, &bb);

  MARGINS margins = {-1}; // For full transparency
  DwmExtendFrameIntoClientArea(hWnd, &margins);
}

struct Flyer {
  // Data duplication, I know. However, if the initial position of an SFML
  // Sprite is negative, it refuses to render ever again. Thus, the separate
  // Vector2f.
  Vector2f pos; 
  Sprite sprite;

  int frame;
  int bread;
  double speed;
  double timer;
  double LO = 1.0 / 20.0;
  double HI = 1.0 / 5.0;

  inline double get_scale() { return 1.0/(20*speed); }

  void randomize() {
    bread = rand() % 3 ? 0 : 1;
    frame = rand() % 6;

    pos.x = rand() % (2*W);
    pos.y = rand() % (H-64) - H - 64;

    speed = LO + (double)(rand()) / ((double)(RAND_MAX/(HI-LO)));

    sprite.setScale(get_scale(), get_scale());
  }

  void update_anim() {
    auto xpos = frame < 3 ? 64*frame : 192-64*(frame-3); // Ping-pong animation
    sprite.setTextureRect(IntRect(xpos, 64*bread, 64, 64));
  }

  Flyer() {} 
  Flyer(Texture& sheet) {
    sprite = Sprite(sheet);

    randomize();
    update_anim();

    sprite.setPosition(pos);
  }

  // Returns true if a z-ordering resort is needed
  bool draw(double dt, RenderTarget& rt) {
    bool resort = false;
    timer += dt;

    if (timer >= speed) {
      frame = (frame + 1) % 6;
      update_anim();
      timer = 0.0;
    }

    pos.x += -5*dt/speed;
    pos.y +=  5*dt/speed;

    if (pos.x < -64*get_scale() || pos.y > H + 64*get_scale()) { 
      randomize();
      resort = true;
    }

    sprite.setPosition(pos);

    rt.draw(sprite);
    return resort;
  }
};

int main(int argc, char* argv[]) {
  FreeConsole(); // Disable Windows console immediately
  srand(time(nullptr));

  // SFML treats Style::None with a VideoMode exactly the same as desktop 
  // resolution as Style::Fullscreen, breaking the transparency effect
  auto mode = VideoMode::getDesktopMode();
  mode.height += 1;

  RenderWindow window(mode, "flying-toasters", Style::None);
  W = window.getSize().x; 
  H = window.getSize().y;
  window.setFramerateLimit(60);
  window.setMouseCursorVisible(false);

  setTransparency(window.getSystemHandle());

  // Windows has trouble loading from filesystem when launching a screensaver,
  // thus we store the spritesheet in memory
  Texture sheet;
  sheet.loadFromMemory(src_sheet_png, src_sheet_png_len);

  Clock clock; 
  Event event;

  auto mpos = Mouse::getPosition();

  int N = (W/64 * H/64) / 5; // Fill ~20% of the screen with toasters 
  vector<Flyer> v(N);
  for (auto& f : v) f = Flyer(sheet);

  bool resort = true;

  while (window.isOpen()) {
    double dt = clock.restart().asSeconds();
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed || event.type == Event::KeyPressed) {
        window.close();
      }
    }

    auto new_mpos = Mouse::getPosition();
    if (abs(mpos.x - new_mpos.x) > 10 || abs(mpos.y - new_mpos.y) > 10)
      window.close();
    mpos = new_mpos;

    // Makeshift z-sorting
    if (resort) {
      sort(begin(v), end(v), [](auto& a, auto& b){ return a.speed > b.speed; });
      resort = false;
    }

    window.clear(clear_color);
    for (int i = 0; i < N; i++) resort |= v[i].draw(dt, window);
    window.display();
  }

  return 0;
}