

#ifndef DROPAREA_H
#define DROPAREA_H

#include <QLabel>

QT_BEGIN_NAMESPACE
class QMimeData;
QT_END_NAMESPACE

//! [DropArea header part1]
class DropArea : public QLabel
{
    Q_OBJECT

public:
    DropArea(QWidget *parent = 0);
    ~DropArea();

public slots:
    void clear();
    bool isImageSelected();
    void setImageSelected(bool sets);

    void saveCurrentImage();
    void restoreCurrentImage();

signals:
    void changed(const QMimeData *mimeData = 0);
    void clicked();
    void dropped();
//! [DropArea header part1]

//! [DropArea header part2]
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);


private:
    bool selected;
    QImage *current, *next;
};
//! [DropArea header part2]

#endif
