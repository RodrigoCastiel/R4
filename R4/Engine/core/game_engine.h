// +--------------------------------------------+
// | File: game_engine.h                        |
// +--------------------------------------------+
// | Author: Rodrigo Castiel.                   |
// | Date:   2017-03-04.                        |
// | All Rights Reserved.                       |
// +--------------------------------------------+

#pragma once

#include <QKeyEvent>
#include <QMouseEvent>

namespace engine
{

class GameEngine
{
public:
    // +++ Constructors/Destructor +++ ----------------------------------------
    GameEngine();
    ~GameEngine();

    // +++ Main +++ -----------------------------------------------------------

    void Load();
    void Update();
    void Render();
    void Resize(int w, int h);

    // ++ User-Interface +++ --------------------------------------------------
    
    void OnKeyboardPress(const QSet<int> & pressedKeys);
    void OnKeyboardRelease(const QSet<int> & pressedKeys);

    void OnMouseMove();

    void OnMouseLeftDown();
    void OnMouseLeftUp();

    void OnMouseRightDown();
    void OnMouseRightUp();

    void OnMouseMiddleDown();
    void OnMouseMiddleUp();

private:

};

}  // namespace engine.