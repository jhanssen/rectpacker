#include "RectPacker.h"
#include <QtGui>
#include <vector>
#include <assert.h>

class Visualizer : public QWidget
{
public:
    Visualizer(QWidget* parent = 0)
        : QWidget(parent), pack(1280, 720)
    {
        resize(1280, 720);

        randomize();
    }

protected:
    void paintEvent(QPaintEvent*)
    {
        const QStringList colors = (QStringList() << "yellow");// QColor::colorNames();
        const QStringList::const_iterator colorEnd = colors.end();
        QStringList::const_iterator color = colors.begin();

        QPainter p(this);
        std::vector<Rect>::const_iterator rect = rects.begin();
        const std::vector<Rect>::const_iterator end = rects.end();
        while (rect != end) {
            if (color == colorEnd)
                color = colors.begin();
            //qDebug() << "filling" << rect->x << rect->y << rect->width() << rect->height();
            p.fillRect(rect->x, rect->y, rect->width(), rect->height(), QColor(*color));
            ++color;
            ++rect;
        }
    }

private:
    void randomize()
    {
        srandom(time(0));
        enum { RectCount = 1000,
               MinWidth = 10, MaxWidth = 50,
               MinHeight = 20, MaxHeight = 80 };
        int tot = 0;
        for (int i = 0; i < RectCount; ++i) {
            const int w = std::max<int>(random() % MaxWidth, MinWidth);
            const int h = std::max<int>(random() % MaxHeight, MinHeight);
            RectPacker::Node* node = pack.insert(w, h);
            if (!node) {
                qWarning("Unable to fit size %dx%d", w, h);
                continue;
            }
            tot += w * h;
            assert(node->rect.width() == w && node->rect.height() == h);
            node->userData = reinterpret_cast<void*>(0x1);
            rects.push_back(node->rect);
        }
        printf("percentage %f\n", static_cast<float>(tot) / (1280. * 720));
    }

private:
    RectPacker pack;
    std::vector<Rect> rects;
};

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    Visualizer vis;
    vis.show();

    return app.exec();
}
