#include "RectPacker.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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
    memset(root, 0, sizeof(Node));
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

bool RectPacker::insertSize(Node* node, int w, int h, Rect& r)
{
    /*printf("walking down %d,%d+%dx%d\n",
           node->rect.x, node->rect.y,
           node->rect.width(), node->rect.height());*/
    if (node->child[0]) {
        assert(node->child[1]);
        if (insertSize(node->child[0], w, h, r))
            return true;
        return insertSize(node->child[1], w, h, r);
    }
    if (node->taken)
        return false;
    if (node->rect.width() < w || node->rect.height() < h)
        return false;
    if (w == node->rect.width() && h == node->rect.height()) {
        node->taken = true;
        r = node->rect;
        return true;
    }
    node->child[0] = new Node;
    memset(node->child[0], 0, sizeof(Node));
    node->child[1] = new Node;
    memset(node->child[1], 0, sizeof(Node));

    int dw = node->rect.width() - w;
    int dh = node->rect.height() - h;
    assert(dw >= 0 && dh >= 0);
    if (dw > dh) {
        //printf("1\n");
        node->child[0]->rect = Rect(node->rect.x, node->rect.y,
                                    node->rect.x + w - 1, node->rect.bottom);
        node->child[1]->rect = Rect(node->rect.x + w, node->rect.y,
                                    node->rect.right, node->rect.bottom);
    } else {
        //printf("2\n");
        node->child[0]->rect = Rect(node->rect.x, node->rect.y,
                                    node->rect.right, node->rect.y + h - 1);
        node->child[1]->rect = Rect(node->rect.x, node->rect.y + h,
                                    node->rect.right, node->rect.bottom);
    }
    /*
    printf("created nodes %d,%d+%dx%d, %d,%d+%dx%d (wanting %dx%d)\n",
           node->child[0]->rect.x, node->child[0]->rect.y,
           node->child[0]->rect.width(), node->child[0]->rect.height(),
           node->child[1]->rect.x, node->child[1]->rect.y,
           node->child[1]->rect.width(), node->child[1]->rect.height(),
           w, h);
    */
    return insertSize(node->child[0], w, h, r);
}

Rect RectPacker::insert(int w, int h, bool* ok)
{
    Rect r;
    const bool inserted = insertSize(root, w, h, r);
    if (ok)
        *ok = inserted;
    return r;
}
