#include "AddCoordinatesPointsAction.h"
#include "ImageLoaderPlugin.h"

AddCoordinatesPointsAction::AddCoordinatesPointsAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    ToggleAction(parent, "Add 2D coordinates points dataset"),
    _imageLoaderPlugin(imageLoaderPlugin)
{
    setEnabled(false);
    setToolTip("Determines whether to add an additional two-dimensional points dataset with the x- and y coordinates");
    setSettingsPrefix(&imageLoaderPlugin, "CreateCoordinatesPoints");

    connect(this, &ToggleAction::toggled, this, &AddCoordinatesPointsAction::updateRows);

    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &AddCoordinatesPointsAction::dataChanged);
    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, this, &AddCoordinatesPointsAction::updateStateFromModel);

    updateStateFromModel();
}

void AddCoordinatesPointsAction::setAddCoordinatesPointsSilently(bool addCoordinatesPoints)
{
    disconnect(this, &AddCoordinatesPointsAction::toggled, this, nullptr);
    {
        setChecked(addCoordinatesPoints);
    }
    connect(this, &AddCoordinatesPointsAction::toggled, this, &AddCoordinatesPointsAction::updateRows);
}

void AddCoordinatesPointsAction::updateRows()
{
    disconnect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, nullptr);
    {
        for (const auto& selectedRow : _imageLoaderPlugin.getSelectedRows())
            _imageLoaderPlugin.getImageCollectionsModel().setData(selectedRow.siblingAtColumn(ImageCollection::Column::AddCoordinatesPoints), isChecked());
    }
    connect(&_imageLoaderPlugin.getImageCollectionsModel(), &QAbstractItemModel::dataChanged, this, &AddCoordinatesPointsAction::dataChanged);
}

void AddCoordinatesPointsAction::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles /*= QList<int>()*/)
{
    if (isColumnInModelIndexRange(topLeft, bottomRight, ImageCollection::Column::AddCoordinatesPoints))
        updateStateFromModel();
}

void AddCoordinatesPointsAction::updateStateFromModel()
{
    const auto selectedRows         = _imageLoaderPlugin.getSelectedRows();
    const auto numberOfSelectedRows = selectedRows.count();

    if (numberOfSelectedRows == 0) {
        setEnabled(false);
        setAddCoordinatesPointsSilently(false);
    }

    if (numberOfSelectedRows == 1) {
        setEnabled(true);
        setAddCoordinatesPointsSilently(selectedRows.first().siblingAtColumn(ImageCollection::Column::AddCoordinatesPoints).data(Qt::EditRole).toBool());
    }

    if (numberOfSelectedRows >= 2) {
        setEnabled(false);
        setAddCoordinatesPointsSilently(false);
    }
}
