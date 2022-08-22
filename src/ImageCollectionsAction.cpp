#include "ImageCollectionsAction.h"
#include "ImageLoaderPlugin.h"

#include <QHeaderView>

using namespace hdps::gui;

const QMap<ImageCollectionsAction::ScaleFactor, TriggersAction::Trigger> ImageCollectionsAction::triggers = QMap<ImageCollectionsAction::ScaleFactor, TriggersAction::Trigger>({
    { ImageCollectionsAction::Perc5, TriggersAction::Trigger("5.0%", "Scale down to 5%") },
    { ImageCollectionsAction::Perc10, TriggersAction::Trigger("10.0%", "Scale down to 10%") },
    { ImageCollectionsAction::Perc25, TriggersAction::Trigger("25.0%", "Scale down to 25%") },
    { ImageCollectionsAction::Perc50, TriggersAction::Trigger("50.0%", "Scale down to 50%") },
    { ImageCollectionsAction::Perc75, TriggersAction::Trigger("75.0%", "Scale down to 75%") }
});

const QMap<ImageCollectionsAction::ScaleFactor, float> ImageCollectionsAction::scaleFactors = QMap<ImageCollectionsAction::ScaleFactor, float>({
    { ImageCollectionsAction::Perc5, 5.0f },
    { ImageCollectionsAction::Perc10, 10.0f },
    { ImageCollectionsAction::Perc25, 25.0f },
    { ImageCollectionsAction::Perc50, 50.0f },
    { ImageCollectionsAction::Perc75, 75.0f }
});

