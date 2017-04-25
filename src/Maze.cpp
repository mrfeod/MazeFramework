#include <QMap>

#include "Maze.h"

#include <QMainWindow>
#include <QApplication>
#include <QScopedPointer>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

using namespace Maze;

namespace
{
    int BUFFER[100][100];

    QList<Ceil> Ceils
    {
        Ceil::Wall, Ceil::Floor, Ceil::Player, Ceil::Exit
    };

    QMap<Ceil, QColor> Color
    {
        { Ceil::Wall,   Qt::darkGray },
        { Ceil::Floor,  Qt::lightGray },
        { Ceil::Player, Qt::green },
        { Ceil::Exit,   Qt::darkGreen }
    };

    QMap<Ceil, QString> Textures
    {
        { Ceil::Wall,   "wall.png" },
        { Ceil::Floor,  "floor.png" },
        { Ceil::Player, "player.png" },
        { Ceil::Exit,   "exit.png" }
    };

    class Window : public QMainWindow
    {
    public:
        Window(int width, int height)
            : m_view(this)
        {
            setFixedSize(width, height);
            m_view.setScene(new QGraphicsScene(this));
            m_view.setFocusPolicy(Qt::NoFocus);
            setCentralWidget(&m_view);

            for(auto& ceil: Ceils)
            {
                m_ceils[ceil] = QPixmap(Textures[ceil], "png");
                if(m_ceils[ceil].isNull())
                {
                    m_ceils[ceil] = QPixmap(25,25);
                    m_ceils[ceil].fill(Color[ceil]);
                }
            }

            setWindowIcon(m_ceils[Ceil::Player]);
        }

    protected:
        void keyPressEvent(QKeyEvent* e)
        {
            QMainWindow::keyPressEvent(e);
            if(update)
            {
                update = m_loop(e->key());
                if(!update)
                {
                    QFont font;
                    QString message("Congratulations! Press any key...");
                    font.setPixelSize(m_view.scene()->width()/message.length());
                    font.setWeight(QFont::Bold);
                    auto text = new QGraphicsTextItem(message);
                    text->setFont(font);
                    text->setDefaultTextColor(Qt::red);
                    text->setPos((m_view.scene()->width() - text->boundingRect().width())/2, (m_view.scene()->height() - text->boundingRect().height())/2);
                    auto rect = new QGraphicsRectItem(text->boundingRect());
                    rect->setPos(text->pos());
                    rect->setBrush(QBrush(Qt::black));
                    m_view.scene()->addItem(rect);
                    m_view.scene()->addItem(text);
                }
            }
            else
                close();
        }

    public:
        callback m_loop;
        QGraphicsView m_view;
        bool update = true;
        bool init = false;
        std::array< std::array<QGraphicsPixmapItem*, 100>, 100> m_items;
        QMap<Ceil, QPixmap> m_ceils;
    };

    class Impl
    {
    public:
        Impl() = default;
        ~Impl() = default;

    public:
        QScopedPointer<QApplication> a;
        QScopedPointer<Window> w;
    };
    static Impl* app;
}

void Maze::Initialize(int& argc, char** argv)
{
    app = new Impl;
    app->a.reset(new QApplication(argc, argv));
}

void Maze::DeInitialize()
{
    delete app;
}

void Maze::CreateWindow(int width, int height)
{
    app->w.reset(new Window(width, height));
}

void Maze::Start(callback loop)
{
    if(!(app->w && app->a))
        return;

    app->w->show();
    loop(0);
    app->w->m_loop = loop;

    app->a->exec();
}

void Maze::Update(int* maze, int width, int height)
{
    int (*MAZE)[100] = (int (*)[100])&maze[0];

    auto scene = app->w->m_view.scene();
    auto& items = app->w->m_items;
    auto& textures = app->w->m_ceils;
    for(int i = 0; i < height; ++i)
    {
         for(int j = 0; j < width; ++j)
         {
             if(!app->w->init)
             {
                auto p = textures[Ceil(MAZE[i][j])];
                items[i][j] = scene->addPixmap(p);
                items[i][j]->setPos(p.width()*j, p.height()*i);
             }
             else if(BUFFER[i][j] != MAZE[i][j])
             {
                 BUFFER[i][j] = MAZE[i][j];
                 items[i][j]->setPixmap(app->w->m_ceils[Ceil(MAZE[i][j])]);
             }
         }
    }
    if(!app->w->init)
        memcpy(BUFFER, MAZE, 100*100*sizeof(int));

    app->w->init = true;
    app->w->m_view.fitInView( scene->sceneRect(), Qt::KeepAspectRatio );
}
