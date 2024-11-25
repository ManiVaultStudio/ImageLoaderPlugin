#include "MirrorAction.h"
#include "ImageLoaderPlugin.h"

MirrorAction::MirrorAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    HorizontalGroupAction(parent, "Mirror image"),
    _imageLoaderPlugin(imageLoaderPlugin),
    _horizontalAction(this, "Mirror horizontal"),
    _verticalAction(this, "Mirror vertical")
{
    setEnabled(false);
    setToolTip("Determines whether to add an additional two-dimensional points dataset with the x- and y coordinates");
    setSettingsPrefix(&imageLoaderPlugin, "Mirror");

    _horizontalAction.setToolTip("Whether to mirror horizontally");
    _verticalAction.setToolTip("Whether to mirror vertically");

    addAction(&_horizontalAction);
    addAction(&_verticalAction);

    connect(&_horizontalAction, &ToggleAction::toggled, this, &MirrorAction::updateRows);
    connect(&_verticalAction, &ToggleAction::toggled, this, &MirrorAction::updateRows);

    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &MirrorAction::dataChanged);
    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, this, &MirrorAction::updateStateFromModel);

    updateStateFromModel();
}

void MirrorAction::setAddCoordinatesPointsSilently(bool addCoordinatesPoints)
{
    //disconnect(this, &AddCoordinatesPointsAction::toggled, this, nullptr);
    //{
    //    setChecked(addCoordinatesPoints);
    //}
    //connect(this, &AddCoordinatesPointsAction::toggled, this, &AddCoordinatesPointsAction::updateRows);
}

void MirrorAction::updateRows()
{
    //disconnect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, nullptr);
    //{
    //    for (const auto& selectedRow : _imageLoaderPlugin.getSelectedRows())
    //        _imageLoaderPlugin.getImageCollectionsModel().setData(selectedRow.siblingAtColumn(ImageCollection::Column::AddCoordinatesPoints), isChecked());
    //}
    //connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &AddCoordinatesPointsAction::dataChanged);
}

void MirrorAction::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles /*= QList<int>()*/)
{
    if (isColumnInModelIndexRange(topLeft, bottomRight, ImageCollection::Column::DatasetName))
        updateStateFromModel();
}

void MirrorAction::updateStateFromModel()
{
    //const auto selectedRows         = _imageLoaderPlugin.getSelectedRows();
    //const auto numberOfSelectedRows = selectedRows.count();

    //if (numberOfSelectedRows == 0) {
    //    setEnabled(false);
    //    setAddCoordinatesPointsSilently(false);
    //}

    //if (numberOfSelectedRows == 1) {
    //    setEnabled(true);
    //    setAddCoordinatesPointsSilently(selectedRows.first().siblingAtColumn(ImageCollection::Column::AddCoordinatesPoints).data(Qt::EditRole).toBool());
    //}

    //if (numberOfSelectedRows >= 2) {
    //    setEnabled(false);
    //    setAddCoordinatesPointsSilently(false);
    //}
}
