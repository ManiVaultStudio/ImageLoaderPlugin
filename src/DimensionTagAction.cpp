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
    setEnabled(false);

    _tagAction.setToolTip("The dimension tag for multi-page TIFF files");
    _tagAction.setClearable(true);
    _tagAction.setPlaceHolderString("Enter multi-page TIFF dimension tag here...");

    connect(&_defaultTagsAction, &TriggersAction::triggered, this, [this](std::int32_t triggerIndex) {
        setTag(defaultTags.values().at(triggerIndex));
    });

    connect(&_tagAction, &StringAction::stringChanged, this, &DimensionTagAction::tagChanged);

    connect(this, &DimensionTagAction::tagChanged, this, &DimensionTagAction::updateRows);

    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &DimensionTagAction::dataChanged);
    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, this, &DimensionTagAction::updateStateFromModel);
}

QString DimensionTagAction::getTag() const
{
    return _tagAction.getString();
}

void DimensionTagAction::setTag(const QString& tag)
{
    _tagAction.setString(tag);
}

void DimensionTagAction::setTagSilently(const QString& tag)
{
    disconnect(&_tagAction, &StringAction::stringChanged, this, nullptr);
    {
        _tagAction.setString(tag);
    }
    connect(&_tagAction, &StringAction::stringChanged, this, &DimensionTagAction::updateRows);
}

void DimensionTagAction::updateRows()
{
    disconnect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, nullptr);
    {
        auto& imageCollectionsModel = _imageLoaderPlugin.getImageCollectionsModel();

        for (const auto& selectedImageCollectionIndex : _imageLoaderPlugin.getSelectedRows()) {
            imageCollectionsModel.setData(selectedImageCollectionIndex.siblingAtColumn(ImageCollection::Column::DimensionTag), _tagAction.getString());
            imageCollectionsModel.guessDimensionNames(selectedImageCollectionIndex);
        }
    }
    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &DimensionTagAction::dataChanged);
}

void DimensionTagAction::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles /*= QList<int>()*/)
{
    if (isColumnInModelIndexRange(topLeft, bottomRight, ImageCollection::Column::DimensionTag))
        updateStateFromModel();
}

void DimensionTagAction::updateStateFromModel()
{
    const auto selectedRows         = _imageLoaderPlugin.getSelectedRows();
    const auto numberOfSelectedRows = selectedRows.count();

    if (numberOfSelectedRows == 0)
        setEnabled(false);

    if (numberOfSelectedRows == 1) {
        if (selectedRows.first().siblingAtColumn(ImageCollection::Column::IsMultiPage).data(Qt::EditRole).toBool()) {
            setEnabled(true);
            setTagSilently(selectedRows.first().siblingAtColumn(ImageCollection::Column::DimensionTag).data(Qt::EditRole).toString());
        }
        else {
            setEnabled(false);
            setTagSilently("");
        }
    }

    if (numberOfSelectedRows >= 2) {
        auto numberOfMultiPageTiffs = 0u;

        for (const auto& selectedRow : selectedRows)
            if (selectedRow.siblingAtColumn(ImageCollection::Column::IsMultiPage).data(Qt::EditRole).toBool())
                numberOfMultiPageTiffs++;

        setEnabled(numberOfMultiPageTiffs > 0);

        QSet<QString> tags;

        for (const auto& selectedRow : selectedRows)
            tags.insert(selectedRow.siblingAtColumn(ImageCollection::Column::DimensionTag).data(Qt::EditRole).toString());

        if (tags.count() == 0)
            setTagSilently("");

        if (tags.count() == 1)
            setTagSilently(tags.values().first());

        if (tags.count() >= 2)
            setTagSilently("");
    }
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
