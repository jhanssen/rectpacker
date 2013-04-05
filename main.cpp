#include "RectPacker.h"
#include <QtGui>
#include <vector>

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
        const QStringList colors = QColor::colorNames();
        const QStringList::const_iterator colorEnd = colors.end();
        QStringList::const_iterator color = colors.begin();

        QPainter p(this);
        std::vector<Rect>::const_iterator rect = rects.begin();
        const std::vector<Rect>::const_iterator end = rects.end();
        while (rect != end) {
            if (color == colorEnd)
                color = colors.begin();
            qDebug() << "filling" << rect->x << rect->y << rect->width() << rect->height();
            p.fillRect(rect->x, rect->y, rect->width(), rect->height(), QColor(*color));
            ++color;
            ++rect;
        }
    }

private:
    void randomize()
    {
        srandom(time(0));
        enum { RectCount = 800 };
        for (int i = 0; i < RectCount; ++i) {
            const int w = random() % 50;
            const int h = random() % 80;
            Rect r = pack.insert(w, h);
            rects.push_back(r);
        }
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
