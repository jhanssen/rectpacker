#ifndef RECTPACKER_H
#define RECTPACKER_H

struct Rect
{
    Rect() : x(0), y(0), right(0), bottom(0) { }
    Rect(int rx, int ry, int rr, int rb) : x(rx), y(ry), right(rr), bottom(rb) { }

    inline int width() const { return right - x + 1; }
    inline int height() const { return bottom - y + 1; }

    int x, y;
    int right, bottom;
};

class RectPacker
{
public:
    RectPacker(int w, int h);
    ~RectPacker();

    Rect insert(int w, int h, bool* ok = 0);

private:
    struct Node
    {
        Node* child[2];
        Rect rect;
        bool taken;
    };

    void deleteNode(Node* node);
    bool insertSize(Node* node, int w, int h, Rect& r);

    Node* root;
};

#endif
