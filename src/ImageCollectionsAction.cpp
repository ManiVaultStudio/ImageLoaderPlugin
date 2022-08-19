#include "ImageCollectionsAction.h"
#include "ImageLoaderPlugin.h"

#include <QHeaderView>

using namespace hdps::gui;

ImageCollectionsAction::ImageCollectionsAction(QWidget* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent),
    _imageLoaderPlugin(imageLoaderPlugin),
    _filterAction(this, "Filter"),
    _datasetNameAction(this, "Dataset name"),
    _loadAsAction(this, "Load as", { "Image sequence", "Image stack" }),
    _dimensionTagAction(this, imageLoaderPlugin),
    _imagesAction(this, imageLoaderPlugin)
{
    setText("Image collections");

    _filterAction.setClearable(true);
    _filterAction.setToolTip("Image collection name filter");

    _datasetNameAction.setEnabled(false);
    _datasetNameAction.setToolTip("Name of the dataset in HDPS");

    _loadAsAction.setEnabled(false);
    _loadAsAction.setToolTip("Determines the layout of the loaded image data");
    _loadAsAction.setPlaceHolderString("Choose data layout");
    _loadAsAction.setCurrentIndex(-1);

    _dimensionTagAction.setEnabled(false);
    _dimensionTagAction.setToolTip("The dimension tag for multi-page TIFF files");
    _dimensionTagAction.setTag("");

    _imageLoaderPlugin.getConversionPickerAction().setEnabled(false);

    _imagesAction.setEnabled(false);

    connect(&_filterAction, &StringAction::stringChanged, [this](QString text) {
        _imageLoaderPlugin.getImageCollectionsFilterModel().setFilter(text);
    });

    const auto updateLoadAs = [this](const std::uint32_t& currentIndex) -> void {
        if (currentIndex < 0)
            return;

        for (const auto& selectedRow : _imageLoaderPlugin.getImageCollectionsModel().selectionModel().selectedRows())
            _imageLoaderPlugin.getImageCollectionsModel().setData(_imageLoaderPlugin.getImageCollectionsFilterModel().mapToSource(selectedRow).siblingAtColumn(ImageCollection::Column::Type), currentIndex);
    };

    const auto setLoadAs = [this, updateLoadAs](const std::int32_t& loadAs) -> void {
        disconnect(&_loadAsAction, &OptionAction::currentIndexChanged, this, nullptr);
        {
            _loadAsAction.setCurrentIndex(loadAs);
        }
        connect(&_loadAsAction, &OptionAction::currentIndexChanged, this, updateLoadAs);
    };

    connect(&_loadAsAction, &OptionAction::currentIndexChanged, this, updateLoadAs);

    const auto updateTag = [this](const QString& tag) -> void {
        for (const auto& selectedRow : _imageLoaderPlugin.getImageCollectionsModel().selectionModel().selectedRows())
            _imageLoaderPlugin.getImageCollectionsModel().setData(_imageLoaderPlugin.getImageCollectionsFilterModel().mapToSource(selectedRow).siblingAtColumn(ImageCollection::Column::DimensionTag), tag);
    };

    connect(&_dimensionTagAction, &DimensionTagAction::tagChanged, this, updateTag);

    const auto setTag = [this, updateTag](const QString& tag) -> void {
        disconnect(&_dimensionTagAction, &DimensionTagAction::tagChanged, this, nullptr);
        {
            _dimensionTagAction.setTag(tag);
        }
        connect(&_dimensionTagAction, &DimensionTagAction::tagChanged, this, updateTag);
    };

    connect(&_imageLoaderPlugin.getConversionPickerAction(), &PluginTriggerPickerAction::currentPluginTriggerActionChanged, this, [this](const PluginTriggerAction* currentPluginTriggerAction) -> void {
        if (currentPluginTriggerAction == nullptr || currentPluginTriggerAction->getSha() == "")
            return;

        for (const auto& selectedRow : _imageLoaderPlugin.getImageCollectionsModel().selectionModel().selectedRows())
            _imageLoaderPlugin.getImageCollectionsModel().setData(_imageLoaderPlugin.getImageCollectionsFilterModel().mapToSource(selectedRow).siblingAtColumn(ImageCollection::Column::Conversion), currentPluginTriggerAction->getSha());
    });

    const auto updateReadOnly = [this]() -> void {
        setEnabled(_imageLoaderPlugin.getImageCollectionsModel().rowCount(QModelIndex()) >= 1);
    };

    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &ImageCollectionsModel::rowsRemoved, this, updateReadOnly);
    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &ImageCollectionsModel::rowsInserted, this, updateReadOnly);

    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, [this, setLoadAs, setTag](const QItemSelection& selected, const QItemSelection& deselected) {
        const auto selectedRows = _imageLoaderPlugin.getSelectedImageCollectionIndices();
        const auto numberOfSelectedRows = selectedRows.count();

        if (numberOfSelectedRows == 0) {
            _datasetNameAction.setEnabled(false);
            _datasetNameAction.setString("");
            _loadAsAction.setEnabled(false);
            setLoadAs(-1);
            _dimensionTagAction.setEnabled(false);
            
            setTag("");

            _imageLoaderPlugin.getConversionPickerAction().setEnabled(false);
            _imageLoaderPlugin.getConversionPickerAction().setCurrentPluginTriggerAction(nullptr);
            _imagesAction.setEnabled(false);
        }

        if (numberOfSelectedRows == 1) {
            _datasetNameAction.setEnabled(true);
            _datasetNameAction.setString(selectedRows.first().data(Qt::DisplayRole).toString());
            _loadAsAction.setEnabled(true);
            setLoadAs(selectedRows.first().siblingAtColumn(ImageCollection::Column::Type).data(Qt::EditRole).toInt());

            if (selectedRows.first().siblingAtColumn(ImageCollection::Column::IsMultiPage).data(Qt::EditRole).toBool()) {
                _dimensionTagAction.setEnabled(true);
                
                setTag(selectedRows.first().siblingAtColumn(ImageCollection::Column::DimensionTag).data(Qt::EditRole).toString());
            }
            else {
                _dimensionTagAction.setEnabled(false);
                
                setTag("");
            }

            _imageLoaderPlugin.getConversionPickerAction().setCurrentPluginTriggerAction(selectedRows.first().siblingAtColumn(ImageCollection::Column::Conversion).data(Qt::EditRole).toString());
            _imageLoaderPlugin.getConversionPickerAction().setEnabled(true);
            _imagesAction.setEnabled(true);
        }

        if (numberOfSelectedRows >= 2) {
            _datasetNameAction.setEnabled(false);
            _datasetNameAction.setString("");
            _loadAsAction.setEnabled(true);

            QSet<std::int32_t> types;

            for (const auto& selectedRow : selectedRows)
                types.insert(selectedRow.siblingAtColumn(ImageCollection::Column::Type).data(Qt::EditRole).toInt());

            if (types.count() == 1)
                setLoadAs(types.values().first());

            if (types.count() == 2)
                setLoadAs(-1);

            QSet<QString> conversions;

            for (const auto& selectedRow : selectedRows)
                conversions.insert(selectedRow.siblingAtColumn(ImageCollection::Column::Conversion).data(Qt::EditRole).toString());

            _imageLoaderPlugin.getConversionPickerAction().setCurrentPluginTriggerAction((conversions.isEmpty() || conversions.count() == 2) ? "" : conversions.values().first());
            _imageLoaderPlugin.getConversionPickerAction().setEnabled(true);
            _imagesAction.setEnabled(false);

            auto numberOfMultiPageTiffs = 0u;

            for (const auto& selectedRow : selectedRows)
                if (selectedRow.siblingAtColumn(ImageCollection::Column::IsMultiPage).data(Qt::EditRole).toBool())
                    numberOfMultiPageTiffs++;

            _dimensionTagAction.setEnabled(numberOfMultiPageTiffs > 0);

            QSet<QString> tags;

            for (const auto& selectedRow : selectedRows)
                tags.insert(selectedRow.siblingAtColumn(ImageCollection::Column::DimensionTag).data(Qt::EditRole).toString());

            if (tags.count() == 0)
                setTag("");
                
            if (tags.count() == 1)
                setTag(tags.values().first());

            if (tags.count() >= 2)
                setTag("");
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
    header->hideSection(ult(ImageCollection::Column::SubsamplingEnabled));
    header->hideSection(ult(ImageCollection::Column::SubsamplingRatio));
    header->hideSection(ult(ImageCollection::Column::SubsamplingFilter));
    header->hideSection(ult(ImageCollection::Column::Conversion));

    connect(&_imageCollectionsAction->_imageLoaderPlugin.getImageCollectionsModel(), &ImageCollectionsModel::rowsInserted, this, &ImageCollectionsAction::Widget::updateTreeView);

    mainLayout->addWidget(&_treeView);

    auto subLayout = new QGridLayout();

    subLayout->addWidget(imageCollectionsAction->_filterAction.createLabelWidget(this), 0, 0);
    subLayout->addWidget(imageCollectionsAction->_filterAction.createWidget(this), 0, 1);

    subLayout->addWidget(imageCollectionsAction->_datasetNameAction.createLabelWidget(this), 1, 0);
    subLayout->addWidget(imageCollectionsAction->_datasetNameAction.createWidget(this), 1, 1);

    subLayout->addWidget(imageCollectionsAction->_loadAsAction.createLabelWidget(this), 2, 0);
    subLayout->addWidget(imageCollectionsAction->_loadAsAction.createWidget(this), 2, 1);

    subLayout->addWidget(imageCollectionsAction->getDimensionTagAction().createLabelWidget(this), 3, 0);
    subLayout->addWidget(imageCollectionsAction->getDimensionTagAction().createWidget(this), 3, 1);

    subLayout->addWidget(imageCollectionsAction->_imageLoaderPlugin.getConversionPickerAction().createLabelWidget(this), 4, 0);
    subLayout->addWidget(imageCollectionsAction->_imageLoaderPlugin.getConversionPickerAction().createWidget(this), 4, 1);

    subLayout->addWidget(imageCollectionsAction->getImagesAction().createLabelWidget(this), 5, 0);
    subLayout->addWidget(imageCollectionsAction->getImagesAction().createWidget(this), 5, 1);

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
