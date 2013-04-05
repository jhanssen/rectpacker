#ifndef RECTPACKER_H
#define RECTPACKER_H

#include <string.h>

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
    RectPacker();
    RectPacker(int w, int h);
    ~RectPacker();

    struct Node
    {
        Node(Node* parent = 0) { memset(this, 0, sizeof(Node)); child[2] = parent; }

        Node* child[3];
        Rect rect;
        void* userData;
    };

    void init(int w, int h);

    Node* insert(int w, int h);

private:
    void deleteNode(Node* node);
    Node* insertSize(Node* node, int w, int h);

    Node* root;
};

#endif
