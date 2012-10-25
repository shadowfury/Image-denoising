#include <QtGui>
#include "droparea.h"

DropArea::DropArea(QWidget *parent)
    : QLabel(parent)
{
    setMinimumSize(200, 200);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(true);
    setAutoFillBackground(true);
    setTextFormat(Qt::RichText);
    setImageSelected(false);
    clear();
}

void DropArea::dragEnterEvent(QDragEnterEvent *event)
{
    if (!isImageSelected()) setText(tr("<u>drop image here or click to \n invoke file open dialog</u>"));

    setBackgroundRole(QPalette::Dark);
    event->acceptProposedAction();
    emit changed(event->mimeData());
}
void DropArea::enterEvent(QEvent *event)
{
    if (!isImageSelected()) setText(tr("<u>drop image here or click to \n invoke file open dialog</u>"));
    setBackgroundRole(QPalette::Dark);
    event->accept();

}
void DropArea::leaveEvent(QEvent *event)
{
    clear();
    event->accept();

}

void DropArea::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void DropArea::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    setText(mimeData->text());
    qDebug()<<"path= "<<mimeData->text();
    setPixmap(QPixmap::fromImage(QImage(mimeData->text().remove("file://").simplified())));
    event->acceptProposedAction();
    if(!pixmap()->isNull()){
        setImageSelected(true);
        emit dropped();
    }
    else setText(tr("<u>Please drop correct image here or click to \n invoke file open dialog</u>"));
}
void DropArea::dragLeaveEvent(QDragLeaveEvent *event)
{
    clear();
    event->accept();
}
void DropArea::clear()
{
    if (!isImageSelected()) setText(tr("drop image here or click to invoke file open dialog"));
    setBackgroundRole(QPalette::Midlight);
    emit changed();
}
bool DropArea::isImageSelected(){
    return selected;
}
void DropArea::setImageSelected(bool sets){
    selected=sets;
}
void DropArea::mousePressEvent(QMouseEvent *event){
    event->accept();
    emit clicked();
}
