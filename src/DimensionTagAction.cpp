#include "DimensionTagAction.h"
#include "ImageLoaderPlugin.h"

#include <QProgressDialog>

const QMap<DimensionTagAction::DefaultTag, TriggersAction::Trigger> DimensionTagAction::triggers = QMap<DimensionTagAction::DefaultTag, TriggersAction::Trigger>({
    { DimensionTagAction::PageName, TriggersAction::Trigger("PageName", "PageName TIFF tag") },
    { DimensionTagAction::ImageDescription, TriggersAction::Trigger("ImageDescription", "ImageDescription TIFF tag") }
    });

const QMap<DimensionTagAction::DefaultTag, QString> DimensionTagAction::defaultTags = QMap<DimensionTagAction::DefaultTag, QString>({
    { DimensionTagAction::PageName, "PageName"},
    { DimensionTagAction::ImageDescription, "ImageDescription"}
});

DimensionTagAction::DimensionTagAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent, "Dimension"),
    _imageLoaderPlugin(imageLoaderPlugin),
    _tagAction(this, "Tag"),
    _defaultTagsAction(this, "Default tags", triggers.values().toVector())
{
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

        QProgressDialog progressDialog("Establishing dimension names", "", 0, _imageLoaderPlugin.getSelectedRows().count(), nullptr);

        progressDialog.setWindowTitle(QString("Establishing dimension names for %1 collection(s)").arg(QString::number(_imageLoaderPlugin.getSelectedRows().count())));
        progressDialog.setWindowIcon(mv::Application::getIconFont("FontAwesome").getIcon("images"));
        progressDialog.setWindowModality(Qt::WindowModal);
        progressDialog.setMinimumDuration(1000);
        progressDialog.setFixedWidth(600);
        progressDialog.setMinimum(0);
        progressDialog.setMaximum(_imageLoaderPlugin.getSelectedRows().count());
        progressDialog.setValue(0);

        emit beginEstablishDimensionNames();
        {
            setEnabled(false);
            {
                for (const auto& selectedImageCollectionIndex : _imageLoaderPlugin.getSelectedRows()) {
                    progressDialog.setLabelText(QString("Extracting dimensions for %1").arg(selectedImageCollectionIndex.siblingAtColumn(ImageCollection::Column::Name).data(Qt::EditRole).toString()));

                    imageCollectionsModel.setData(selectedImageCollectionIndex.siblingAtColumn(ImageCollection::Column::DimensionTag), _tagAction.getString());
                    imageCollectionsModel.guessDimensionNames(selectedImageCollectionIndex);

                    progressDialog.setValue(_imageLoaderPlugin.getSelectedRows().indexOf(selectedImageCollectionIndex));

                    QCoreApplication::processEvents();
                }
            }
            setEnabled(true);
        }
        emit endEstablishDimensionNames();
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
