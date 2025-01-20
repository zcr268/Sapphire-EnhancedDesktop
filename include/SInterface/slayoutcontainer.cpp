#include "slayoutcontainer.h"
#include "qglobal.h"
#include "sdir.h"
#include "sfile.h"
#include "sunit.h"
#include "slayout.h"
#include"snotice.h"
#include "unitfunc.h"
QWidget *SLayoutContainer::asWidget()
{
    return m_self_as_widget;
}

void SLayoutContainer::initWidget(QWidget *widget)
{
    m_self_as_widget = widget;
}

SLayout *SLayoutContainer::activeInside()
{
    if(m_activeInsideInd == -1) {
        return nullptr;
    } else {
        return insides[m_activeInsideInd];
    }
}

QWidget *SLayoutContainer::fieldOf(SLayout *layout)
{
    if(!fields.contains(layout)) {
        return nullptr;
    }
    return fields[layout];
}

QWidget *SLayoutContainer::fieldAt(int ind)
{
    if(!insides.contains(ind)) {
        return nullptr;
    } else {
        return fields[insides[ind]];
    }
}

SLayoutContainer::SLayoutContainer(QWidget *widget)
{
    initWidget(widget);
    qDebug() << "LayoutContainer Created";
};

void SLayoutContainer::clearPut(SUnit* aim, bool animated)
{
    activeInside()->clearPut(aim, animated);
}

bool SLayoutContainer::OKForClearPut(SUnit* aim)
{
    return activeInside()->OKForClearPut(aim);
}
void SLayoutContainer::afterResize(QResizeEvent *event)
{
    activeInside()->afterResize();
    activeInside()->UpdateContentPositon(false);
}

QJsonObject SLayoutContainer::to_json()
{
    return activeInside()->to_json();;
}

void SLayoutContainer::load_json(QJsonObject rootObject)
{
    activeInside()->load_json(rootObject);
}

void SLayoutContainer::setPMW(MainWindow *pmw)
{
    this->pmw = pmw;
    foreach(auto inside, insides) {
        inside->setPMW(pmw);
    }
}

void SLayoutContainer::updateColor()
{
    foreach (auto content, activeInside()->contents) {
        content->updateColor();
    }
}

void SLayoutContainer::loadInsideAll()
{
    foreach (auto content, activeInside()->contents) {
        content->startToLoad();
    }
}

bool SLayoutContainer::checkType(SUnit *unit)
{
    return true;
}

void SLayoutContainer::updateAfterPut(SUnit *aim)
{
    // if(aim->inherits("SDir")) {
    //     ((SDir*)aim)->scanDir();
    // }
}

void SLayoutContainer::updateBeforePut(SUnit *aim)
{

}

void SLayoutContainer::updateAfterRemove(SUnit *aim)
{

}

void SLayoutContainer::updateBeforeRemove(SUnit *aim)
{

}

bool SLayoutContainer::initAUnit(SUnit *aim, bool notice, QPoint globalPos)
{
    if (!aim) {
        return false;
    }
    // qDebug() << "Mainwindow try to add a info" << info.filePath;
    if(!activeInside()->OKForDefaultPut(aim, true)) {
        SNotice::notice("布局无法容纳目标，请调整布局", "布局错误");
        return false;
    };

    if(globalPos == QPoint(-1, -1)) {
        activeInside()->defaultPut(aim, false);
    } else {
        aim->setParent(activeInside()->pContainerW);
        aim->move(activeInside()->pContainerW->mapFromGlobal(globalPos));
        activeInside()->clearPut(aim, false);
    }
    aim->raise();
    if(notice && aim->inherits("SFile")) {
        SNotice::notice(QStringList() << ((SFile*)aim)->filePath, "增添文件", 3000);
    }
    return true;
}

bool SLayoutContainer::addAFile(QString path, bool notice, QPoint globalPos)
{
    return initAUnit(from_path(path, activeInside()),  notice, globalPos);
}
bool SLayoutContainer::addAFile(MyFileInfo& path, bool notice, QPoint globalPos)
{
    return initAUnit(from_info(path, activeInside()),  notice, globalPos);
}

void SLayoutContainer::setSimpleMode(bool val)
{
    foreach(SUnit * content, activeInside()->contents) {
        content->setSimpleMode(val);
    }
}

void SLayoutContainer::setScale(double val)
{
    foreach(SUnit * content, activeInside()->contents) {
        content->setScale(val);
    }
}

void SLayoutContainer::setOpacity(double val)
{
    // qDebug() << val;
    foreach(SUnit * content, activeInside()->contents) {
        // qDebug() << "apply to " << content->objectName() << val;
        content->setOpacity(val);
    }
}

void SLayoutContainer::endUpdate()
{
    foreach(SUnit * content, activeInside()->contents) {
        content->endUpdate();
    }
}

void SLayoutContainer::addInside(SLayout *aim, QWidget* fieldWidget, int ind)
{
    if(aim == nullptr) {
        return;
    }

    //没有特别指定的fieldWidget，就用自己作为fieldWidget
    if(fieldWidget == nullptr) {
        fieldWidget = asWidget();
    }

    // aim->pmw = pmw;
    int aimInd = ind;
    if(aimInd == -1) {
        aimInd = insides.size();
    }


    insides[aimInd] = aim;
    fields[aim] = fieldWidget;

    qInfo() << QString("%1 add a layout").arg(asWidget()->objectName());
    setActiveInside(0);

    aim->setPMW(pmw);
}

void SLayoutContainer::setActiveInside(int ind)
{
    if(ind < 0 || ind >= insides.size()) {
        qWarning() << "SLayoutContainer::setActiveInside: ind out of range";
        return;
    }
    m_activeInsideInd = ind;
    qInfo() << "setActiveInside" << ind;
}

