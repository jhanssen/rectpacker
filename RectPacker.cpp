#include "RectPacker.h"
#include <assert.h>

// #define PACK_DEBUG
#ifdef PACK_DEBUG
#include <stdio.h>
#endif

RectPacker::RectPacker()
    : root(0)
{
}

RectPacker::RectPacker(int w, int h)
    : root(0)
{
    init(w, h);
}

RectPacker::~RectPacker()
{
    if (root)
        deleteNode(root);
}

void RectPacker::init(int w, int h)
{
    if (root)
        deleteNode(root);
    root = new Node;
    root->rect.right = w - 1;
    root->rect.bottom = h - 1;
}

void RectPacker::deleteNode(Node* node)
{
    if (node->nodes[0]) {
        assert(node->nodes[1]);
        deleteNode(node->nodes[0]);
        deleteNode(node->nodes[1]);
    }
    delete node;
}

void RectPacker::clear(Node* node)
{
    if (node->nodes[0]) {
        assert(node->nodes[1]);
        deleteNode(node->nodes[0]);
        node->nodes[0] = 0;
        deleteNode(node->nodes[1]);
        node->nodes[1] = 0;
    }
    node->userData = 0;
}

RectPacker::Node* RectPacker::insertSize(Node* node, int w, int h)
{
#ifdef PACK_DEBUG
    printf("walking down %d,%d+%dx%d\n",
           node->rect.x, node->rect.y,
           node->rect.width(), node->rect.height());
#endif
    if (node->nodes[0]) {
        assert(node->nodes[1]);
        if (Node* inserted = insertSize(node->nodes[0], w, h))
            return inserted;
        return insertSize(node->nodes[1], w, h);
    }
    if (node->userData)
        return 0;
    if (node->rect.width() < w || node->rect.height() < h)
        return 0;
    if (w == node->rect.width() && h == node->rect.height())
        return node;
    node->nodes[0] = new Node(node);
    node->nodes[1] = new Node(node);

    int dw = node->rect.width() - w;
    int dh = node->rect.height() - h;
    assert(dw >= 0 && dh >= 0);
    if (dw > dh) {
#ifdef PACK_DEBUG
        printf("left\n");
#endif
        node->nodes[0]->rect = Rect(node->rect.x, node->rect.y,
                                    node->rect.x + w - 1, node->rect.bottom);
        node->nodes[1]->rect = Rect(node->rect.x + w, node->rect.y,
                                    node->rect.right, node->rect.bottom);
    } else {
#ifdef PACK_DEBUG
        printf("right\n");
#endif
        node->nodes[0]->rect = Rect(node->rect.x, node->rect.y,
                                    node->rect.right, node->rect.y + h - 1);
        node->nodes[1]->rect = Rect(node->rect.x, node->rect.y + h,
                                    node->rect.right, node->rect.bottom);
    }
#ifdef PACK_DEBUG
    printf("created nodes %d,%d+%dx%d, %d,%d+%dx%d (wanting %dx%d)\n",
           node->nodes[0]->rect.x, node->nodes[0]->rect.y,
           node->nodes[0]->rect.width(), node->nodes[0]->rect.height(),
           node->nodes[1]->rect.x, node->nodes[1]->rect.y,
           node->nodes[1]->rect.width(), node->nodes[1]->rect.height(),
           w, h);
#endif
    return insertSize(node->nodes[0], w, h);
}

RectPacker::Node* RectPacker::insert(int w, int h)
{
    return insertSize(root, w, h);
}
