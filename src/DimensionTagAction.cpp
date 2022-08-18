#include "DimensionTagAction.h"
#include "ImageLoaderPlugin.h"

const QMap<DimensionTagAction::DefaultTag, TriggersAction::Trigger> DimensionTagAction::triggers = QMap<DimensionTagAction::DefaultTag, TriggersAction::Trigger>({
    { DimensionTagAction::PageName, TriggersAction::Trigger("PageName", "PageName TIFF tag") },
    { DimensionTagAction::ImageDescription, TriggersAction::Trigger("ImageDescription", "ImageDescription TIFF tag") }
    });

const QMap<DimensionTagAction::DefaultTag, QString> DimensionTagAction::defaultTags = QMap<DimensionTagAction::DefaultTag, QString>({
    { DimensionTagAction::PageName, "PageName"},
    { DimensionTagAction::ImageDescription, "ImageDescription"}
});

DimensionTagAction::DimensionTagAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent),
    _imageLoaderPlugin(imageLoaderPlugin),
    _tagAction(this, "Tag"),
    _defaultTagsAction(this, "Default tags", triggers.values().toVector())
{
    setText("Dimension tag");
    
    _tagAction.setClearable(true);
    _tagAction.setPlaceHolderString("Enter multi-page TIFF dimension tag here...");

    connect(&_defaultTagsAction, &TriggersAction::triggered, this, [this](std::int32_t triggerIndex) {
        setTag(defaultTags.values().at(triggerIndex));
    });

    connect(&_tagAction, &StringAction::stringChanged, this, &DimensionTagAction::tagChanged);
}

QString DimensionTagAction::getTag() const
{
    return _tagAction.getString();
}

void DimensionTagAction::setTag(const QString& tag)
{
    _tagAction.setString(tag);
}

DimensionTagAction::Widget::Widget(QWidget* parent, DimensionTagAction* dimensionTagAction, const std::int32_t& widgetFlags) :
    WidgetActionWidget(parent, dimensionTagAction, widgetFlags)
{
    auto mainLayout = new QHBoxLayout();

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(dimensionTagAction->getTagAction().createWidget(this), 1);
    mainLayout->addWidget(dimensionTagAction->getDefaultTagsAction().createWidget(this));

    setLayout(mainLayout);
}
