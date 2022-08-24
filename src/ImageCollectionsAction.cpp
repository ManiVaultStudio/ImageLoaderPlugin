#include "ImageCollectionsAction.h"
#include "ImageLoaderPlugin.h"

#include <QHeaderView>

using namespace hdps::gui;

ImageCollectionsAction::ImageCollectionsAction(QWidget* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent),
    _imageLoaderPlugin(imageLoaderPlugin),
    _filterAction(this, "Filter"),
    _datasetNameAction(this, imageLoaderPlugin),
    _dataLayoutAction(this, imageLoaderPlugin),
    _subsamplingAction(this, imageLoaderPlugin),
    _dimensionTagAction(this, imageLoaderPlugin),
    _imagesAction(this, imageLoaderPlugin),
    _groupDataAction(this, imageLoaderPlugin)
{
    setText("Image collections");

    _filterAction.setClearable(true);
    _filterAction.setToolTip("Image collection name filter");

    connect(&_filterAction, &StringAction::stringChanged, this, [this](const QString& string) -> void {
        _imageLoaderPlugin.getImageCollectionsFilterModel().setFilter(string);
    });

    _imagesAction.setEnabled(false);

    const auto updateReadOnly = [this]() -> void {
        setEnabled(_imageLoaderPlugin.getImageCollectionsModel().rowCount(QModelIndex()) >= 1);
    };

    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &ImageCollectionsModel::rowsRemoved, this, updateReadOnly);
    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &ImageCollectionsModel::rowsInserted, this, updateReadOnly);

    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, [this](const QItemSelection& selected, const QItemSelection& deselected) {
        const auto selectedRows = _imageLoaderPlugin.getSelectedRows();
        const auto numberOfSelectedRows = selectedRows.count();

        if (numberOfSelectedRows == 0) {
            _imagesAction.setEnabled(false);
        }

        if (numberOfSelectedRows == 1) {
            _imagesAction.setEnabled(true);
        }

        if (numberOfSelectedRows >= 2) {
            _imagesAction.setEnabled(false);
        }

        if (selectedRows.count() == 1)
            for (const auto& selectedRow : selectedRows)
                _imageLoaderPlugin.getImageCollectionsModel().guessDimensionNames(selectedRow);
    });

    _filterAction.setSettingsPrefix(&imageLoaderPlugin, "ImageNameFilter");
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

    auto header = _treeView.header();

    header->hideSection(ult(ImageCollection::Column::NoSelectedImages));
    header->hideSection(ult(ImageCollection::Column::DimensionTag));
    header->hideSection(ult(ImageCollection::Column::IsMultiPage));
    header->hideSection(ult(ImageCollection::Column::SourceSize));
    header->hideSection(ult(ImageCollection::Column::TargetSize));
    header->hideSection(ult(ImageCollection::Column::Type));
    header->hideSection(ult(ImageCollection::Column::SubsamplingType));
    header->hideSection(ult(ImageCollection::Column::SubsamplingRatio));
    header->hideSection(ult(ImageCollection::Column::SubsamplingFilter));
    header->hideSection(ult(ImageCollection::Column::SubsamplingNumberOfLevels));
    header->hideSection(ult(ImageCollection::Column::SubsamplingLevelFactor));
    header->hideSection(ult(ImageCollection::Column::Conversion));
    header->hideSection(ult(ImageCollection::Image::Column::ShouldLoad));
    header->hideSection(ult(ImageCollection::Image::Column::FileName));
    header->hideSection(ult(ImageCollection::Image::Column::DimensionName));
    header->hideSection(ult(ImageCollection::Image::Column::FilePath));

    connect(&_imageCollectionsAction->_imageLoaderPlugin.getImageCollectionsModel(), &ImageCollectionsModel::rowsInserted, this, &ImageCollectionsAction::Widget::updateTreeView);

    mainLayout->addWidget(&_treeView);

    auto subLayout = new QGridLayout();

    subLayout->addWidget(imageCollectionsAction->_filterAction.createLabelWidget(this), 0, 0);
    subLayout->addWidget(imageCollectionsAction->_filterAction.createWidget(this), 0, 1);

    subLayout->addWidget(imageCollectionsAction->_datasetNameAction.createLabelWidget(this), 1, 0);
    subLayout->addWidget(imageCollectionsAction->_datasetNameAction.createWidget(this), 1, 1);

    subLayout->addWidget(imageCollectionsAction->_dataLayoutAction.createLabelWidget(this), 2, 0);
    subLayout->addWidget(imageCollectionsAction->_dataLayoutAction.createWidget(this), 2, 1);

    subLayout->addWidget(imageCollectionsAction->_dimensionTagAction.createLabelWidget(this), 3, 0);
    subLayout->addWidget(imageCollectionsAction->_dimensionTagAction.createWidget(this), 3, 1);

    subLayout->addWidget(imageCollectionsAction->_imageLoaderPlugin.getConversionPickerAction().createLabelWidget(this), 4, 0);
    subLayout->addWidget(imageCollectionsAction->_imageLoaderPlugin.getConversionPickerAction().createWidget(this), 4, 1);

    subLayout->addWidget(imageCollectionsAction->_imagesAction.createLabelWidget(this), 5, 0);
    subLayout->addWidget(imageCollectionsAction->_imagesAction.createWidget(this), 5, 1);

    subLayout->addWidget(imageCollectionsAction->_subsamplingAction.getTypeAction().createLabelWidget(this), 6, 0);
    subLayout->addWidget(imageCollectionsAction->_subsamplingAction.getTypeAction().createWidget(this), 6, 1);

    subLayout->addWidget(imageCollectionsAction->_subsamplingAction.getRatioAction().createLabelWidget(this), 7, 0);
    subLayout->addWidget(imageCollectionsAction->_subsamplingAction.getRatioAction().createWidget(this), 7, 1);

    subLayout->addWidget(imageCollectionsAction->_subsamplingAction.getFilterTypeAction().createLabelWidget(this), 8, 0);
    subLayout->addWidget(imageCollectionsAction->_subsamplingAction.getFilterTypeAction().createWidget(this), 8, 1);

    subLayout->addWidget(imageCollectionsAction->_subsamplingAction.getNumberOfLevelsAction().createLabelWidget(this), 9, 0);
    subLayout->addWidget(imageCollectionsAction->_subsamplingAction.getNumberOfLevelsAction().createWidget(this), 9, 1);

    subLayout->addWidget(imageCollectionsAction->_groupDataAction.createWidget(this), 10, 1);

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

    _treeView.resizeColumnToContents(ult(ImageCollection::Column::Name));
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
