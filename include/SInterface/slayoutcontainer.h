#ifndef SLAYOUTCONTAINER_H
#define SLAYOUTCONTAINER_H

#include "filefunc.h"
#include "qevent.h"
class SUnit;
class MainWindow;
class SLayout;

class SLayoutContainer
{
protected:
    int m_activeInsideInd = -1;
    QWidget* m_self_as_widget;
public:
    // 获取QWidget
    QWidget* asWidget();
    // 初始化
    void initWidget(QWidget* widget);
    // SLayout* inside = nullptr;
    MainWindow* pmw = nullptr;

    QMap<int, SLayout*> insides;
    QMap<SLayout*, QWidget*> fields;



    // virtual void InitLayout() = 0;

    // 当前活动的SLayout
    SLayout* activeInside();

    // 返回对应的SLayout
    SLayout* insideAt(int ind);

    // 返回对应的Field
    QWidget* fieldOf(SLayout* layout);

    // 返回对应的Field
    QWidget* fieldAt(int ind);


    SLayoutContainer(QWidget* widget);

    // 定位放置
    void clearPut(SUnit* aim, bool animated);

    // 是否可以定位放置
    bool OKForClearPut(SUnit* aim);

    // resize后触发
    void afterResize(QResizeEvent* event) ;

    // 设置精简模式
    void setSimpleMode(bool);

    // 设置缩放大小度
    void setScale(double val);

    // 设置透明度
    void setOpacity(double val);

    // 最终更新
    void endUpdate();

    // 设置SLayout
    void addInside(SLayout* aim, QWidget *fieldWidget = nullptr, int ind = -1);

    // 设置活动的SLayout
    void setActiveInside(int ind);

    // 设置活动的SLayout
    void setActiveInside(SLayout* inside);

    QJsonObject to_json() ;
    void load_json(QJsonObject rootObject) ;


    void setPMW(MainWindow *pmw) ;

    void updateColor();

    // 加载内部所有可加载内容
    void loadInsideAll();

    //用于给容器检测时调用，用于过滤不想要的Unit类型
    virtual bool checkType(SUnit* unit);


    //给容器的各种更新函数

    virtual void updateAfterPut(SUnit* aim);
    virtual void updateBeforePut(SUnit* aim);
    virtual void updateAfterRemove(SUnit* aim);
    virtual void updateBeforeRemove(SUnit* aim);


    // 添加SUnit的方法
    virtual bool initAUnit(SUnit* aim, bool notice = true, QPoint globalPos = QPoint(-1, -1));

    // 添加SFile的方法(不创建文件）
    virtual bool addAFile(MyFileInfo &path, bool notice = false, QPoint globalPos = QPoint(-1, -1));
    virtual bool addAFile(QString path, bool notice = false, QPoint globalPos = QPoint(-1, -1));
};

#endif // SLAYOUTCONTAINER_H
