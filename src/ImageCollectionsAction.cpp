#include "ImageCollectionsAction.h"
#include "ImageLoaderPlugin.h"

#include <QHeaderView>

using namespace hdps::gui;

ImageCollectionsAction::ImageCollectionsAction(QWidget* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent),
    _imageLoaderPlugin(imageLoaderPlugin),
    _filterAction(this, "Filter"),
    _datasetNameAction(this, "Dataset name"),
    _loadAsAction(this, "Load as", { "Image stack", "Image sequence" })
{
    setText("Image collections");

    _datasetNameAction.setEnabled(false);

    _loadAsAction.setToolTip("Determines the layout of the loaded image data");
    _loadAsAction.setPlaceHolderString("Choose data layout");

    connect(&_filterAction, &StringAction::stringChanged, [this](QString text) {
        _imageLoaderPlugin.getImageCollectionsFilterModel().setFilter(text);
    });

    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, [this](const QItemSelection& selected, const QItemSelection& deselected) {
        const auto selectedRows = _imageLoaderPlugin.getImageCollectionsModel().selectionModel().selectedRows();

        if (selectedRows.isEmpty())
            return;
        return;
        if (selectedRows.count() == 1) {
            const auto selectedImageCollectionIndex = _imageLoaderPlugin.getSelectedImageCollectionIndex();

            _datasetNameAction.setEnabled(selectedRows.count() == 1);
            _datasetNameAction.setString(_imageLoaderPlugin.getImageCollectionsFilterModel().mapToSource(selectedRows.first()).data(Qt::DisplayRole).toString());
            _loadAsAction.setCurrentIndex(selectedImageCollectionIndex.siblingAtColumn(ImageCollection::Column::Type).data(Qt::EditRole).toInt());
            //_conversionAction.setCurrentPluginTriggerAction(selectedImageCollectionIndex.siblingAtColumn(ImageCollection::Column::Conversion).data(Qt::EditRole).toString());
        }
        
        if (selectedRows.count() >= 2) {
            _datasetNameAction.setEnabled(false);
            _datasetNameAction.setString("");
            _loadAsAction.setCurrentIndex(-1);
            //_conversionAction.reset();
        }
    });
}

ImageCollectionsAction::Widget::Widget(QWidget* parent, ImageCollectionsAction* imageCollectionsAction, const std::int32_t& widgetFlags) :
    WidgetActionWidget(parent, imageCollectionsAction, widgetFlags),
    _imageCollectionsAction(imageCollectionsAction),
    _treeView(this)
{
    setEnabled(false);

    auto mainLayout = new QVBoxLayout();

    _treeView.setModel(&imageCollectionsAction->_imageLoaderPlugin.getImageCollectionsFilterModel());
    _treeView.setSelectionModel(&imageCollectionsAction->_imageLoaderPlugin.getImageCollectionsModel().selectionModel());
    _treeView.setRootIsDecorated(false);
    _treeView.setSortingEnabled(true);
    _treeView.setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);

    _treeView.header()->hideSection(ult(ImageCollection::Column::NoSelectedImages));
    _treeView.header()->hideSection(ult(ImageCollection::Column::DimensionTag));
    _treeView.header()->hideSection(ult(ImageCollection::Column::IsMultiPage));
    _treeView.header()->hideSection(ult(ImageCollection::Column::SourceSize));
    _treeView.header()->hideSection(ult(ImageCollection::Column::TargetSize));
    _treeView.header()->hideSection(ult(ImageCollection::Column::Type));
    _treeView.header()->hideSection(ult(ImageCollection::Column::SubsamplingEnabled));
    _treeView.header()->hideSection(ult(ImageCollection::Column::SubsamplingRatio));
    _treeView.header()->hideSection(ult(ImageCollection::Column::SubsamplingFilter));
    _treeView.header()->hideSection(ult(ImageCollection::Column::Conversion));

    connect(&_imageCollectionsAction->_imageLoaderPlugin.getImageCollectionsModel(), &ImageCollectionsModel::rowsInserted, this, &ImageCollectionsAction::Widget::updateTreeView);

    mainLayout->addWidget(&_treeView);

    auto subLayout = new QGridLayout();

    subLayout->addWidget(imageCollectionsAction->_filterAction.createLabelWidget(this), 0, 0);
    subLayout->addWidget(imageCollectionsAction->_filterAction.createWidget(this), 0, 1);

    subLayout->addWidget(imageCollectionsAction->_datasetNameAction.createLabelWidget(this), 1, 0);
    subLayout->addWidget(imageCollectionsAction->_datasetNameAction.createWidget(this), 1, 1);

    subLayout->addWidget(imageCollectionsAction->_loadAsAction.createLabelWidget(this), 2, 0);
    subLayout->addWidget(imageCollectionsAction->_loadAsAction.createWidget(this), 2, 1);

    subLayout->addWidget(imageCollectionsAction->_imageLoaderPlugin.getConversionPickerAction().createLabelWidget(this), 3, 0);
    subLayout->addWidget(imageCollectionsAction->_imageLoaderPlugin.getConversionPickerAction().createWidget(this), 3, 1);

    mainLayout->addLayout(subLayout);

    setPopupLayout(mainLayout);

    layout()->setContentsMargins(0, 0, 0, 0);

    updateTreeView();
}

void ImageCollectionsAction::Widget::updateTreeView()
{
    auto& imageCollectionsModel = _imageCollectionsAction->_imageLoaderPlugin.getImageCollectionsModel();

    if (imageCollectionsModel.rowCount(QModelIndex()) == 0)
        return;

    setEnabled(true);

    _treeView.resizeColumnToContents(ult(ImageCollection::Column::DatasetName));
    _treeView.resizeColumnToContents(ult(ImageCollection::Column::ImageType));
    _treeView.resizeColumnToContents(ult(ImageCollection::Column::ImageFormat));
    _treeView.resizeColumnToContents(ult(ImageCollection::Column::ToGrayscale));
    _treeView.resizeColumnToContents(ult(ImageCollection::Column::NoImages));
    _treeView.resizeColumnToContents(ult(ImageCollection::Column::NoSelectedImages));
    _treeView.resizeColumnToContents(ult(ImageCollection::Column::SourceSize));
    _treeView.resizeColumnToContents(ult(ImageCollection::Column::TargetSize));
    _treeView.resizeColumnToContents(ult(ImageCollection::Column::TargetWidth));
    _treeView.resizeColumnToContents(ult(ImageCollection::Column::TargetHeight));
    _treeView.resizeColumnToContents(ult(ImageCollection::Column::NoPoints));
    _treeView.resizeColumnToContents(ult(ImageCollection::Column::NoDimensions));
    _treeView.resizeColumnToContents(ult(ImageCollection::Column::Memory));
    _treeView.resizeColumnToContents(ult(ImageCollection::Column::Directory));
    _treeView.resizeColumnToContents(ult(ImageCollection::Column::Type));
}
