#include <QtGui>
#include "droparea.h"
#include <QMimeData>

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
    current= new QImage(QSize(0,0), QImage::Format_ARGB32_Premultiplied);
    current->fill(Qt::transparent);
    next= new QImage(QSize(0,0), QImage::Format_ARGB32_Premultiplied);
    next->fill(Qt::transparent);
    //clear();
}

void DropArea::dragEnterEvent(QDragEnterEvent *event)
{
    //if (!isImageSelected()) setText(tr("<u>drop image here or click to \n invoke file open dialog</u>"));
    if (isImageSelected()) saveCurrentImage();

    const QMimeData *mimeData = event->mimeData();
    setText(mimeData->text());
    QUrl str=mimeData->urls().at(0);
    setPixmap(QPixmap::fromImage(QImage(str.toLocalFile())));
    if(pixmap()->isNull()) setText(tr("<u>drop correct image here or click to \n invoke file open dialog</u>"));
    for (int i=0;i<mimeData->urls().count();i++) qDebug()<< mimeData->urls().at(i);

    setBackgroundRole(QPalette::Dark);
    event->acceptProposedAction();
    emit changed(event->mimeData());
}
void DropArea::dragLeaveEvent(QDragLeaveEvent *event)
{
    restoreCurrentImage();
    if (!isImageSelected()) clear();
    event->accept();
}
void DropArea::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    setText(mimeData->text());
    QUrl str=mimeData->urls().at(0);
    setPixmap(QPixmap::fromImage(QImage(str.toLocalFile())));
    event->acceptProposedAction();
    if(!pixmap()->isNull()){
        setImageSelected(true);
        saveCurrentImage();
        emit dropped();
    }
    else{
        setImageSelected(false);
        saveCurrentImage();
        setText(tr("<u>drop correct image here or click to \n invoke file open dialog</u>"));
    }
}
void DropArea::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
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

void DropArea::saveCurrentImage(){
    delete current;
    current= new QImage(pixmap()->toImage());
}
void DropArea::restoreCurrentImage(){
    setPixmap(QPixmap::fromImage(*current));
}
