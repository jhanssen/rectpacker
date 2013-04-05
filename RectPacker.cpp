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
    if (node->child[0])
        deleteNode(node->child[0]);
    if (node->child[1])
        deleteNode(node->child[1]);
    delete node;
}

RectPacker::Node* RectPacker::insertSize(Node* node, int w, int h)
{
#ifdef PACK_DEBUG
    printf("walking down %d,%d+%dx%d\n",
           node->rect.x, node->rect.y,
           node->rect.width(), node->rect.height());
#endif
    if (node->child[0]) {
        assert(node->child[1]);
        if (Node* inserted = insertSize(node->child[0], w, h))
            return inserted;
        return insertSize(node->child[1], w, h);
    }
    if (node->userData)
        return 0;
    if (node->rect.width() < w || node->rect.height() < h)
        return 0;
    if (w == node->rect.width() && h == node->rect.height()) {
        return node;
    }
    node->child[0] = new Node(node);
    node->child[1] = new Node(node);

    int dw = node->rect.width() - w;
    int dh = node->rect.height() - h;
    assert(dw >= 0 && dh >= 0);
    if (dw > dh) {
#ifdef PACK_DEBUG
        printf("left\n");
#endif
        node->child[0]->rect = Rect(node->rect.x, node->rect.y,
                                    node->rect.x + w - 1, node->rect.bottom);
        node->child[1]->rect = Rect(node->rect.x + w, node->rect.y,
                                    node->rect.right, node->rect.bottom);
    } else {
#ifdef PACK_DEBUG
        printf("right\n");
#endif
        node->child[0]->rect = Rect(node->rect.x, node->rect.y,
                                    node->rect.right, node->rect.y + h - 1);
        node->child[1]->rect = Rect(node->rect.x, node->rect.y + h,
                                    node->rect.right, node->rect.bottom);
    }
#ifdef PACK_DEBUG
    printf("created nodes %d,%d+%dx%d, %d,%d+%dx%d (wanting %dx%d)\n",
           node->child[0]->rect.x, node->child[0]->rect.y,
           node->child[0]->rect.width(), node->child[0]->rect.height(),
           node->child[1]->rect.x, node->child[1]->rect.y,
           node->child[1]->rect.width(), node->child[1]->rect.height(),
           w, h);
#endif
    return insertSize(node->child[0], w, h);
}

RectPacker::Node* RectPacker::insert(int w, int h)
{
    return insertSize(root, w, h);
}
