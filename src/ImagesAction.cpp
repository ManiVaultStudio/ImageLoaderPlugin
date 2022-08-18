#include "ImagesAction.h"
#include "ImageLoaderPlugin.h"

#include <QHeaderView>

using namespace hdps::gui;

ImagesAction::ImagesAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    WidgetAction(parent),
    _imageLoaderPlugin(imageLoaderPlugin)
{
    setText("Images");

    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, [this](const QItemSelection& selected, const QItemSelection& deselected) {
        setEnabled(_imageLoaderPlugin.getSelectedImageCollectionIndices().count() == 1);
    });
}

ImagesAction::Widget::Widget(QWidget* parent, ImagesAction* imagesAction, const std::int32_t& widgetFlags) :
    WidgetActionWidget(parent, imagesAction, widgetFlags),
    _imagesAction(imagesAction),
    _treeView()
{
    auto mainLayout = new QGridLayout();

    auto& imageLoaderPlugin = imagesAction->_imageLoaderPlugin;

    connect(&imagesAction->_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, this, &ImagesAction::Widget::updateTreeView);
    connect(&imagesAction->_imageLoaderPlugin.getImageCollectionsModel(), &ImageCollectionsModel::dataChanged, this, &ImagesAction::Widget::updateTreeView);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(&_treeView, 0, 1);

    setLayout(mainLayout);

    updateTreeView();
}

void ImagesAction::Widget::updateTreeView()
{
    const auto selectedRows = _imagesAction->_imageLoaderPlugin.getSelectedImageCollectionIndices();

    _treeView.setModel(!selectedRows.isEmpty() ? &_imagesAction->_imageLoaderPlugin.getImageCollectionsModel() : nullptr);
    _treeView.setRootIndex(selectedRows.count() == 1 ? selectedRows.first() : QModelIndex());

    auto header = _treeView.header();

    for (int column = ult(ImageCollection::Column::Start); column <= ult(ImageCollection::Column::End); column++)
        header->hideSection(column);

    header->setHidden(selectedRows.count() != 1);

    if (selectedRows.count() >= 1) {
        header->setSectionResizeMode(ult(ImageCollection::Image::Column::ShouldLoad), QHeaderView::Fixed);
        header->setSectionResizeMode(ult(ImageCollection::Image::Column::FileName), QHeaderView::ResizeToContents);
        header->setSectionResizeMode(ult(ImageCollection::Image::Column::DimensionName), QHeaderView::Interactive);
        header->setSectionResizeMode(ult(ImageCollection::Image::Column::FileName), QHeaderView::Interactive);
    }

    header->setMinimumSectionSize(20);
    header->resizeSection(ult(ImageCollection::Image::Column::ShouldLoad), 20);

    _treeView.resizeColumnToContents(ult(ImageCollection::Image::Column::FileName));
    _treeView.resizeColumnToContents(ult(ImageCollection::Image::Column::DimensionName));

    header->hideSection(ult(ImageCollection::Image::Column::FileName));
}
