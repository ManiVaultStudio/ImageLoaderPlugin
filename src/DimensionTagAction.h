#pragma once

#include <actions/WidgetAction.h>
#include <actions/StringAction.h>
#include <actions/TriggersAction.h>

class ImageLoaderPlugin;

using namespace hdps::gui;

class DimensionTagAction : public WidgetAction
{

Q_OBJECT

public:

    enum DefaultTag {
        PageName,
        ImageDescription
    };

    static const QMap<DefaultTag, TriggersAction::Trigger> triggers;
    static const QMap<DefaultTag, QString> defaultTags;

protected:

    class Widget : public WidgetActionWidget {
    public:
        Widget(QWidget* parent, DimensionTagAction* dimensionTagAction, const std::int32_t& widgetFlags);
    };

public:

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new Widget(parent, this, widgetFlags);
    };

public:
    DimensionTagAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin);

    QString getTag() const;
    void setTag(const QString& tag);

public:

    StringAction& getTagAction() { return _tagAction; }
    TriggersAction& getDefaultTagsAction() { return _defaultTagsAction; }

signals:

    void tagChanged(const QString& tag);

protected:
    ImageLoaderPlugin&      _imageLoaderPlugin;
    StringAction            _tagAction;
    TriggersAction          _defaultTagsAction;

    friend class Widget;
};