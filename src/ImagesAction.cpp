#include "ImagesAction.h"
#include "ImageLoaderPlugin.h"

#include <QHeaderView>

using namespace mv::gui;

ImagesAction::ImagesAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent, "Images"),
    _imageLoaderPlugin(imageLoaderPlugin),
    _selectAllAction(this, "Select all"),
    _selectNoneAction(this, "Select none"),
    _selectInvertAction(this, "Select invert"),
    _guessDimensionNamesAction(this, "Guess dimension names")
{
    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, [this](const QItemSelection& selected, const QItemSelection& deselected) {
        setEnabled(_imageLoaderPlugin.getSelectedRows().count() == 1);
    });

    connect(&_selectAllAction, &TriggerAction::triggered, [this]() {
        for (const auto& index : _imageLoaderPlugin.getSelectedRows())
            _imageLoaderPlugin.getImageCollectionsModel().selectAll(index);
    });

    connect(&_selectNoneAction, &TriggerAction::triggered, [this]() {
        for (const auto& index : _imageLoaderPlugin.getSelectedRows())
            _imageLoaderPlugin.getImageCollectionsModel().selectNone(index);
    });

    connect(&_selectInvertAction, &TriggerAction::triggered, [this]() {
        for (const auto& index : _imageLoaderPlugin.getSelectedRows())
            _imageLoaderPlugin.getImageCollectionsModel().invertSelection(index);
    });

    connect(&_guessDimensionNamesAction, &TriggerAction::triggered, [this]() {
        for (const auto& index : _imageLoaderPlugin.getSelectedRows())
            _imageLoaderPlugin.getImageCollectionsModel().guessDimensionNames(index);
    });
}

ImagesAction::Widget::Widget(QWidget* parent, ImagesAction* imagesAction, const std::int32_t& widgetFlags) :
    WidgetActionWidget(parent, imagesAction, widgetFlags),
    _imagesAction(imagesAction),
    _treeView()
{
    auto mainLayout = new QVBoxLayout();

    auto& imageLoaderPlugin = imagesAction->_imageLoaderPlugin;

    connect(&imagesAction->_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, this, &ImagesAction::Widget::updateTreeView);
    connect(&imagesAction->_imageLoaderPlugin.getImageCollectionsModel(), &ImageCollectionsModel::rowsRemoved, this, &ImagesAction::Widget::updateTreeView);
    connect(&imagesAction->_imageLoaderPlugin.getImageCollectionsModel(), &ImageCollectionsModel::rowsInserted, this, &ImagesAction::Widget::updateTreeView);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(&_treeView);

    auto selectionLayout = new QHBoxLayout();

    selectionLayout->addWidget(imagesAction->_selectAllAction.createWidget(this));
    selectionLayout->addWidget(imagesAction->_selectNoneAction.createWidget(this));
    selectionLayout->addWidget(imagesAction->_selectInvertAction.createWidget(this));
    selectionLayout->addStretch(1);
    selectionLayout->addWidget(imagesAction->_guessDimensionNamesAction.createWidget(this));

    mainLayout->addLayout(selectionLayout);

    setLayout(mainLayout);

    updateTreeView();
}

void ImagesAction::Widget::updateTreeView()
{
    const auto selectedRows = _imagesAction->_imageLoaderPlugin.getSelectedRows();

    _treeView.setModel(!selectedRows.isEmpty() ? &_imagesAction->_imageLoaderPlugin.getImageCollectionsModel() : nullptr);
    _treeView.setRootIndex(selectedRows.count() == 1 ? selectedRows.first() : QModelIndex());

    auto header = _treeView.header();

    for (int column = ult(ImageCollection::Column::Start); column <= ult(ImageCollection::Column::End); column++)
        header->hideSection(column);

    header->setHidden(selectedRows.count() != 1);

    if (selectedRows.count() >= 1) {
        header->setSectionResizeMode(ult(ImageCollection::Image::Column::ShouldLoad), QHeaderView::Fixed);
        header->setSectionResizeMode(ult(ImageCollection::Image::Column::DimensionName), QHeaderView::Interactive);
        header->setSectionResizeMode(ult(ImageCollection::Image::Column::FileName), QHeaderView::Interactive);
    }

    header->setMinimumSectionSize(20);
    header->resizeSection(ult(ImageCollection::Image::Column::ShouldLoad), 20);

    _treeView.resizeColumnToContents(ult(ImageCollection::Image::Column::FileName));
    _treeView.resizeColumnToContents(ult(ImageCollection::Image::Column::DimensionName));

    header->hideSection(ult(ImageCollection::Image::Column::FileName));
}