ImageCollectionsAction::ImageCollectionsAction(QWidget* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent),
    _imageLoaderPlugin(imageLoaderPlugin),
    _filterAction(this, "Filter"),
    _datasetNameAction(this, "Dataset name"),
    _loadAsAction(this, "Load as", { "Image sequence", "Image stack" }),
    _dataLayoutAction(this, imageLoaderPlugin),
    _dimensionTagAction(this, imageLoaderPlugin),
    _imagesAction(this, imageLoaderPlugin),
    _subsamplingEnabledAction(this, "Subsampling"),
    _subsamplingRatioAction(this, "Ratio", 1.0f, 100.0f, 50.0f, 50.0f, 1),
    _subsamplingDefaultRatiosAction(this, "Default ratios", triggers.values().toVector()),
    _subsamplingFilterTypeAction(this, "Filter type", { "Box", "Bilinear", "BSpline", "Bicubic", "Catmull Rom", "Lanczos" })
{
    setText("Image collections");

    _filterAction.setClearable(true);
    _filterAction.setToolTip("Image collection name filter");

    _datasetNameAction.setEnabled(false);
    _datasetNameAction.setToolTip("Name of the dataset in HDPS");

    _dimensionTagAction.setEnabled(false);
    _dimensionTagAction.setToolTip("The dimension tag for multi-page TIFF files");
    _dimensionTagAction.setTag("");

    _imageLoaderPlugin.getConversionPickerAction().setEnabled(false);

    _imagesAction.setEnabled(false);

    _subsamplingRatioAction.setToolTip("Subsampling ratio");
    _subsamplingRatioAction.setSuffix("%");

    _subsamplingFilterTypeAction.setToolTip("Type of subsampling filter");
    _subsamplingFilterTypeAction.setPlaceHolderString("Pick subsampling filter");

    connect(&_subsamplingDefaultRatiosAction, &TriggersAction::triggered, this, [this](std::int32_t triggerIndex) {
        _subsamplingRatioAction.setValue(scaleFactors.values().at(triggerIndex));
    });

    connect(&_filterAction, &StringAction::stringChanged, [this](QString text) {
        _imageLoaderPlugin.getImageCollectionsFilterModel().setFilter(text);
    });

    const auto updateSubsamplingFilter = [this](const std::int32_t& subsamplingFilter) -> void {
        for (const auto& selectedRow : _imageLoaderPlugin.getImageCollectionsModel().selectionModel().selectedRows())
            _imageLoaderPlugin.getImageCollectionsModel().setData(_imageLoaderPlugin.getImageCollectionsFilterModel().mapToSource(selectedRow).siblingAtColumn(ImageCollection::Column::SubsamplingFilter), subsamplingFilter);
    };

    connect(&_subsamplingFilterTypeAction, &OptionAction::currentIndexChanged, this, updateSubsamplingFilter);

    const auto setSubsamplingFilter = [this, updateSubsamplingFilter](const std::int32_t& subsamplingFilter) -> void {
        disconnect(&_subsamplingFilterTypeAction, &OptionAction::currentIndexChanged, this, nullptr);
        {
            _subsamplingFilterTypeAction.setCurrentIndex(subsamplingFilter);
        }
        connect(&_subsamplingFilterTypeAction, &OptionAction::currentIndexChanged, this, updateSubsamplingFilter);
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

    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, [this, setSubsamplingFilter](const QItemSelection& selected, const QItemSelection& deselected) {
        const auto selectedRows = _imageLoaderPlugin.getSelectedImageCollectionIndices();
        const auto numberOfSelectedRows = selectedRows.count();

        if (numberOfSelectedRows == 0) {
            _datasetNameAction.setEnabled(false);
            _datasetNameAction.setString("");
            _imageLoaderPlugin.getConversionPickerAction().setEnabled(false);
            _imageLoaderPlugin.getConversionPickerAction().setCurrentPluginTriggerAction(nullptr);
            _imagesAction.setEnabled(false);
            _subsamplingEnabledAction.setEnabled(false);
        }

        if (numberOfSelectedRows == 1) {
            _datasetNameAction.setEnabled(true);
            _datasetNameAction.setString(selectedRows.first().data(Qt::DisplayRole).toString());
            _imageLoaderPlugin.getConversionPickerAction().setCurrentPluginTriggerAction(selectedRows.first().siblingAtColumn(ImageCollection::Column::Conversion).data(Qt::EditRole).toString());
            _imageLoaderPlugin.getConversionPickerAction().setEnabled(true);
            _imagesAction.setEnabled(true);
            _subsamplingEnabledAction.setEnabled(true);
            _subsamplingEnabledAction.setChecked(selectedRows.first().siblingAtColumn(ImageCollection::Column::SubsamplingEnabled).data(Qt::EditRole).toBool());
            setSubsamplingFilter(selectedRows.first().siblingAtColumn(ImageCollection::Column::SubsamplingFilter).data(Qt::EditRole).toInt());
        }

        if (numberOfSelectedRows >= 2) {
            _datasetNameAction.setEnabled(false);
            _datasetNameAction.setString("");
            _loadAsAction.setEnabled(true);

            QSet<QString> conversions;

            for (const auto& selectedRow : selectedRows)
                conversions.insert(selectedRow.siblingAtColumn(ImageCollection::Column::Conversion).data(Qt::EditRole).toString());

            _imageLoaderPlugin.getConversionPickerAction().setCurrentPluginTriggerAction((conversions.isEmpty() || conversions.count() == 2) ? "" : conversions.values().first());
            _imageLoaderPlugin.getConversionPickerAction().setEnabled(true);
            _imagesAction.setEnabled(false);

            QSet<std::int32_t> subsamplingEnabled;

            for (const auto& selectedRow : selectedRows)
                subsamplingEnabled.insert(selectedRow.siblingAtColumn(ImageCollection::Column::SubsamplingEnabled).data(Qt::EditRole).toBool());

            if (subsamplingEnabled.count() == 1)
                setSubsamplingFilter(subsamplingEnabled.values().first());

            if (subsamplingEnabled.count() >= 2)
                _subsamplingEnabledAction.setIndeterminate(true);

            QSet<std::int32_t> subsamplingFilterTypes;

            for (const auto& selectedRow : selectedRows)
                subsamplingFilterTypes.insert(selectedRow.siblingAtColumn(ImageCollection::Column::SubsamplingFilter).data(Qt::EditRole).toInt());

            if (subsamplingFilterTypes.count() == 1)
                setSubsamplingFilter(subsamplingFilterTypes.values().first());

            if (subsamplingFilterTypes.count() >= 2)
                setSubsamplingFilter(-1);

            _subsamplingEnabledAction.setEnabled(true);
        }

        if (selectedRows.count() >= 1)
            for (const auto& selectedRow : selectedRows)
                _imageLoaderPlugin.getImageCollectionsModel().guessDimensionNames(selectedRow);
    });

    _filterAction.setSettingsPrefix(&imageLoaderPlugin, "ImageNameFilter");

    const auto updateSubsamplingActions = [this]() -> void {
        _subsamplingRatioAction.setEnabled(_subsamplingEnabledAction.isChecked());
        _subsamplingDefaultRatiosAction.setEnabled(_subsamplingEnabledAction.isChecked());
        _subsamplingFilterTypeAction.setEnabled(_subsamplingEnabledAction.isChecked());
    };

    connect(&_subsamplingEnabledAction, &ToggleAction::toggled, this, [this, updateSubsamplingActions](bool toggled) -> void {
        updateSubsamplingActions();

        const auto selectedImageCollectionIndices = _imageLoaderPlugin.getSelectedImageCollectionIndices();

        for (const auto& selectedImageCollectionIndex : selectedImageCollectionIndices)
            _imageLoaderPlugin.getImageCollectionsModel().setData(selectedImageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::SubsamplingEnabled)), toggled);
    });

    connect(&_subsamplingRatioAction, &DecimalAction::valueChanged, this, [this](const float& value) -> void {
        const auto selectedImageCollectionIndices = _imageLoaderPlugin.getSelectedImageCollectionIndices();

        for (const auto& selectedImageCollectionIndex : selectedImageCollectionIndices)
            _imageLoaderPlugin.getImageCollectionsModel().setData(selectedImageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::SubsamplingRatio)), 0.01f * value);
    });

    connect(&_subsamplingFilterTypeAction, &OptionAction::currentIndexChanged, this, [this](const std::int32_t& currentIndex) -> void {
        const auto selectedImageCollectionIndices = _imageLoaderPlugin.getSelectedImageCollectionIndices();

        for (const auto& selectedImageCollectionIndex : selectedImageCollectionIndices)
            _imageLoaderPlugin.getImageCollectionsModel().setData(selectedImageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::SubsamplingFilter)), currentIndex);
    });

    updateSubsamplingActions();
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

    subLayout->addWidget(imageCollectionsAction->_dataLayoutAction.createLabelWidget(this), 2, 0);
    subLayout->addWidget(imageCollectionsAction->_dataLayoutAction.createWidget(this), 2, 1);

    subLayout->addWidget(imageCollectionsAction->_dimensionTagAction.createLabelWidget(this), 3, 0);
    subLayout->addWidget(imageCollectionsAction->_dimensionTagAction.createWidget(this), 3, 1);

    subLayout->addWidget(imageCollectionsAction->_imageLoaderPlugin.getConversionPickerAction().createLabelWidget(this), 4, 0);
    subLayout->addWidget(imageCollectionsAction->_imageLoaderPlugin.getConversionPickerAction().createWidget(this), 4, 1);

    subLayout->addWidget(imageCollectionsAction->_imagesAction.createLabelWidget(this), 5, 0);
    subLayout->addWidget(imageCollectionsAction->_imagesAction.createWidget(this), 5, 1);

    subLayout->addWidget(imageCollectionsAction->_subsamplingEnabledAction.createWidget(this), 6, 1);

    auto ratioLayout = new QHBoxLayout();

    ratioLayout->setContentsMargins(0, 0, 0, 0);
    ratioLayout->addWidget(imageCollectionsAction->_subsamplingRatioAction.createWidget(this));
    ratioLayout->addWidget(imageCollectionsAction->_subsamplingDefaultRatiosAction.createWidget(this));

    subLayout->addLayout(ratioLayout, 7, 1);

    subLayout->addWidget(imageCollectionsAction->_subsamplingFilterTypeAction.createLabelWidget(this), 8, 0);
    subLayout->addWidget(imageCollectionsAction->_subsamplingFilterTypeAction.createWidget(this), 8, 1);

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
