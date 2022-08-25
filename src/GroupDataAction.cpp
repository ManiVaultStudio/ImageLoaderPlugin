#include "GroupDataAction.h"
#include "ImageLoaderPlugin.h"

GroupDataAction::GroupDataAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin) :
    ToggleAction(parent, "Group data"),
    _imageLoaderPlugin(imageLoaderPlugin),
    _enabledAction(this, "Add group dataset"),
    _nameAction(this, "Group dataset name")
{
    setEnabled(false);
    
    _enabledAction.setToolTip("Whether to create a group dataset");

    _nameAction.setToolTip("The name of the group dataset");
    _nameAction.setPlaceHolderString("Enter dataset group name...");

    connect(&_imageLoaderPlugin.getImageCollectionsModel().selectionModel(), &QItemSelectionModel::selectionChanged, this, &GroupDataAction::updateStateFromModel);

    updateStateFromModel();

    const auto updateNameActionReadOnly = [this]() -> void {
        _nameAction.setEnabled(_enabledAction.isChecked());
    };

    connect(&_enabledAction, &ToggleAction::toggled, this, updateNameActionReadOnly);

    updateNameActionReadOnly();
}

void GroupDataAction::updateStateFromModel()
{
    const auto selectedRows = _imageLoaderPlugin.getSelectedRows();

    QSet<std::int32_t> noDimensions;

    for (const auto& selectedRow : selectedRows)
        noDimensions.insert(selectedRow.siblingAtColumn(ImageCollection::Column::NoDimensions).data(Qt::EditRole).toInt());

    setEnabled(selectedRows.count() >= 2 ? (noDimensions.count() == 1) : false);

    if (selectedRows.count() <= 1)
        _nameAction.setString("");
}

GroupDataAction::Widget::Widget(QWidget* parent, GroupDataAction* groupDataAction, const std::int32_t& widgetFlags) :
    WidgetActionWidget(parent, groupDataAction, widgetFlags)
{
    auto mainLayout = new QHBoxLayout();

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(groupDataAction->_enabledAction.createWidget(this));
    mainLayout->addWidget(groupDataAction->_nameAction.createWidget(this), 1);

    setLayout(mainLayout);
}
